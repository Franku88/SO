#include <xinu.h>
#include "titlescreen.h"
#include "playerImage.h"
#include "enemy.h"
#include "boss.h"
#include "gameover.h"
#include "shoot.h"
#include "endscreen.h"

extern unsigned char tecla_actual;
typedef unsigned short u16;

// Macro para generacion de colores RGB
#define RGB(r, g, b) (r | (g << 5) | (b << 10))
// #define REG_DISPCNT *(u16 *)0x4000000
#define extern videoBuffer
#define MODE3 3
#define BG2_ENABLE (1 << 10)
#define WHITE RGB(31, 31, 31)
#define BLACK RGB(0, 0, 0)

/*
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
*/

//#define BUTTONS *(volatile unsigned int *)0x4000130
#define BUTTON_A	0x24 //j
#define BUTTON_B	0x25 //k
#define BUTTON_SELECT	0x03 //2
#define BUTTON_START	0x2c
#define BUTTON_UP	0x11 //w
#define BUTTON_LEFT	0x1e //a
#define BUTTON_DOWN 	0x1f //s
#define BUTTON_RIGHT	0x20 //d
#define BUTTON_R	'1'
#define BUTTON_L	'2'
#define KEY_DOWN_NOW(key)  (tecla_actual == key)
#define BUTTON_END 0xb //0

//variable definitions
#define playerspeed 2
#define enemyspeed 1
#define fastXSpeed 3
#define fastYSpeed 2
//cantidad max de disparos
#define N_SHOOTS 10

//objects
struct Players {
	volatile u16 playerX;
	volatile u16 playerY;
};
struct Enemy {
	volatile u16 enemyX;
	volatile u16 enemyY;
};
struct FastEnemy {
	volatile u16 fastX;
	volatile u16 fastY;
};

//vars globales
int shoots[N_SHOOTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int curr_shot = 0;
int shootCooldown = 0;
int playerImmunityTimer = 0;
//para vidas y puntuacion
int score = 0;
int lives = 3;

//prototypes
void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16* image);
void delay_galaga();
void waitForVBlank();
int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY);
void endGame();
//Added
void initialize(struct Players *player, struct Enemy easyEnemies[], struct Enemy hardEnemies[], struct FastEnemy *fast);
void titleScreen();
void endScreen();
void handleInput(struct Players *player, int *shoots);
void drawPlayer(struct Players *player);
void spritePlayer(struct Players *player);
void drawEnemies(struct Enemy easyEnemies[], struct Enemy hardEnemies[], struct FastEnemy *fast, struct Players *player);
void drawShots(int *shoots, struct Enemy easyEnemies[]);
void game(pid32 PID_CONTROL); //Proceso 1
void handleScore(); //Proceso 2
//int galaga(); //Proceso 3

//Almacena globalmente los processId
int PID_GAME;
int PID_SCORE;
//CODIGOS PARA PROCESOS
#define MSG_NONE -1
#define MSG_END 1
#define MSG_DAMAGE 2
#define MSG_SCORED 3

//PROCESO 3 - control
int galaga(void) {
	PID_GAME = create(game, 2048, 10, "Game process", 1, getpid());
	PID_SCORE = create(handleScore, 2048, 10, "Score process", 0);
	resume(PID_GAME);
	resume(PID_SCORE);
	
	if (receive() == MSG_END) {
		kill(PID_GAME);
		kill(PID_SCORE);
		printf("GALAGA finalizado.\n");
	}
	return 0;
}

//PROCESO 1 - Juego
void game(pid32 PID_CONTROL) {
	struct Enemy easyEnemies[9];
	struct Enemy hardEnemies[9];
	struct FastEnemy fast;
	struct Players player;
	
	//inicializacion principal
	initialize(&player, easyEnemies, hardEnemies, &fast);
	
	//muestra title screen
	titleScreen();
	
	//bucle principal
	while(1) {
		//go back to title screen if select button is pressed
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			initialize(&player, easyEnemies, hardEnemies, &fast);
			titleScreen();
			break;
		}
		
		//Cierra el juego
		if (KEY_DOWN_NOW(BUTTON_END)) {			
			endScreen();
			send(PID_CONTROL, MSG_END);
			break;
		}
		
		//manejo de player input
		handleInput(&player, shoots);
		//Dibujan sprites y manejan colisiones
		waitForVBlank();
		sleepms(50); // Tiempo entre cada frame
		drawPlayer(&player);
		drawEnemies(easyEnemies, hardEnemies, &fast, &player); //Colisiones con enemigos
		drawShots(shoots, easyEnemies); //Colisiones de disparos con enemigos
	}
}

// PROCESO 2 - Manejo de score, vidas los muestra en pantalla
void handleScore() {
	int msg = MSG_NONE;
	while (1) {
		// Verificar si la vida llegó a 0
		if (lives == 0) {
			lives = 3; // Reiniciar vidas
			score = 0;
			break;
		}
		if (msg == MSG_DAMAGE || msg == MSG_NONE) {
			char vidasStr[10];
			print_text_on_vga(250, 30, "<------------------>");
			sprintf(vidasStr, "Vidas: %d", lives);
			print_text_on_vga(260, 40, vidasStr); // Dibuja el texto de vidas en pantalla
		}
		if (msg == MSG_SCORED || msg == MSG_NONE) {
			char scoreStr[20];
			sprintf(scoreStr, "Puntos: %d", score);
			print_text_on_vga(260, 60, scoreStr); // Dibuja el texto de puntuación en pantalla
			print_text_on_vga(250, 70, "<------------------>");
		}
		msg = receive();
    }
    endGame();
}

//Inicializa el juego
void initialize(struct Players *player, struct Enemy easyEnemies[], struct Enemy hardEnemies[], struct FastEnemy *fast) {
	//easy enemy wave set setup
	for (int a = 0; a < 9; a++) {
		easyEnemies[a].enemyX = (28*a);
		easyEnemies[a].enemyY = 0;
	} 
	easyEnemies[1].enemyX = 240;
	easyEnemies[4].enemyX = 240;
	easyEnemies[8].enemyX = 240;
	
	//difficult enemies setup	
	for (int a = 0; a < 9; a++) {
		hardEnemies[a].enemyX = (28*a);
		hardEnemies[a].enemyY = 160;
	} 
	hardEnemies[3].enemyX = 240;
	hardEnemies[6].enemyX = 240;
	
	//fast enemy "boss" setup	
	fast->fastX = 0;
	fast->fastY = 30;
	
	//player setup	
	player->playerX = 120;
	player->playerY = 136;
}

//Muestra menu principal
void titleScreen() {
	// REG_DISPCNT = MODE3 | BG2_ENABLE;
	//initalize title screen
	print_text_on_vga(10, 20, "GALAGA ");
	drawImage3(0, 0, 240, 160, titlescreen);
	while(1) {
		if (KEY_DOWN_NOW(BUTTON_START)) {
			break;
		}
	}	
	//start black screen for drawing
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 160; j++) {
			setPixel(i, j, BLACK);
		}
	}
}

//Pantalla al finalizar juego con BUTTON_END
void endScreen() {
	drawImage3(0, 0, 240, 160, endscreen);
	//start black screen for drawing
	/*for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 160; j++) {
			setPixel(i, j, BLACK);
		}
	}*/
}

//Manipula entrada del jugador por teclado
void handleInput(struct Players *player, int *shoots) {
	//player shots 
	if (KEY_DOWN_NOW(BUTTON_A)) {
		if (shoots[curr_shot] == 0) {
			//SOLO SE ESTABLECE SU POSICION HORIZONTAL
			shoots[curr_shot] = 136*240 + player->playerX+9; /* 24 widht player */
			
			curr_shot++;
			if (curr_shot >= N_SHOOTS)
				curr_shot = 0;
		};
	}
	//player movement input
	if (KEY_DOWN_NOW(BUTTON_LEFT) && (player->playerX > 0)) {
		player->playerX -= playerspeed;
	}
	if (KEY_DOWN_NOW(BUTTON_RIGHT) && (player->playerX <= 216)) {
		player->playerX += playerspeed;
	}
	if (KEY_DOWN_NOW(BUTTON_UP) && (player->playerY > 25)) {
		player->playerY -= playerspeed;
	}
	if (KEY_DOWN_NOW(BUTTON_DOWN) && (player->playerY <= 136)) {
		player->playerY += playerspeed;
	}
}

//Dibuja sprite del jugador
void drawPlayer(struct Players *player) {
    if (playerImmunityTimer > 0) { // Si el jugador esta en estado de inmunidad, parpadea
        playerImmunityTimer--;
        // Alternar entre visible e invisible
        if (playerImmunityTimer % 10 < 5) { // Visible en los primeros 5 ciclos de cada 10
            spritePlayer(player);
        } else { // Invisible en los últimos 5 ciclos de cada 10
			// Borra al jugador
            drawRect(player->playerX, player->playerY, 24, 24, BLACK);
			drawHollowRect(player->playerX - 1, player->playerY - 1, 26, 26, BLACK);
			drawHollowRect(player->playerX - 2, player->playerY - 2, 28, 28, BLACK);
        }
    } else {
        spritePlayer(player);
    }
}

void spritePlayer(struct Players *player) {
	//dimensiones total del player
	drawImage3(player->playerX, player->playerY, 24, 24, playerImage);
	//dibuja huecos donde no esta el sprite del player
	drawHollowRect(player->playerX - 1, player->playerY - 1, 26, 26, BLACK);
	drawHollowRect(player->playerX - 2, player->playerY - 2, 28, 28, BLACK);
}

void drawEnemies(struct Enemy easyEnemies[], struct Enemy hardEnemies[], struct FastEnemy *fast, struct Players *player) {
	//draw easy enemies with downward movement
	for (int a = 0; a < 9; a++) {
		easyEnemies[a].enemyY += enemyspeed;
		drawImage3(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, enemy);
		if (collision(easyEnemies[a].enemyX, easyEnemies[a].enemyY, 20, 20, player->playerX, player->playerY)) {
			//Reduce vida solo si no es inmune
			if (playerImmunityTimer == 0) {
				lives--; //Resta vida
				playerImmunityTimer = 30; //Da inmunidad por 30 frames
				//avisa a proceso 2 que se perdio una vida
				send(PID_SCORE, MSG_DAMAGE);
			}
		}
		if (easyEnemies[a].enemyY >= 160) {
			easyEnemies[a].enemyY = 0;
		}		
	}
/*	Logica de hard enemies y fast enemies, comentado para debugear luego
	//draw hard enemies
	for (int a = 0; a < 9; a++) {
		hardEnemies[a].enemyY += enemyspeed;
		drawImage3(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 20, 20, enemy);
		if (collision(hardEnemies[a].enemyX, hardEnemies[a].enemyY, 20, 20, player->playerX, player->playerY)) {
			endGame();
		}	
		if (hardEnemies[a].enemyY >= 228) {
			hardEnemies[a].enemyY = 0;
		}
		if ((hardEnemies[a].enemyY >= 200) && (easyEnemies[a].enemyY <=45)) {
			hardEnemies[a].enemyY = 160;
		}	
		//space enemies apart
		if ((hardEnemies[a].enemyY >= 200) && (easyEnemies[a].enemyY <=45)) {
			hardEnemies[a].enemyY = 160;
		}		
		if ((easyEnemies[a].enemyY >= 120) && (hardEnemies[a].enemyY >=170)) {
			hardEnemies[a].enemyY = 160;
		}							
	}	
	//draw fast enemy
	drawImage3(fast->fastX, fast->fastY, 15, 15, boss);
	drawHollowRect(fast->fastX - 1, fast->fastY - 1, 17, 17, BLACK);
	drawHollowRect(fast->fastX - 2, fast->fastY - 2, 19, 19, BLACK);
	if(collision(fast->fastX, fast->fastY, 15, 15, player->playerX, player->playerY)) {
		endGame();
	}		
//RAFA		fast.fastX += fastXSpeed;
//RAFA		fast.fastY += fastYSpeed;
	if (fast->fastX >= 240) {
		fast->fastX = 0;
	}
	if (fast->fastY >= 200) {
		fast->fastY = player->playerY - 20;
	}
*/
}

void drawShots(int *shoots, struct Enemy easyEnemies[]) {
	//draw shots
	for (int i = 0; i < N_SHOOTS; i++) {
		if (shoots[i] != 0) { //Establece su posicion inicial
			//Solo guarda su posicion en eje X
			drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
			drawImage3((shoots[i] % 240), (shoots[i] / 240), 5, 5, shoot);
			shoots[i] = shoots[i]-240*4;
			if (shoots[i] <=0) {
				shoots[i] = 0;
			}
		}

		// check hits of shoots
		for (int j = 0; j < 9; j++) {
			if (shoots[i] != 0) {//chequea que este activo
				int shotX = shoots[i] % 240; // position X (col)
				int shotY = shoots[i] / 240; // position Y (row)
				if (shoots[i] != 0 && collision(easyEnemies[j].enemyX, easyEnemies[j].enemyY, 15, 15, shotX, shotY)) {
					drawRect(easyEnemies[j].enemyX, easyEnemies[j].enemyY,  20, 20, BLACK);
					drawRect((shoots[i] % 240), (shoots[i] / 240)+4, 5, 5, BLACK);
					easyEnemies[j].enemyY = 0;
					shoots[i] = 0;
					score++;
					//avisa a proceso 2 que se obtuvo un punto
					send(PID_SCORE, MSG_SCORED);
					break; //Como colisiono, no lo chequea mas
				}
			}
		}
	}
}

int collision(u16 enemyX, u16 enemyY, u16 enemyWidth, u16 enemyHeight, u16 playerX, u16 playerY) {
	//check if bottom right corner of enemy is within player
	if (((enemyX + enemyWidth) > playerX) && ( (enemyY + enemyHeight) 
		> playerY ) &&  ((enemyX + enemyWidth) < (playerX + 24)) 
		&& ((enemyY + enemyWidth) < (playerY + 24))) {
		return 1;
	} 
	//check bottom left corner of enemy
	if ( (enemyX < (playerX + 24)) 
		&& (enemyX > playerX)
		&& ((enemyY + enemyHeight) > playerY)
		&& ((enemyY + enemyHeight) < (playerY + 24))
		) {
		return 1;
	}
	//check top left corner
	if ( (enemyX < (playerX + 24)) 
		&& (enemyX > playerX)
		&& (enemyY > playerY)
		&& (enemyY < (playerY + 24))
		) {
		return 1;
	}	
	//check top right corner
	if ( ((enemyX + enemyWidth) < (playerX + 24)) 
		&& ((enemyX + enemyWidth) > playerX)
		&& (enemyY > playerY)
		&& (enemyY < (playerY + 24))
		) {
		return 1;
	}	
	return 0;
}

void endGame() {
	//start Game Over State
	/* Detiene a GAME para que deje de imprimir
	 * frames de la partida finalizada*/
	kill(PID_GAME);
	drawImage3(0, 0, 240, 160, gameover);
	drawHollowRect(0, 0, 240, 160, WHITE);
	while(1) {
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			galaga();
			break;
		}
		if (KEY_DOWN_NOW(BUTTON_START))	{
			galaga();
			break;
		}
	}
}

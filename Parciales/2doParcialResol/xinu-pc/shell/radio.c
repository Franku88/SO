
#include<xinu.h>

const char * mensajes[] = {
" Mensaje de la primer radio \n",
" Segunda radio transmitiendo \n",
" Tercera emisora enviando \n",
};

int emisora(int n, int retardo, int pid_radio){

	int i = 0;
	int n2 = 0;
	while (mensajes[n-1][i] != 0) {
		n2++;
		i++;
	}

	int msg;
	while (1) {
		for (i=0; i<n2; i++) {
			msg = (n << 8) | mensajes[n-1][i];
			send(pid_radio, msg); // envia msg a proceso con pid_radio
			sleepms(retardo);
		}
	}
}

sid32 sem_print;
int sintonia;

int radio_play() {
	int msg;
	int n;
	char c;

	while(1) {
		msg = receive(); // espera msg de emisora
		
		n = (msg >> 8); // obtenemos el numero de emisora
		c = (char) (msg & 0xFF);

		if (sintonia == n) { // imprime solo si n tiene num de emisora
			wait(sem_print); // obtiene permiso de sem
			printf("%c", c);
			signal(sem_print); // desocupa permiso de sem
		}
	}
}

int radio_control() {

	control(CONSOLE, TC_MODER, 0, 0);

	sem_print = semcreate(1); //inicializa semaforo con un permiso

	sintonia = 1;
	int pid_radio = create(radio_play, 8192, 20, "play", 0);
	int pid_emi1 = create(emisora, 8129, 20, "emi1", 3, 1, 600, pid_radio);
	int pid_emi2 = create(emisora, 8129, 20, "emi2", 3, 2, 500, pid_play);
	int pid_emi3 = create(emisora, 8129, 20, "emi3", 3, 3, 888, pid_play);

	resume(pid_radio);
	resume(pid_emi1);
	resume(pid_emi2);
	resume(pid_emi3);

	char c; 
	while(1) {
		c = getchar();
		if (c == 'q') {
			kill(pid_radio);
			kill(pid_emi1);
			kill(pid_emi2);
			kill(pid_emi3);
			break;
		} else if (c == '1') {
			sintonia = 1;
			wait(sem_print);
			printf("\n\r La emisora es %d \n\r", sintonia);
			signal(sem_print);

		} else if (c == '2') {
			sintonia = 2;
			wait(sem_print);
			printf("\n\r La emisora es %d \n\r", sintonia);
			signal(sem_print);

		} else if (c == '3') {
			sintonia = 3;
			wait(sem_print);
			printf("\n\r La emisora es %d \n\r", sintonia);
			signal(sem_print);
		}

	}
	
	control(CONSOLE, TC_MODEC, 0, 0); // Envia sin presionar ENTER
}
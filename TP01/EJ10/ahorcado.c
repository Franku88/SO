#include <xinu.h>

/************************************/
/****** Benitez, Franco Fabian ******/
/******        FAI-3169       *******/
/************************************/

#define INTENTOS 5
#define CANTIDAD 5 

char *palabras[CANTIDAD] = {"java", "procedural", "pan", "sistema", "lenguaje"};
char *palabraElegida = NULL;

void seleccionarPalabra() {
	srand((int)getticks());  //ticks como seed
	int indice = rand() % CANTIDAD; // Obtener un índice aleatorio
	palabraElegida = palabras[indice];
}

int verificarLetra(char* progreso, char letra) {
	/*Busca letra en palabraElegida, si la encuentra, entonces lo establece en el progreso*/
	int flag = 0;
	int i = 0;
	char caract = palabraElegida[i];

	while(caract != '\0') {
    	if(caract == letra) {
        	progreso[i] = letra;
        	flag = 1;
    	}
    	i++;
    	caract = palabraElegida[i];
   	 
	}
	return flag;
}

int verificarPalabra(char* progreso) {
	/*Verifica si el progreso no tiene '_', es decir, encontro toda letra*/
	int i = 0;
	char caract = progreso[i];
	while (caract != '\0') {
    	if (caract == '_') {
        	return 0;
    	}
    	i++;
    	caract = progreso[i];
	}
	return 1;
}

void ahorcado() {
	/* Decirle al sistema que el modo input es RAW */
	control(CONSOLE, TC_MODER, 0, 0);
	
	int c, intentos;
	intentos = INTENTOS;
	seleccionarPalabra(); //Elije palabra aleatoria

	// Inicializa progreso (Solo espacios)
	int tam = strlen(palabraElegida);
	char progreso[tam+1];
	for (int i = 0; i < tam; i++) {
    	progreso[i] = '_';
	}
	progreso[tam] = '\0'; // Agregar el nulo al final

	while(1) {
    	printf("\r                                                      	");
    	printf("\r Palabra: [%s] Intentos: %d |  c = %c  Ingrese una letra (0 para salir): ", progreso, intentos, c);
    	c = getchar();

    	if (c == '0') { //Si es la ultima posicion, corta
			printf("\n");
        	break;
    	}

    	if (!verificarLetra(progreso, c)) { 
        	intentos--;
        	if (intentos < 1) {
            	printf("\n <<< PERDISTE >>>\n");
            	break;
        	}
    	} else { 
			if (verificarPalabra(progreso)) {
				printf("\r                                                      	              ");
    			printf("\r Palabra: [%s] | <<< GANASTE >>>                                        \n",progreso);
        		break;
			}
    	}
	}

	control(CONSOLE, TC_MODEC, 0, 0);
}

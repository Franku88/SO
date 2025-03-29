#include <stdio.h>
#include <stdlib.h> /* para las funciones system y exit */
#include <string.h> /* Para la función strlen() */
#include <time.h> /* Para la función rand() */

/************************************/
/****** Benitez, Franco Fabian ******/
/******        FAI-3169       *******/
/************************************/

#define INTENTOS 7
#define CANTIDAD 5 

const char *palabraElegida = NULL;
const char *palabras[CANTIDAD] = {"java", "procedural", "pan", "sistema", "lenguaje"};

void seleccionarPalabra() {
	srand(time(NULL));  //seed de num aleatorio
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
	while (caract != '\0'){
    	if (caract == '_'){
        	return 0;
    	}
    	i++;
    	caract = progreso[i];
	}
	return 1;
}

int main() {
	/* Decirle al sistema que el modo input es RAW */
	system ("/bin/stty raw");
	int c, intentos;
	intentos = INTENTOS;
	seleccionarPalabra(); //Elije palabra aleatoria

	// Inicializa progrso (Solo espacios)
	int tam = strlen(palabraElegida);
	char progreso[tam+1];
	for (int i = 0; i < tam; i++) {
    	progreso[i] = '_';
	}
	progreso[tam] = '\0'; // Agregar el nulo al final

	while(1) {
    	printf("\r                                                      	");
    	printf("\r Palabra: [%s] Intentos: %i |  c = %c  Ingrese una letra (0 para salir): ", progreso, intentos, c);
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

	system ("/bin/stty sane erase ^H");
}
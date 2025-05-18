/* Ejercicio 2.
Portar el programa del Ejercicio 1. a XINU, con la siguiente modificación: 
el programa principal debe crear dos procesos: procA y procB. 
El proceso procA pide la entrada al usuario. 
El segundo proceso procB computa el posible palíndromo y muestra el resultado. 
El programa principal deberá esperar a que procA finalice antes de poner a ejecutar a procB.
Integre el programa al shell de XINU y verifique su funcionamiento.
NOTA1: el system call receive() sin argumentos puede ser utilizado en XINU para esperar a que
un proceso hijo finalice (receive() bloquea al proceso hasta que un proceso hijo finalice).​
NOTA2: el arreglo donde quedará la entrada del usuario debe ser global para que todos los 
procesos puedan accederlo (lo mismo para otras variables compartidas).

IMPORTANTE: Para la resolución utilice UNICAMENTE el siguiente codigo fuente de XINU:
https://se.fi.uncoma.edu.ar/so/misc/xinu-pc.tar.gz
(SI REUTILIZA el código fuente de XINU que usó durante los trabajos prácticos SE ANULA LA 
ENTREGA). Se entrega todo el código fuente de XINU, no sólo el fuente .c. 
Antes de crear un zip o tar.gz de la carpeta xinu-pc realice un make clean en compile*/

#include <xinu.h>

/************************************/
/****** Benitez, Franco Fabian ******/
/******        FAI-3169       *******/
/************************************/

char palabra[128];
int n = 0;

void procA(void), procB(void);

int detectarPalindromo(char palabra[], int cant) {
	/*Params: 
	 * palabra - autoexplicativa
	 * cant - cantidad de caracteres que tiene palabra*/

	int flag = 1; // Retorno
	int i = 0; // Pivote izquierdo
	int d = cant-1; // Pivote derecho
	int comp = cant/2; /*cantidad de comparaciones*/
	
	while(1) {
		if (comp != 0) { /*comparaciones restantes*/
			if (palabra[i] == palabra[d]) { /*Si son iguales, sigue comparando*/
				i++;
				d--;
				comp--;
			} else { /* Si son diferentes, entonces no es capicua*/
				flag = 0;
				break;
			}
		} else { /*no restan comparaciones*/
			break;
		}
	}
	return flag;
}

void ej2() {
	int idA, idB;
	idA = create(procA, 1024, 10, "proceso A", 0);
	idB = create(procB, 1024, 5, "proceso B", 0);
	resume(idA);
	receive();
	resume(idB);
}

void procA() {
	int c; /* caracter ingresado */
	int i; /* cant letras*/
	printf("--- BIENVENIDA/O AL DETECTOR DE PALINDROMOS ---\n");
	printf("Ingrese una palabra/numero y presione ENTER: ");

	/* Decirle al sistema que el modo input es RAW */
	control(CONSOLE, TC_MODER, 0, 0);
	i = 0;
	while (c != 13) {
		c = getchar();
		if (c == 13) {
			palabra[i] = '\0'; /* fin de linea */
			break;
		}

		palabra[i] = c;
		i++;
	}
	n = i;
	control(CONSOLE, TC_MODEC, 0, 0);
	printf("\n");
}

void procB() {
	/* Reutilizo i como flag*/
	n = detectarPalindromo(palabra, n);
	if (n) {
		printf("La palabra/numero '%s' ES palindromo: \n\r", palabra);
	} else {
		printf("La palabra/numero '%s' NO ES palindromo: \n\r", palabra);
	}
	putc(CONSOLE, '\n');
	printf("--- FIN DETECTOR DE PALINDROMOS ---\n");
}

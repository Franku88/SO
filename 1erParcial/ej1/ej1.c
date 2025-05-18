/* Ejercicio 1.
Desarrollar un programa en Linux que indique si una palabra o número ingresado por el usuario es
palíndromo. Una palabra o frase es palíndromo si se lee igual en un sentido que en el otro​ (por ejemplo;
ana, anna, otto, neuquen son palíndromos). Si se trata de números en lugar de letras, se llaman capicúas
(121, y 3333 son capicúas).
Específicamente, el programa debe ir solicitando pulsaciones de teclado al usuario, y colocando la
entrada del usuario en un arreglo. Hasta que el usuario presione enter (el ENTER tiene código 13 en base
10).
Luego, se debe analizar el arreglo para ver si la entrada del usuario es palíndromo.
Al finalizar el análisis el programa debe mostrar la entrada del usuario y una leyenda que indique si es
palíndromo o no.*/

#include <stdio.h>
#include <stdlib.h>	/* para la funcion system */

/************************************/
/****** Benitez, Franco Fabian ******/
/******        FAI-3169       *******/
/************************************/

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

void main() {
	int c; /* caracter ingresado */
	int i; /* cant letras*/
	
	char palabra[128];
	
	printf("--- BIENVENIDA/O AL DETECTOR DE PALINDROMOS ---\n");
	printf("Ingrese una palabra/numero y presione ENTER: ");
	
	/* Decirle al sistema que el modo input es RAW */
	system("/bin/stty raw");
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
	system("/bin/stty sane erase ^H");
	printf("\n");
		
	/* Reutilizo i como flag*/
	i = detectarPalindromo(palabra, i);
	if (i) {
		printf("La palabra/numero '%s' ES palindromo: \n\r", palabra);
	} else {
		printf("La palabra/numero '%s' NO ES palindromo: \n\r", palabra);
	}
	
	printf("--- FIN DETECTOR DE PALINDROMOS ---\n");
}

#include <xinu.h>
#include <mutex.h>
#include "imagen1.h"
#include "imagen2.h"

/* La diferencia principal con el ejercicio del parcial es que no es 
necesario solcilitar, con un system call, memoria compartida. Indique por 
qué con un comentario al principio del código fuente de su resolución: */

/* Esta diferencia ocurre dado a que los bytes de la imagen fusionada ahora son 
utilizados dinámicamente en la misma ejecución de un solo programa, es decir, los procesos
que forman la imagen en un array y el proceso que luego la muestra en pantalla, pueden
acceder dinámicamente al array fusion sin necesidad de memoria compartida. 
En cambio, en el del primer parcial, primero un programa formaba la imagen, y luego otro 
programa lo guardaba en un archivo, necesitando asi un segmento de memoria compartida
para acceder a la imagen formada. */

/* IMPORTANTE: Para no perder mensajes, es apropiado darle, a los programas impares
y pares menos prioridad que el programa principal (el programa principal tiene prioridad 20).
Explique por qué haciendo simplemente esto no se pierden mensajes: */

/* Si los programas pares e impares tienen más prioridad que el main, entonces estos
podrían finalizar su ejecución, sin que el main pueda ejecutar uno de los receive().
Por ejemplo, el main crea los dos procesos y se pone en espera con el primer receive(), 
permitiendo que sus hijos inicien. En el momento que uno de los dos procesos termine su ejecución
enviando un send al main, sucederá que, al tener más prioridad que el main, el siguiente hijo retome
ejecución sin que el main ejecute el segundo receive(), por lo que el segundo send() no será recibido 
por el main.
Al darle más prioridad al main, se asegura que el mismo ejecute receive() antes de que cualquiera de
los procesos hijos realice un send().*/

#define B_IMAGE 270000
#define END_CODE 16532
unsigned char fusion[B_IMAGE];
struct mutex fusionMutex; // Mutex

extern void pixel(unsigned x, unsigned y, uint32 color);
void mostrar(unsigned char * imagen) {
	int *c;
	int n = 0;
	for (int y = 0; y < 300; y++) {
		for (int x = 0; x < 300; x++) {
			c = (int *) &imagen[n];
			pixel(x, y, *c);
			n = n + 3;
		}
	}
}

int impares(pid32 PID_F) {
	int i = 1;
	while (i < B_IMAGE) {
		mutex_lock(&fusionMutex); //ocupo mutex
		fusion[i] = pixels1[i];
		mutex_unlock(&fusionMutex); //desocupo mutex

		i = i + 2;
	}
	send(PID_F, END_CODE);

	return 0;
}

int pares(pid32 PID_F) {
	int i = 0;
	while (i < B_IMAGE) {
		mutex_lock(&fusionMutex); //ocupo mutex
		fusion[i] = pixels2[i];
		mutex_unlock(&fusionMutex); //desocupo mutex

		i = i + 2;
	}
	send(PID_F, END_CODE);

	return 0;
}

int fusiona() {
	mutex_init(&fusionMutex); //inicializo mutex

	resume(create(impares, 1024, 19, "pixels impares", 1, getpid()));
	resume(create(pares, 1024, 19, "pixels pares", 1, getpid()));

	printf("ESPERANDO PRIMER PROCESO.\n");
	if (receive()) {
		printf("PRIMER PROCESO HA TERMINADO.\n");
	}

	printf("ESPERANDO SEGUNDO PROCESO.\n");
	if (receive()) {
		printf("SEGUNDO PROCESO HA TERMINADO.\n");
	}

	printf("CTRL+ALT+1 PARA VER IMAGEN FUSIONADA.\n");
	mostrar(fusion);

	return 0;
}
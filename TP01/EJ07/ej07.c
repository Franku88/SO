/* Creación de proceso en UNIX (LINUX, MAC OS)*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int esPrimo(int numero){
	int esPrimo = 1;
	int i = 2;
    
	if (numero <= 1)
    	return 0;
    
	// Numero de divisores que puede tener un numero es el resultado de la raiz cuadrada del numero
	while (esPrimo && (i * i) <= numero){
    	if ((numero % i) == 0){
        	esPrimo = 0;
    	} else {
        	i = i + 1;
    	}
	}
	return esPrimo;
}

void encontrarPrimosPadre() {
	for (int i = 0; i <=1000; i++) {
    	if (esPrimo(i))
        	printf("- %d \n", i);  
	}
}

void encontrarPrimosHijo() {
	for (int i = 1000; i <= 5000; i++) {
    	if (esPrimo(i))
        	printf("+ %d \n", i);    
	}
}

void main(void) {
	int pid = fork(); //Se duplica el proceso

	if (pid == 0) {  // Proceso hijo
    	printf(" Empezo  el hijo.\n");
    	encontrarPrimosHijo();
    	printf(" Proceso hijo finalizado.\n");
    	exit(0);   
	} else {  // Proceso padre
    	printf(" Proceso padre iniciado.\n");
    	encontrarPrimosPadre();
    	printf(" Proceso padre finalizado\n");
    	kill(pid, SIGKILL);
    	printf(" Elimino el hijo.\n");
    	exit(0);
	}
}

/* Otras funciones de la biblioteca de C (que realizan llamadas al sistema)
 * wait()
 * exit()
 * execv()
 * getpid()
 * */

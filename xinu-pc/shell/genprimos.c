
#include <xinu.h>

void genprimosinterval(int, int);

void genprimos() {
	int gen1, gen2;
	gen1 = create(genprimosinterval, 1024, 6, "genprimos 1", 2, 1, 5000);
	gen2 = create(genprimosinterval, 1024, 5, "genprimos 2", 2, 5000, 10000);
	resume(gen1);
	resume(gen2);
	putc(CONSOLE, '\n');
}

void genprimosinterval(int min, int max) {
	/* Imprime en consola numeros primos desde min hasta max*/
	int i;
	int j;
	int cantp = 0;
	
	// Establece contador, verifica que minimo sea positivo diferente a 1
	if (min < 2) {
		i = 2;
	} else {
		i = min;
	}
	 
	while (1) {
		j = 2; //divisor de i
		while (1) {
			if (j*j <= i) { // Si j <= sqrt(i), entonces j es candidato a divisor
				if (i%j == 0) { //Si es candidato y ademas es divisor
					break; //Corta while test con i
				}
			} else { // Si j > sqrt(i), entonces no es candidato (se llego al ultimo)
				printf("%d ", i); //Lo imprime y corta while
				cantp++;
				break;
			}
			j++;
		}
		i++;
	
		if (min == 1 && cantp % 100 == 0) {
			sleepms(10);
		}
		if (i>max) { //Si pasamos el limite, termina de imprimir
			break;
		}
	}
	
	
}


// Test de mutex, ubicar en /xinu-pc/shell
// Definir en cmdtab.c y shprototypes.h  
/* mut.c - mut, operar, incrementar */

# include <xinu.h>
# include <mutex.h>

void operar(void), incrementar(void);

unsigned char x = 0;
struct mutex unMutex; // Mutex

/* ---------------------------------------
 * mut -- programa con regiones criticas
 * ---------------------------------------
 * */

void mut(void) {
	mutex_init(&unMutex); // Inicializo mutex
	
	resume(create(operar, 1024, 20, "process 1", 0));
	resume(create(incrementar, 1024, 20, "process 2", 0));
	
	sleep(10);
}

/*----------------------------------------
 * operar x e y
 * ---------------------------------------
 * */
 void operar(void) {
	int y = 0;
	printf("Si no existen mensajes de ERROR entonces todo va OK! \n");
	
	while(1) {
		// Adquiere mutex antes de acceder a recurso compartido
		mutex_lock(&unMutex);
		
		/* Si x es multiplo de 10 */
		if ((x % 10) == 0) {
			/* Como y es el doble de x entonces y es multiplo de 10 tambien*/
			y = x * 2;
			
			/* Si y no es multiplo de 10 entonces hubo un error */
			if ((y % 10) != 0) {
				printf("\r ERROR!! y=%d, x=%d \r", y, x);
			}
		}
		
		// Libera mutex despues de desocupar recurso compartido
		mutex_unlock(&unMutex);
	}
}

/*----------------------------------------
 * incrementar x
 * ---------------------------------------
 * */
 void incrementar(void) {
	while(1) {
		// Adquiere mutex antes de acceder a recurso compartido
		mutex_lock(&unMutex);
		
		x = x + 1;
		
		// Libera mutex despues de desocupar recurso compartido
		mutex_unlock(&unMutex);
	}
}

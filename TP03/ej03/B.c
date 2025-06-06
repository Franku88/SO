/* gcc -o B B.c -lrt	// Compila B.c sobre librt
 * ls /dev/shm 			// Ver objetos de memcompartida
 * */
#include <stdio.h>
#include <sys/mman.h> 	/* Funciones y const de memcompartida*/
#include <unistd.h> 	/* ftruncate */
#include <fcntl.h> 		/* constantes O_* */
#include <errno.h> 		/* obtener información sobre errores */

const int SIZE = 4096;
const char *name = "MemComp88";

int shm_fd;
void *ptr;

int main() {
	/* Abre segmento de memcompartida, readonly 
	 * Explicación detallada de los params en A.c
	 * */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	
	/* Mapea segmento de memcompartida en el espacio de direccion del proceso 
	* Explicación detallada de los params en A.c
	* */
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);	
	if (ptr == MAP_FAILED) {
		printf("Mapeo fallido: %d.\n", errno);
		return -1;
	}
	
	// Muestra desde el segmento de memcompartida
	printf("Contenido de la memoria compartida: \n%s\n", (char *)ptr);
	
	/* Borramos nombre de segmento de memcompartida,
	es decir, ningun otro proceso podrá acceder al mismo
	desde que se ejecute shm_unlink */
	if (shm_unlink(name) == -1) {
		printf("Error al borrar %s.\n", name);
		return -1;
	} else {
		printf("%s borrada con éxito.\n", name);
	}
	return 0;
}	

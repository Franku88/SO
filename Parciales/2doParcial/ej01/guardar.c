#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h> 	/* Funciones y const de memcompartida*/
#include <unistd.h> 	/* Para ftruncate */
#include <fcntl.h> 		/* Para constantes O_* */
#include <errno.h> 		/* Para info sobre errores*/

#define BSIZE_H 15 //Bytes de cabecera
#define	BSIZE_I 270000 //Bytes de imagen (pixeles)

const char *name = "ffbFusion"; // Nombre ultrasecreto

/* memcomp = "memoria compartida"
 * shm_open() crea y abre un nuevo objeto de memcomp POSIX, 
 * o abre uno existente. Un objeto de memcomp POSIX es un identificador
 * que puede ser utilizado por procesos no relacionados para mapear la
 * misma region de memcomp.
 * La función shm_unlink() realiza la operación opuesta, eliminando un 
 * objeto previamente creado por shm_open() 
 * */
int shm_fd;
void *ptr;

int main () {
    int BSIZE_F = BSIZE_H+BSIZE_I;

    shm_fd = shm_open(name, O_RDONLY, 0666);
    /* Mapea segmento de memcompartida en el espacio de direccion del proceso*/
	ptr = mmap(0, BSIZE_F, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Mapeo fallido: %d.\n", errno);
		return -1;
	}

    int f = open("DUCKDUCKFAI.ppm", O_RDWR|O_CREAT, 0666);
    write(f, ptr, BSIZE_F);
    close(f);

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
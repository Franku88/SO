#include "imagen1.h"
#include "imagen2.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h> // Funciones y const de memcompartida
#include <unistd.h> // Para ftruncate
#include <fcntl.h> // Para constantes O_*
#include <errno.h> // Para info sobre errores
#include <string.h> // Para memcpy

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

int main() {
	// en image1.h e imagen2.h están los arreglos de las imágenes
	// los arreglos son de tipo: unsigned char
	
	// cabecera1[] tiene 15 bytes que son la cabecera de la imagen
	
	// pixels1[4]  tiene el pixel 4 de la imagen 1
	// pixels1[5]  tiene el pixel 5 de la imagen 1
	
	// pixels2[10]  tiene el pixel 10 de la imagen 2

	int BSIZE_F = BSIZE_H+BSIZE_I; // Bytes del archivo (total)
	unsigned char fusion[BSIZE_F]; // Nuevo array
	int i; // Indice para recorrer imagenes
	char byte; // Byte a copiar

	// Copio cabecera (al ser iguales es indiferente cual copio)
	for (i = 0; i < BSIZE_H; i++) {
		fusion[i] = cabecera1[i]; //fusion[i] = cabecera1[i];
	}
	// Copio pixeles de imagen (segun lo indicado)
	for (i = 0; i < BSIZE_I; i++) {
		//Impares para img1 (impar ya que seria 1er pixel == i igual a 0)
		if (i % 2 == 0) { 
			byte = pixels1[i];
		} else { //Impares para img2
			byte = pixels2[i];
		}
		//fusion[BSIZE_H+i] = byte; ya que toma en cuenta la cabecera
		fusion[BSIZE_H+i] = byte;
	}
	
	/* Crear objeto de memoria compartida POSIX.
	 * name es nombre del objeto de memcomp. Es unico en el msistema y
	 * se utiliza para identificar el objeto de memcomp.
	 * O_CREAT | O_RDWR: flags de apertura que se pasan a shm_open.
	 * O_CREAT: indica que se debe crear el objeto si no existe.
	 * O_RDWR: indica que el objeto se abrirá para lectura y escritura.
	 * 0666: modo o permisos que se aplicarán al objeto de memcomp,
	 * en este caso, permite lectura y escritura para todos los usuarios.
	 * shm_open, ftruncate, mmap 
	* */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	 
	/* Asigna el segmento de memoria compartida en el espacio de 
	 * direcciones del proceso 
	* */
	ftruncate(shm_fd, BSIZE_I);
	 
	/* Mapea segmento de memcompartida en el espacio de direccion del proceso 
	 * 0: Especifica que la dirección de inicio de la asignación se 
	 * elige automáticamente por el sistema operativo.
	 * BSIZE_F: Es el tamaño de la región de memoria que se va a asignar.
	 * PROT_READ | PROT_WRITE: Indica que la región de memoria asignada
	 * se puede leer y escribir.
	 * MAP_SHARED: Indica que la memoria asignada es compartida entre
	 * múltiples procesos y cualquier cambio realizado en esta memoria
	 * será visible para todos los procesos que la compartan.
	 * shm_fd: descriptor de archivo de la memcomp que se está asignando.
	 * 0: Desplazamiento dentro del archivo de memcomp,
	 * en este caso, se coloca al principio del archivo.
	* */	 
	ptr = mmap(0, BSIZE_F, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Mapeo fallido: %d.\n", errno);
		return -1;
	}
	// Luego copio array generado a shm_fd con memcpy
	memcpy(ptr, fusion, BSIZE_F);

	printf("IMAGENES FUSIONADAS CON EXITO. \n");
	return 0;
}

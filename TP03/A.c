/* gcc -o A A.C -lrt	// Compila A.c sobre librt
 * cat /usr/share/doc/aufs-dkms/filesystems/aufs/design/06mmap.txt 	// Imprime en terminal el txt
 * ls /dev/shm  		// Ver objetos de memcompartida
 * */
#include <stdio.h>
#include <sys/mman.h> 	/* Funciones y const de memcompartida*/
#include <unistd.h> 	/* Para ftruncate */
#include <fcntl.h> 		/* Para constantes O_* */
#include <errno.h> 		/* Para info sobre errores*/

const int SIZE = 4096;
const char *name = "MemComp88";
const char *pathName = "/usr/share/doc/aufs-dkms/filesystems/aufs/design/06mmap.txt";

/* memcomp = "memoria compartida"
 * shm_open() crea y abre un nuevo objeto de memcomp POSIX, 
 * o abre uno existente. Un objeto de memcomp POSIX es un identificador
 * que puede ser utilizado por procesos no relacionados para mapear la
 * misma region de memcomp.
 * La función shm_unlink() realiza la operación opuesta, eliminando un 
 * objeto previamente creado por shm_open() 
 * */
int shm_fd, fileDescriptor;
void *ptr;

int main() {
	int buffSize = 1024;
	char buff[buffSize];
	
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
	 ftruncate(shm_fd, SIZE);
	 
	 /* Mapea segmento de memcompartida en el espacio de direccion del proceso 
	  * 0: Especifica que la dirección de inicio de la asignación se 
	  * elige automáticamente por el sistema operativo.
	  * SIZE: ES el tamaño de la región de memoria que se va a asignar.
	  * PROT_READ | PROT_WRITE: Indica que la región de memoria asignada
	  * se puede leer y escribir.
	  * MAP_SHARED: Indica que la memoria asignada es compartida entre
	  * múltiples procesos y cualquier cambio realizado en esta memoria
	  * será visible para todos los procesos que la compartan.
	  * shm_fd: descriptor de archivo de la memcomp que se está asignando.
	  * 0: Desplazamiento dentro del archivo de memcomp,
	  * en este caso, se coloca al principio del archivo.
	  * */	 
	 ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	 if (ptr == MAP_FAILED) {
		printf("Mapeo fallido: %d.\n", errno);
		return -1;
	 }
	 
	 // Apertura de archivo solo lectura
	 fileDescriptor = open(pathName, O_RDONLY);
	 // Copia contenido de fileDescriptor (pathName) en buffer
	 read(fileDescriptor, buff, buffSize);
	 // Copia contenido de buff en el segmento de memcomp
	 sprintf(ptr, "%s", buff);
	 // Clausura de archivo
	 if (close(fileDescriptor) == -1) {
		printf("Error al cerrar %s.\n", name);
		return -1;
	 } else {
		printf("%s cerrada con éxito.\n", name);
	 }
	 return 0;
}

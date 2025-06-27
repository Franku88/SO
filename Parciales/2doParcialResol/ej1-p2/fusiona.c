#include "imagen1.h"
#include "imagen2.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <string.h>

int main()
{
	// en image1.h e imagen2.h están los arreglos de las imágenes
	// los arreglos son de tipo: unsigned char
	// fusionamos
	unsigned char  fus[270015];

	int i;

	for (i=0; i<15; i++)
		fus[i] = cabecera1[i];

	int j = 0;
	for (i=0; i<270000; i++) {
		fus[15+j] = pixels1[i];
		j++;
		i++;
		fus[15+j] = pixels2[i];
		j++;
	}

	const int SIZE = 270015;
	const char *name = "OS";
	const char *mess0= "Studying ";
	const char *mess1= "Operating Systems ";
	int shm_fd;
	void *ptr;
	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);
	/* now map the shared memory segment in the
	 * address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	memcpy(ptr,  fus,  270015);

	return 0;
	
	// cabecera1[] tiene 15 bytes que son la cabecera de la imagen
	
	// pixels1[4]  tiene el pixel 4 de la imagen 1
	// pixels1[5]  tiene el pixel 5 de la imagen 1
	
	// pixels2[10]  tiene el pixel 10 de la imagen 2
}

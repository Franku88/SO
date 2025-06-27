
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <string.h>

int main()
{
	const char *name = "OS";
	const int SIZE = 270015;
	int shm_fd;
	void *ptr;
	int i;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		return -1;
	}
	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	int f = open("archivo.ppm", O_RDWR|O_CREAT, 0666);
	write(f, ptr, 270015);
	close(f);

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		return -1;
	}
}


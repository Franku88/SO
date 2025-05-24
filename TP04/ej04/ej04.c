#include <stdio.h> //Entrada/salida: printf(), perror(), putchar()...
#include <stdlib.h> //Manejo de memoria dinámica y salir del programa: malloc(), free(), exit()...
#include <fcntl.h> //Constantes, macros y funciones para apertura, creación y manipulación de descriptores de archivo: O_RDONLY...
#include <unistd.h> //Define funciones POSIX: read(), open(), close().

//Obtains file size by reading each byte
int file_get_size(const char nombre_de_archivo[]) {
    //Obtains file descriptor from the especified file route
    int fd = open(nombre_de_archivo, O_RDONLY);
    if (fd < 0) {
        perror("Error al abrir el archivo.");
        exit(1);
    }

    //Counts each byte until find none (read(...) == 0)
    char c;
    int size = 0;
    while(read(fd, &c, 1) == 1) {
        size++;
    }
    close(fd);

    return size;
}

//Main
int main() {
    //File route and size values
    const char *nombre_archivo = "/usr/share/doc/libsdl1.2-dev/docs.html";
    int size = file_get_size(nombre_archivo);

    //Obtains file descriptor of the especified route file
    int fd = open(nombre_archivo, O_RDONLY);
    if (fd < 0) {
        perror("Error al abrir el archivo.");
        exit(1);
    }

    //Reserves memory to save the content
    char *contenido = malloc(size);
    if (contenido == NULL) {
        printf("ERROR malloc.\n");
        close(fd);
        exit(1);
    }

    //Reads each byte and allocates them in "contenido"
    int leidos = 0;
    while(leidos < size) {
        int r = read(fd, &contenido[leidos], 1);
        if (r < 1) {
            printf("Error leyendo el archivo.\n");
            free(contenido);
            close(fd);
            exit(1);
        }
        leidos++;
    }
    close(fd);
    
    printf("------ INICIO DE ARCHIVO INVERTIDO ------");
    //Prints reversed content
    for (int i = size -1; i >= 0; i--) {
        putchar(contenido[i]);
    }
    putchar('\n');
    free(contenido);

    printf("------ FIN DE ARCHIVO INVERTIDO ------\n");
    return 0;
}

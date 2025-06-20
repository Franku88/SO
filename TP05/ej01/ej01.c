/* programa usamem.c */
#include <stdio.h>
#include <stdlib.h>

#define N 240000 //Cantidad de "filas" de bytes (4KiB)
#define BSIZE 4096 //Cantidad de "columnas" (bytes), 4KiB por fila
#define SMALL 4

char *pp; //puntero al primer byte (char)

int main() {
    int i, j, k;
    printf("la dir. virtual de main es 0x%llX \n", main);

    pp = malloc(N*BSIZE); //Tamanio de pp
    printf("la dir. virtual de pp es 0x%llX \n", &pp);
    printf("la dir. virtual almacenada en pp es 0x%llX \n", pp);
    printf("la dir. virtual de i es 0x%llX \n", &i);
    printf("la dir. virtual de j es 0x%llX \n", &j);
    printf("la dir. virtual de k es 0x%llX \n", &k);

    if (pp == NULL) {
        printf("Error al reservar memoria.\n");
        exit(1);
    }

    /* RECORREMOS y modificamos todo el segmento solicitado */
    for (j = 0; j < BSIZE; j++) {
        for (i = 0; i < N; i++) {
            *(pp+i*BSIZE+j) = 2; //pp[i][j] = 2;
        }
    }

    /* RECORREMOS y VERIFICAMOS QUE HEMOS MODIFICADO todo el segmento solicitado */
    for (i = 0; i<N; i++) {
        for(j = 0; j<BSIZE; j++) {
            if (*(pp+i*BSIZE+j) != 2) { //if (pp[i][j] != 2)
                printf("ERROR!\n");
                exit(1);
            }
        }
    }

    printf("OK.\n");
    return(0);
}

/* 
   a.
    El programa solicita al SO mediante malloc 
    N*BSIZE = 240000*4096 bytes 
            = 983040000 bytes 
            = 960000KB 
            = 937,5MB

   b.
    Resultado de comando time
    real    0m46,560s
    user    0m45,952s
    sys     0m0,548s

   c.
    la dir. virtual de pp es 0x6B4D58 
    la dir. virtual de i es 0x7FFDB816ED6C 
    la dir. virtual de j es 0x7FFDB816ED68 
    la dir. virtual de k es 0x7FFDB816ED64 
    OK.

    real    0m44,194s
    user    0m43,408s
    sys     0m0,732s

   d.
    la dir. virtual almacenada en pp es 0x7FF288967010 

   e.
    la dir. virtual de main es 0x400AAE
    la dir. virtual de i es 0x7FFDB816ED6C 
    la dir. virtual de j es 0x7FFDB816ED68 
   
   f. Salida de cat /proc/PID/maps
    00400000-004b2000 r-xp 00000000 00:29 17193883705       /export/home/.../ej01/ej01
    006b1000-006b4000 rw-p 000b1000 00:29 17193883705       /export/home/.../ej01/ej01
    006b4000-006b6000 rw-p 00000000 00:00 0 
    00903000-00926000 rw-p 00000000 00:00 0                 [heap]
    7f2646f98000-7f2681919000 rw-p 00000000 00:00 0 
    7fff9d1de000-7fff9d1ff000 rw-p 00000000 00:00 0         [stack]
    7fff9d201000-7fff9d203000 r--p 00000000 00:00 0         [vvar]
    7fff9d203000-7fff9d205000 r-xp 00000000 00:00 0         [vdso]
    ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0 [vsyscall]
*/
#include <stdio.h>
#include <string.h>

int main() {
    char palabra[] = "holaaaa";
    int longitud = strlen(palabra);

    printf("%s\n", palabra);
    printf("%d\n", longitud);
    printf("%x\n", palabra[0]);
    printf("%x\n", palabra[1]);
    printf("%x\n", palabra[2]);
    printf("%x\n", palabra[3]);
    printf("%x\n", palabra[4]);
    printf("%x\n", palabra[5]);
    printf("%x\n", palabra[6]);
    printf("%x\n", palabra[7]);

    printf("%x\n", palabra[8]); /*longitud string+2 almacena longitud de string?*/

    printf("%x\n", palabra[9]);
    printf("%x\n", palabra[10]);
}
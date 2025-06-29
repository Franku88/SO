#include <xinu.h>
#include <keyboard.h>

void testkbd() {
    int dev = open(KEYBOARD, NULL, NULL);
    //kprintf("Resultado de open: %d\n", dev);
    if (dev == SYSERR) {
        kprintf("No se pudo abrir el teclado.\n");
        return;
    }

    kprintf("CTRL+ALT+1 para ver que detecta tu teclado.\n");
    kprintf("ESC en pantalla amarilla para terminar.\n");

    char c;
    char t[80];
    while(TRUE) {
        read(dev, &c, 1);
        if(c == 0x01) {
            sprintf(t, "                      ");
            print_text_on_vga(10, 300, t);
            break;
        } else {
            sprintf(t, "kbd: 0x%x     ", (unsigned char) c);
            print_text_on_vga(10, 300, t);
        }
        
    }

    kprintf("Fin del test de teclado.\n");
    close(dev);
}
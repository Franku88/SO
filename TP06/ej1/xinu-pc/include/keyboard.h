#ifndef _KEYBOARD_H_ //if para evitar errores de doble inclusión
#define _KEYBOARD_H_

devcall kbdgetc(struct dentry *devptr);

#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)

#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write)

#define KBD_BUFF_SIZE 10

extern unsigned char kblayout [128];  // { ... } Fill your layout yourself 


// Buffer
extern unsigned char kbd_buffer[KBD_BUFF_SIZE];
extern int kbd_head, kbd_tail, kbd_count;

// Semaphores 
extern sid32 kbd_sem_filled; //cant elementos disponibles
extern sid32 kbd_sem_mutex; //acceso al buffer

// Control de acceso
extern bool8 kbd_opened; // TRUE si un proceso tiene abierto el teclado
extern pid32 kbd_owner;  //PID del proceso que hizo open()

#endif /* _KEYBOARD_H_ */
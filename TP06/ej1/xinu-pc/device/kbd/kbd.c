#include <xinu.h>
#include <keyboard.h>

unsigned char kbd_buffer[KBD_BUFF_SIZE];
int kbd_head = 0;
int kbd_tail = 0;
int kbd_count = 0;

sid32 kbd_sem_filled;
sid32 kbd_sem_mutex;

bool8 kbd_opened = FALSE;
pid32 kbd_owner = -1;

//previamente definido en kbdinit.c, por orden, lo defini junto con las desmas variables globales
unsigned char kblayout [128];  // { ... } Fill your layout yourself 


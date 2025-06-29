/* kbdgetc.c  -  kbdgetc */

#include <xinu.h>
#include <keyboard.h>

/*------------------------------------------------------------------------
 * kbdgetc  -  Obtiene char
 *------------------------------------------------------------------------
 */
devcall kbdgetc (
      struct dentry *devptr
    )
{
    if (!kbd_opened || kbd_owner != getpid()) {		
        return SYSERR;
    }

    //kprintf("\r[ESPERA ELEMENTO] \n ");
    wait(kbd_sem_filled); //espera que haya al menos un elemento en el buffer
    //kprintf("\r[TIENE ELEMENTO] \n ");
    wait(kbd_sem_mutex); //espera a que el kbd esté desocupado
    //kprintf("\r[TOMO PERMISO] \n ");

    unsigned char scancode = kbd_buffer[kbd_head];
    kbd_head =  (kbd_head + 1) % KBD_BUFF_SIZE;
    kbd_count--;
	//kprintf("\r[GETC: 0x%02X] \n", scancode);

    signal(kbd_sem_mutex);
    
    return scancode;
}
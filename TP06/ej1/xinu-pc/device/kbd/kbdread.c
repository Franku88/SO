/* kbdread.c  -  kbdread */

#include <xinu.h>
//#include <mouse.h>
#include <keyboard.h>

/*------------------------------------------------------------------------
 * kbdread  -  Read the status of the keyboard driver
 *------------------------------------------------------------------------
 */
devcall	kbdread (
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char          *buffer,        /* Address of buffer            */
          uint32        count           /* Length of buffer             */
	)
{
	//kprintf("\r[BUFF: %d] \n ", kbd_count);
	if (!kbd_opened || kbd_owner != getpid()) {
		return SYSERR;
	}

	
	if (count == 0) {
		return 0;
	}

	/*
	if (kbd_count == 0) {
		kprintf("\r[BUFF VACIO] \n ");
	} 
	*/
	buffer[0] = (char) kbdgetc(devptr); // uso kbdgetc
	
	return 1;

	/* Bloque original
	memcpy(buffer, &mouse, sizeof(mouse));

	//mouse.buttons = 0;
	//mouse.x = 0;
	//mouse.y = 0;
	return OK;*/
}

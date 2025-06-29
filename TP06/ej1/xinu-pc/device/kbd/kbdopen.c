/* kbdopen.c  -  kbdopen */

#include <xinu.h>
#include <keyboard.h>

/*------------------------------------------------------------------------
 * kbdopen  -  Open the ps/2 keyboard device
 *------------------------------------------------------------------------
 */

devcall	kbdopen (
	 struct	dentry	*devptr,	/* Entry in device switch table	*/
	 char	*name,			/* Unused for a kbd */
	 char	*mode			/* Unused for a kbd */
	)
{
	outportb(0x21, inportb(0x21) & ~0x02);
	
	if (kbd_opened == TRUE) {
		if (kbd_owner == getpid()) {
			return devptr->dvnum; // Ya es mío, permitir
		}
		return SYSERR; // Otro lo está usando
	}

	kbd_opened = TRUE;
	kbd_owner = getpid();

	/* Definición origianl de kbdopen
	// No action -- just return the device descriptor */
	return devptr->dvnum;	
}
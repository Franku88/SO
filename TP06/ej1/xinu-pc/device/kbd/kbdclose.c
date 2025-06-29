/* kbdclose.c  -  kbdclose */

#include <xinu.h>
#include <keyboard.h>

/*------------------------------------------------------------------------
 * kbdclose  -  Close the keyboard device
 *------------------------------------------------------------------------
 */
devcall	kbdclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	if (kbd_owner != getpid()) {
		return SYSERR;
	}

	kbd_opened = FALSE;
	kbd_owner = -1;
	
	return OK;
}

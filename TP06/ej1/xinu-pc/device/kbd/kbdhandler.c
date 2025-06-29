/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

/* kbdhandler.c - kbdhandler */

#include <xinu.h>
#include <keyboard.h>

unsigned char get_scancode()
{
    unsigned char inputdata;
    inputdata = inportb(KEYBOARD_DATAREG);
    return inputdata;
}


/*------------------------------------------------------------------------
 *  kbdhandler  -  Handle an interrupt for the keyboard device
 *------------------------------------------------------------------------
 */
void kbdhandler(void)
{
	unsigned char scancode = get_scancode();
	//kprintf("\r[IRQ: 0x%02X]  \n", scancode);

	if (kbd_count < KBD_BUFF_SIZE) { //si el buffer no esta lleno
		// Inserta scancode en buffer
		kbd_buffer[kbd_tail] = scancode;
		kbd_tail = (kbd_tail + 1) % KBD_BUFF_SIZE;
		kbd_count++;

		signal(kbd_sem_filled);	
	} //Si está lleno, descarta

	/* Codigo original
	char t[80];
	unsigned char scancode; 
	unsigned int shift_key = 0;
	int i = 10;

	scancode = get_scancode();
	sprintf(t, "kbd: 0x%x     ", scancode);
	print_text_on_vga(10, 300, t);

	if(scancode == 0x2A) {
		shift_key = 1;//Shift key is pressed
	} else if(scancode & 0xAA) {
		shift_key = 0;//Shift Key is not pressed
	} else {          
		if (scancode & 0x80) {
			int shiftaltctrl = 1;//Put anything to see what special keys were pressed
		} else {  
		}     
	}
	*/
}


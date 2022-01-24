/****************************************************************************************************
* written by Ross Ridge                                                                             *
* https://stackoverflow.com/questions/40961527/checking-if-a-key-is-down-in-ms-dos-c-c/40963633     *
*****************************************************************************************************/
/*
There is no function provided by Turbo C++, MS-DOS or the BIOS that corresponds to Windows function GetAsyncKeyState. The BIOS only keeps track of which modifier keys (Shift, Ctrl, or Alt) are held down, it doesn't track any of the other keys. If you want to do that you need to talk to the keyboard controller directly and monitor the make (key pressed) and break (key released) scan codes it receives from the keyboard.

To do that you'll need to hook the keyboard interrupt (IRQ 1, INT 0x09), read the scancodes from the keyboard controller and then update your own keyboard state table.

Here's a simple program that demonstrates how do this:
#include <conio.h>
#include <dos.h>
#include <stdio.h>

unsigned char normal_keys[0x60];
unsigned char extended_keys[0x60];

static void interrupt 
keyb_int() {
    static unsigned char buffer;
    unsigned char rawcode;
    unsigned char make_break;
    int scancode;

    rawcode = inp(0x60); // read scancode from keyboard controller 
    make_break = !(rawcode & 0x80); // bit 7: 0 = make, 1 = break 
    scancode = rawcode & 0x7F;

    if (buffer == 0xE0) { // second byte of an extended key 
        if (scancode < 0x60) {
            extended_keys[scancode] = make_break;
        }
        buffer = 0;
    } else if (buffer >= 0xE1 && buffer <= 0xE2) {
        buffer = 0; // ingore these extended keys 
    } else if (rawcode >= 0xE0 && rawcode <= 0xE2) {
        buffer = rawcode; // first byte of an extended key 
    } else if (scancode < 0x60) {
        normal_keys[scancode] = make_break;
    }

    outp(0x20, 0x20); // must send EOI to finish interrupt 
}

static void interrupt (*old_keyb_int)();

void
hook_keyb_int(void) {
    old_keyb_int = getvect(0x09);
    setvect(0x09, keyb_int);
}

void
unhook_keyb_int(void) {
    if (old_keyb_int != NULL) {
        setvect(0x09, old_keyb_int);
        old_keyb_int = NULL;
    }
}

int
ctrlbrk_handler(void) {
    unhook_keyb_int();
    _setcursortype(_NORMALCURSOR);
    return 0;
}

static
putkeys(int y, unsigned char const *keys) {
    int i;
    gotoxy(1, y);
    for (i = 0; i < 0x30; i++) {
        putch(keys[i] + '0');
    }
}

void
game(void) {
    _setcursortype(_NOCURSOR);
    clrscr();
    while(!normal_keys[1]) {
        putkeys(1, normal_keys);
        putkeys(2, normal_keys + 0x30);
        putkeys(4, extended_keys);
        putkeys(5, extended_keys + 0x30);
    }
    gotoxy(1, 6);
    _setcursortype(_NORMALCURSOR);
}

int
main() {
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
    game();
    unhook_keyb_int();
    return 0;
}   
The code above has been compiled with Borland C++ 3.1 and tested under DOSBox and MS-DOS 6.11 running under VirtualBox. It shows the current state of keyboard a string of 0's and 1's, a 1 indicating that the key corresponding to that position's scan code is being pressed. Press the ESC key to exit the program.

Note that the program doesn't chain the original keyboard handler, so the normal MS-DOS and BIOS keyboard functions will not work while the keyboard interrupt is hooked. Also note that it restores original keyboard handler before exiting. This is critical because MS-DOS won't do this itself. It also properly handles extended keys that send two byte scan codes, which was the problem with the code in the question you linked to in your answer here.

Share
Improve this answer
Follow
edited Dec 4 '16 at 21:09
answered Dec 4 '16 at 21:03

Ross Ridge
36.3k77 gold badges6969 silver badges108

*/

#include <dos.h>
#include <conio.h>
#include <stdlib.h>

#include "keyb.h"

unsigned char normal_keys[0x60];
unsigned char extended_keys[0x60];

static void interrupt keyb_int() {
//void interrupt keyb_int() {
    static unsigned char buffer;
    unsigned char rawcode;
    unsigned char make_break;
    int scancode;

    rawcode = inp(0x60); /* read scancode from keyboard controller */
    make_break = !(rawcode & 0x80); /* bit 7: 0 = make, 1 = break */
    scancode = rawcode & 0x7F;

    if (buffer == 0xE0) { /* second byte of an extended key */
        if (scancode < 0x60) {
            extended_keys[scancode] = make_break;
        }
        buffer = 0;
    } else if (buffer >= 0xE1 && buffer <= 0xE2) {
        buffer = 0; /* ingore these extended keys */
    } else if (rawcode >= 0xE0 && rawcode <= 0xE2) {
        buffer = rawcode; /* first byte of an extended key */
    } else if (scancode < 0x60) {
        normal_keys[scancode] = make_break;
    }

    outp(0x20, 0x20); /* must send EOI to finish interrupt */
}

//void interrupt (*old_keyb_int)();
static void interrupt (*old_keyb_int)();

void hook_keyb_int(void) {
    old_keyb_int = getvect(0x09);
    setvect(0x09, keyb_int);
}

void unhook_keyb_int(void) {
    if (old_keyb_int != NULL) {
        setvect(0x09, old_keyb_int);
        old_keyb_int = NULL;
    }
}

int ctrlbrk_handler(void) {
    unhook_keyb_int();
    _setcursortype(_NORMALCURSOR);
    return 0;
}

int get_key_status(unsigned char ch){ // 1 pressed  0 not
	return  normal_keys[ch] || extended_keys[ch];
}

char get_pressed_key(){
	int i;
    for (i = 0; i < 0x30 + 0x30; i++) {
        if(normal_keys[i] == 1) 
			switch(i){
				case SCAN_A: return 'A'; 
				case SCAN_B: return 'B'; 
				case SCAN_C: return 'C'; 
				case SCAN_D: return 'D'; 
				case SCAN_E: return 'E'; 
				case SCAN_F: return 'F'; 
				case SCAN_G: return 'G'; 
				case SCAN_H: return 'H'; 
				case SCAN_I: return 'I'; 
				case SCAN_J: return 'J'; 
				case SCAN_K: return 'K'; 
				case SCAN_L: return 'L'; 
				case SCAN_M: return 'M'; 
				case SCAN_N: return 'N'; 
				case SCAN_O: return 'O'; 
				case SCAN_P: return 'P'; 
				case SCAN_R: return 'R'; 
				case SCAN_Q: return 'Q'; 
				case SCAN_S: return 'S'; 
				case SCAN_T: return 'T'; 
				case SCAN_U: return 'U'; 
				case SCAN_V: return 'V'; 
				case SCAN_W: return 'W'; 
				case SCAN_X: return 'X'; 
				case SCAN_Y: return 'Y'; 
				case SCAN_Z: return 'Z'; 
				case SCAN_0: return '0'; 
				case SCAN_1: return '1'; 
				case SCAN_2: return '2'; 
				case SCAN_3: return '3'; 
				case SCAN_4: return '4'; 
				case SCAN_5: return '5'; 
				case SCAN_6: return '6'; 
				case SCAN_7: return '7'; 
				case SCAN_8: return '8'; 
				case SCAN_9: return '9'; 
				default: return '#';
			}
    }
	return '#';
}


#include "scancode.h"
/*
#define e_Key 0x12
#define d_Key 0x20
#define o_Key 0x18
#define l_Key 0x26
#define a_Key 0x1E
#define s_Key 0x1F
#define q_Key 0x10
#define n_Key 0x31

#define left_Key 0x4B
#define right_Key 0x4D
#define up_Key 0x48
#define down_Key 0x50

#define esc_Key 0x01
#define back_Key 0x0E
#define home_Key 0x47
#define end_Key 0x4F
#define space_Key 0x39
#define del_Key 0x53
#define ins_Key 0x52
*/

//https://www.scs.stanford.edu/10wi-cs140/pintos/specs/kbd/scancodes-9.html

//void interrupt keyb_int();
//void interrupt (*old_keyb_int)();
void hook_keyb_int(void);
void unhook_keyb_int(void);
int ctrlbrk_handler(void);
int get_key_status(unsigned char ch); // 1 pressed  0 not
char get_pressed_key();

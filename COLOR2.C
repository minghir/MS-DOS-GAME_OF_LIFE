
#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <math.h>

#include "LVGA\\lvga.h"
#include "LVGA\\keyb.h"
#include "LVGA\\ini.h"
#include "LVGA\\flog.h"


#define MAX_ZOOM 5

int zoom = 30;
float a_ratio = 1.2;
unsigned long int tstart,tend, fps_counter = 0, fps_count = 0;
int delay_msec = 200;
int grid = 1;


int pan_vert = 5;
int pan_horiz = 60;

void initialize(void)
{
    initialize_graphics(true);
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
    init_chars_15();
}


unsigned long int get_msec(){
         struct dostime_t t;
        _dos_gettime(&t);
        //return t.hour * 3600 * 1000 + t.minute * 60 * 1000  + t.second * 1000  + t.hsecond;
        return  t.second * 1000  + t.hsecond;
 }

void draw_grid(){
               // rect_fast(0,0, 199*a_ratio, 199, WHITE);
                //rect_fast(200*a_ratio,0,310,199, WHITE);
}

void print_fps(){



}

void clear_screen(){
        rect_fill(0,0, 319, 199, BLACK);
}


void draw(){
        int i,x,y;
        char buf[5];
        clear_screen();
        
		x=pan_horiz;
		y=pan_vert;

        for( i= 1; i<=25; i++){
                rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,100+i);
                if(i%5 == 0){
                        y += zoom;
                        x = pan_horiz;
                }else{
                        x += zoom;
                }
        }
}



int main(){

        initialize();
		randomize(); 

                while(1){
                    if(get_key_status(SCAN_Q) || get_key_status(SCAN_ESC)){
                        break;
					}

                        draw();
                        show_buffer();
                }


                unhook_keyb_int();

                delay(100);
                exit_graphic();
                exit(0);

}


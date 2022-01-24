
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

int zoom = 15;
float a_ratio = 1.2;
unsigned long int tstart,tend, fps_counter = 0, population = 0, generation = 0, fps_count = 0;
int delay_msec = 200;
int grid = 1;


int pan_vert = 0;
int pan_horiz = 0;

void initialize(void)
{
        initialize_graphics(true);
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
        init_chars_15();
        //flog_start();
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
        rect_fill(0,0, 319, 199, WHITE);
}


void draw(){
        int i,x,y;
		char buf[5];
        clear_screen();
        x=0;y=0;
        for (i= 0 ; i<=256;i++){
                rect_fill(x,y,x+zoom,y+zoom, i);
				itoa(i,buf,10);
				put_str_15(x+3,y,buf,WHITE,0);


                if(x > 320-2*zoom){
                        y += zoom;
                        x = 0;
                }else{
                        x += zoom;
                }

        }

}



int main(){

        initialize();

                while(1){
                                if(get_key_status(q_Key)){
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


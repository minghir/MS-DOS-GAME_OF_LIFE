
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
unsigned long int tstart,tend, fps_counter = 0, fps_count = 0;
int delay_msec = 200;
int grid = 1;

int colors[6];
int table[100];
int current_color;
int old_color;

int pan_vert = 5;
int pan_horiz = 60;

void initialize(void)
{
	
    initialize_graphics(true);
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
    init_chars_15();
	
	colors[0] = 42;
	colors[1] = 35;
	colors[2] = 52;
	colors[3] = 49;
	colors[4] = 27;
	colors[5] = 44;
	
	

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
        int i,x,y,c;
        char buf[5];
        clear_screen();
        
		rect_fill(0,zoom,(zoom)*a_ratio,zoom*2,current_color);
		rect_fill(0,zoom*2,(zoom)*a_ratio,zoom*3,old_color);
		
		x=pan_horiz;
		y=pan_vert;

        for( i= 1; i<=100; i++){
				rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,table[i-1]);
                if(i%10 == 0){
                        y += zoom;
                        x = pan_horiz;
                }else{
                        x += zoom;
                }
        }
		
		x=pan_horiz+30;
		y = 170;
		for( i = 0; i<6;i++){
			rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,colors[i]);
			x +=zoom;
		}
		
		
}

void init_table(){
	int i;
	for (i = 0; i<100;i++){
		table[i] = colors[random(6)];
	}
	current_color = table[0];
	old_color = current_color;
}

bool check_solution( ){
	int i = 0;
	int color = table[0];
	
	for (i = 1 ; i<100; i++)
		if(table[i] != color)
			return false;
	
	return true;
}

void calculate_table(){
	int i;
	table[0] = current_color;
	for(i = 1;i<100;i++){
		if(table[i] == current_color) continue;
		if(table[i] == old_color) table[i] = current_color;
	}
}

int main(){
        initialize();
		randomize(); 
		init_table();
                while(1){
                    if(get_key_status(SCAN_Q) || get_key_status(SCAN_ESC)){
                        break;
					}
					if(get_key_status(SCAN_R)){
                        init_table();
						draw();
						delay(200);
					}
					
					if(get_key_status(SCAN_1)){
						old_color=current_color;
						current_color = colors[0];
                        calculate_table();
						delay(200);
					}
					
					if(get_key_status(SCAN_2)){
						old_color=current_color;
						current_color = colors[1];
                        calculate_table();
						delay(200);
					}
					
					if(get_key_status(SCAN_3)){
						old_color=current_color;
						current_color = colors[2];
                        calculate_table();
						delay(200);
					}
					
					if(get_key_status(SCAN_4)){
						old_color=current_color;
						current_color = colors[3];
                        calculate_table();
						delay(200);
					}
					
					if(get_key_status(SCAN_5)){
						old_color=current_color;
						current_color = colors[4];
                        calculate_table();
						delay(200);
					}
					
					if(get_key_status(SCAN_6)){
						old_color=current_color;
						current_color = colors[5];
                        calculate_table();
						delay(200);
					}
					

                        draw();
                        show_buffer();
                }
                unhook_keyb_int();
                delay(100);
                exit_graphic();
                exit(0);

}


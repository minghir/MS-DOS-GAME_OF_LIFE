
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

//int zoom = 15;
int zoom = 30;
float a_ratio = 1.2;
unsigned long int tstart,tend, fps_counter = 0, fps_count = 0;
int delay_msec = 400;
int grid = 1;

int colors[6];
int table[100];

int current_color;
int old_color;

int pan_vert = 5;
int pan_horiz = 60;

void initialize(void)
{
	
	InstallMouse();

	SetMousePosi(1,1);
	
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

        //for( i= 1; i<=100; i++){
		for( i= 1; i<=25; i++){
				rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,table[i-1]);
                //if(i%10 == 0){
				if(i%5 == 0){
                        y += zoom;
                        x = pan_horiz;
                }else{
                        x += zoom;
                }
        }
		
		x=pan_horiz+30;
		y = 170;
		for( i = 0; i<6;i++){
			rect_fill(x*a_ratio,y,(x+zoom/2)*a_ratio,y+zoom/2,colors[i]);
			x +=zoom/2;
		}
		
		RestrictMousePtr(0,0,318,197);
		GetMouseStatus();
		put_str_15(GetPosX(),GetPosY(),"m",WHITE,240);
}

void init_table(){
	int i;
	//for (i = 0; i<100;i++){
	for (i = 0; i<25;i++){
		table[i] = colors[random(6)];
	
	}
	current_color = table[0];
	old_color = current_color;
	
}

void calculate_table2(int cell){

	if(old_color == current_color)
			return;

	if(table[cell] == old_color ){
		table[cell] = current_color;	
	}else{
		return;
	}
	
	if( ( table[cell+1] == old_color) && cell%5 != 4)
		calculate_table2(cell+1);
	
	if( ( table[cell - 1] == old_color ) && cell%5 != 0)
		calculate_table2(cell -1 );
		
	if( ( table[cell+5] == old_color) && cell < 19)
		calculate_table2(cell+5);
		
	if( ( table[cell-5] == old_color) && cell > 4 )
		calculate_table2(cell-5);

}

bool usr_input(){
	int x,y;
	int tstart_upd,tend_upd;
	
					if(get_key_status(SCAN_Q) || get_key_status(SCAN_ESC)){
                        return false;
					}
					
					if(get_key_status(SCAN_R)){
                        init_table();
						//draw();
						return true;
						
					}
					
					if(get_key_status(SCAN_1)){
						old_color=current_color;
						current_color = colors[0];
                        calculate_table2(0);
						return true;
					}
					
					if(get_key_status(SCAN_2)){
						old_color=current_color;
						current_color = colors[1];
                        calculate_table2(0);
						return true;
					}
					
					if(get_key_status(SCAN_3)){
						old_color=current_color;
						current_color = colors[2];
                        calculate_table2(0);
						return true;
					}
					
					if(get_key_status(SCAN_4)){
						old_color=current_color;
						current_color = colors[3];
                        calculate_table2(0);
						return true;
					}
					
					if(get_key_status(SCAN_5)){
						old_color=current_color;
						current_color = colors[4];
                        calculate_table2(0);
						return true;
					}
					
					if(get_key_status(SCAN_6)){
						old_color=current_color;
						current_color = colors[5];
                        calculate_table2(0);
						return true;
					}
					
					GetMouseStatus();
					x = GetPosX();
					y = GetPosY();
					
					if(GetButton() == 1){
						delay(150);
						if(y>=170 && y<= 185){
							if((x >= (pan_horiz+30)*a_ratio ) && (x <= (pan_horiz+ 45)*a_ratio)){
								old_color=current_color;
								current_color = colors[0];
								calculate_table2(0);
								return true;
							}
							
							if((x >= (pan_horiz+46)*a_ratio ) && (x <= (pan_horiz+ 60)*a_ratio)){
								old_color=current_color;
								current_color = colors[1];
								calculate_table2(0);
								return true;
							}
							
							if((x >= (pan_horiz+61)*a_ratio ) && (x <= (pan_horiz+ 75)*a_ratio)){
								old_color=current_color;
								current_color = colors[2];
								calculate_table2(0);
								return true;
							}
							
							if((x >= (pan_horiz+76)*a_ratio ) && (x <= (pan_horiz+ 90)*a_ratio)){
								old_color=current_color;
								current_color = colors[3];
								calculate_table2(0);
								return true;
							}
							
							if((x >= (pan_horiz+91)*a_ratio ) && (x <= (pan_horiz+ 105)*a_ratio)){
								old_color=current_color;
								current_color = colors[4];
								calculate_table2(0);
								return true;
							}
							
							if((x >= (pan_horiz+106)*a_ratio ) && (x <= (pan_horiz+ 120)*a_ratio)){
								old_color=current_color;
								current_color = colors[5];
								calculate_table2(0);
								return true;
							}
							
						}else{
							return true;
						}
						
					}
	return true;
}

int main(){
				
        initialize();
		randomize(); 
		init_table();
                while(1){
                 					
					if(!usr_input())
						break;
					
					draw();
					show_buffer();
                }
				
                unhook_keyb_int();
                delay(100);
                exit_graphic();
                exit(0);

}


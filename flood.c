#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <math.h>

#include "LVGA\\lvga.h"
#include "LVGA\\keyb.h"

int zoom = 15;
float a_ratio = 1.2;

int colors[6];
int table[100];

int current_color;
int old_color;

int pan_vert = 5;
int pan_horiz = 60;

int clicks = 0;
int max_score = 17;

bool end_game = false;

void initialize(void){
	
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

void end_win(){
	put_str_15(150,190,"WINNER",WHITE,240);
}

void end_loose(){
	put_str_15(150,190,"LOOSER",WHITE,240);
}


void print_score(){
	char fps_c[10];

	sprintf(fps_c,"%u:%u",clicks,max_score );
	if(clicks > 9)
		put_str_15(153,160,fps_c,WHITE,240);
	else
		put_str_15(157,160,fps_c,WHITE,240);
}

void clear_screen(){
        rect_fill(0,0, 319, 199, BLACK);
}


void draw(){
        int i,x,y,c;
        char buf[5];
        clear_screen();
        
		//rect_fill(0,zoom,(zoom)*a_ratio,zoom*2,current_color);
		//rect_fill(0,zoom*2,(zoom)*a_ratio,zoom*3,old_color);
		
		x=pan_horiz;
		y=pan_vert;

        for( i= 1; i<=100; i++){
		//for( i= 1; i<=25; i++){
				rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,table[i-1]);
                if(i%10 == 0){
			//	if(i%5 == 0){
                        y += zoom;
                        x = pan_horiz;
                }else{
                        x += zoom;
                }
        }
		
		x=90;
		y = 170;
		for( i = 0; i<6;i++){
			rect_fill(x*a_ratio,170,(x+15)*a_ratio,185,colors[i]);
			x +=15;
		}
		
		RestrictMousePtr(0,0,318,197);
		GetMouseStatus();
		put_str_15(GetPosX(),GetPosY(),"m",WHITE,240);
		
		print_score();
		
		if(end_game){
			if(clicks <= max_score)
				end_win();
			else
				end_loose();
		}
}

void init_table(){
	int i;
	for (i = 0; i<100;i++){
	//for (i = 0; i<25;i++){
		table[i] = colors[random(6)];
	
	}
	current_color = table[0];
	old_color = current_color;
	clicks = 0;
	end_game = false;
	
}

bool check_table(){
	int color,i;
	color = table[0];
	//for(i = 1; i< 25; i++)
	for(i = 1; i< 100; i++)
		if(color != table[i])
			return false;
			
	end_game = true;
	return true;
}

void calculate_table2(int cell){

	if(old_color == current_color)
		return;

	if(table[cell] == old_color ){
		table[cell] = current_color;	
	}else{
		return;
	}
	
	//if( ( table[cell+1] == old_color) && cell%5 != 4)
	if( ( table[cell+1] == old_color) && cell%10 != 9)
		calculate_table2(cell+1);
	
	//if( ( table[cell - 1] == old_color ) && cell%5 != 0)
	if( ( table[cell - 1] == old_color ) && cell%10 != 0)
		calculate_table2(cell -1 );
		
	//if( ( table[cell+5] == old_color) && cell < 20)
	if( ( table[cell+10] == old_color) && cell < 90)
		//calculate_table2(cell+5);
		calculate_table2(cell+10);
		
	//if( ( table[cell-5] == old_color) && cell > 4 )
	if( ( table[cell-10] == old_color) && cell > 9 )
		//calculate_table2(cell-5);
		calculate_table2(cell-10);

}

void change_color(int color){
	old_color=current_color;
	current_color = color;
	calculate_table2(0);
	
	if(!end_game)
		clicks++;
	delay(150);
	check_table();
}

bool usr_input(){
	int x,y;
					if(get_key_status(SCAN_Q) || get_key_status(SCAN_ESC)){
                        return false;
					}
					
					if(get_key_status(SCAN_R) || get_key_status(SCAN_ENTER)){
                        init_table();
						delay(300);
						return true;
					}
					
					if(get_key_status(SCAN_1)){
						change_color(colors[0]);
						return true;
					}
					
					if(get_key_status(SCAN_2)){
						change_color(colors[1]);
						return true;
					}
					
					if(get_key_status(SCAN_3)){
						change_color(colors[2]);
						return true;
					}
					
					if(get_key_status(SCAN_4)){
						change_color(colors[3]);
						return true;
					}
					
					if(get_key_status(SCAN_5)){
						change_color(colors[4]);
						return true;
					}
					
					if(get_key_status(SCAN_6)){
						change_color(colors[5]);
						return true;
					}
					
					
					GetMouseStatus();
					x = GetPosX();
					y = GetPosY();
					
					if(GetButton() == 1){
						
						while(1){
								GetMouseStatus();
								if(GetButton() == 1) {
									//delay(20);
								}else{
									break;
								}
							}
					
						if(y>=170 && y<= 185){
							if((x >= (pan_horiz+30)*a_ratio ) && (x <= (pan_horiz+ 45)*a_ratio)){
								change_color(colors[0]);
								return true;
							}
							
							if((x >= (pan_horiz+46)*a_ratio ) && (x <= (pan_horiz+ 60)*a_ratio)){
								change_color(colors[1]);
								return true;
							}
							
							if((x >= (pan_horiz+61)*a_ratio ) && (x <= (pan_horiz+ 75)*a_ratio)){
								change_color(colors[2]);
								return true;
							}
							
							if((x >= (pan_horiz+76)*a_ratio ) && (x <= (pan_horiz+ 90)*a_ratio)){
								change_color(colors[3]);
								return true;
							}
							
							if((x >= (pan_horiz+91)*a_ratio ) && (x <= (pan_horiz+ 105)*a_ratio)){
								change_color(colors[4]);
								return true;
							}
							
							if((x >= (pan_horiz+106)*a_ratio ) && (x <= (pan_horiz+ 120)*a_ratio)){
								change_color(colors[5]);
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
				return 1;
}
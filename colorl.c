#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <math.h>

#include "LVGA\\lvga.h"
#include "LVGA\\keyb.h"


#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define MWHITE  30

int zoom = 30;
float a_ratio = 1.2;

int colors[6];
int table[100];

int pan_vert = 13;
int pan_horiz = 60;

int clicks = 0;

int line_found = -1;
int col_found = -1;

int selector = 24;
int selected_cell = -1;

void initialize(void){
	
    initialize_graphics(true);
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
    init_chars_15();
	
	colors[0] = 6;//42;
	colors[1] = 9;
	colors[2] = 2;
	colors[3] = 12;
	colors[4] = 13;
}

void print_score(){
	char fps_c[10];
	sprintf(fps_c,"%u",clicks );
	put_str_15(160,170,fps_c,MWHITE,240);	
}

void clear_screen(){
        //rect_fill(0,0, 319, 199, BLACK);
		rect_fill(0,0, 319, 199, 17);
}



void draw_selection(){
	int color;
	if(selected_cell > -1)
		color = 44;
	else
		color = MWHITE;

	rect_fast((pan_horiz+selector%5*zoom)*a_ratio,(pan_vert+(selector/5)*zoom),(pan_horiz+selector%5*zoom+zoom)*a_ratio,(pan_vert+(selector/5)*zoom+zoom),color);
}

void draw(){
        int i,x,y;
        clear_screen();
        
		x=pan_horiz;
		y=pan_vert;

		for( i= 1; i<=25; i++){

			rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,table[i-1]);
			rect_fast(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,BLACK);

				if(i%5 == 0){
                        y += zoom;
                        x = pan_horiz;
                }else{
                        x += zoom;
                }
				
        }
		draw_selection();
		print_score();
}

void init_table(){
	int i;
	for (i = 0; i<24;i++){
		table[i] = colors[random(5)];
		draw();
		show_buffer();
		delay(60);
	}
	
	table[24] = 0;
/*
	table[0] = colors[1];
	table[1] = colors[1];
	table[2] = colors[1];
	table[3] = colors[1];
*/	
	line_found = -1;
	col_found = -1;

}

int check_table_lines(){
	int i;
	for(i = 0; i <= 20 ; i += 5){
		if( (table[i] == table[i+1]) && (table[i] == table[i+2]) && (table[i] == table[i+3] )){
			return i;
		}
		if( (table[i+1] == table[i+2]) && (table[i+1] == table[i+3]) && (table[i+1] == table[i+4] )){
			return i+1;
		}
	}
	
	return -1;
}

int check_table_cols(){
	int i;
	for(i = 0; i <= 4 ; i++){
		if( (table[i] == table[i+5]) && (table[i] == table[i+10]) && (table[i] == table[i+15] )){
			return i;
		}
		if( (table[i+5] == table[i+10]) && (table[i+5] == table[i+15]) && (table[i+5] == table[i+20] )){
			return i+5;
		}
	}
	
	return -1;
}

void flick_square(int x1, int y1, int x2, int y2,int cl){
		int i=0;
		int color;
		while( i!=5){
			color = color == cl ? MWHITE : cl;
			
			rect_fill(x1*a_ratio,y1,x2*a_ratio,y2,color);	
			rect_fast(x1*a_ratio,y1,x2*a_ratio,y2,(color == cl?MWHITE:cl));	
			show_buffer();
			
			if(color == cl)
				delay(120);	
			else
				delay(60);	
			i++;
		}
	
}

void replace_line(){
	int i,rnd;
	flick_square((pan_horiz+line_found%5*zoom),(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom),(pan_vert+(line_found/5)*zoom)+zoom,table[line_found]);

	for(i = 0; i<4;i++){
		table[line_found+i] = colors[random(5)];
		draw();
		show_buffer();
		delay(120);
	}
	
	line_found = -1;
}

void replace_col(){
	int i;
	flick_square((pan_horiz+col_found%5*zoom),pan_vert+(col_found>4?zoom:0), (pan_horiz+col_found%5*zoom+zoom),pan_vert+4*zoom+(col_found>4?zoom:0),table[col_found]);		
	for(i=0;i<4;i++){
		table[col_found+(i*5)] = colors[random(5)];
		draw();
		show_buffer();
		delay(120);
	}
	col_found = -1;
}

void check(){
	line_found = check_table_lines();
	if(line_found > -1){
		replace_line();
		clicks++;
	}
	col_found = check_table_cols();
	if(col_found > -1){
		replace_col();
		clicks++;
	}
}

void move_cell(int pos){
	
	switch(pos){
		case LEFT:
			if(table[selected_cell - 1] == 0){
				table[selected_cell - 1] = table[selected_cell];
				table[selected_cell] = 0;
			}	
		break;
		
		case RIGHT:
			if(table[selected_cell + 1] == 0){
				table[selected_cell + 1] = table[selected_cell];
				table[selected_cell] = 0;
			}	
		break;
		case UP:
			if(table[selected_cell - 5] == 0){
				table[selected_cell - 5] = table[selected_cell];
				table[selected_cell] = 0;
			}	
		break;
		case DOWN:
			if(table[selected_cell + 5] == 0){
				table[selected_cell + 5] = table[selected_cell];
				table[selected_cell] = 0;
			}	
		break;
	}
	
	draw();
	show_buffer();
	
	selected_cell = -1;
	
}

bool usr_input(){

					if(get_key_status(SCAN_Q) || get_key_status(SCAN_ESC)){
                        return false;
					}
					
					if( get_key_status(SCAN_ENTER) ){

						while(get_key_status(SCAN_ENTER)){}
							selected_cell = selector;
							check();
						return true;
					}
					
					if(get_key_status(SCAN_LEFT_ARROW)){
						if(selector > 0)
							selector--;

						if(selected_cell > -1)
							move_cell(LEFT);
						draw();	
						show_buffer();
						while(get_key_status(SCAN_LEFT_ARROW)){}
						check();
						return true;
					}
					
					if(get_key_status(SCAN_RIGHT_ARROW)){
						if(selector < 24 )
							selector++;
							
						if(selected_cell > -1)
							move_cell(RIGHT);	
						
						draw();	
						show_buffer();
						while(get_key_status(SCAN_RIGHT_ARROW)){}
						check();
						return true;
					}
					
					if(get_key_status(SCAN_UP_ARROW)){
						if(selector > 4 )
							selector -= 5;
							
						if(selected_cell > -1)
							move_cell(UP);		
							
						draw();	
						show_buffer();
						while(get_key_status(SCAN_UP_ARROW)){}
						check();
						return true;
					}
					
					if(get_key_status(SCAN_DOWN_ARROW)){
						if(selector < 20 )
							selector += 5;
							
						if(selected_cell > -1)
							move_cell(DOWN);		
							
						draw();	
						show_buffer();
						while(get_key_status(SCAN_DOWN_ARROW)){}
						check();
						return true;
					}
					
	return true;
}

int main(){
        initialize();
		randomize(); 
		init_table();
		check();
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
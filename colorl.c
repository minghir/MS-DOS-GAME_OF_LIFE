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

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define MWHITE  29

int DELAY = 2;

#define SHORT_DELAY DELAY * 30
#define LONG_DELAY DELAY * 60

bool hard_mode = true;

int zoom = 30;
float a_ratio = 1.2;

int colors[6];
int table[25];

int pan_vert = 13;
int pan_horiz = 60;

int score = 0;

int line_found = -1;
int col_found = -1;
bool five_found = false;

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
	sprintf(fps_c,"%u",score );
	put_str_15(160,170,fps_c,MWHITE,240);	
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
        rect_fill(0,0, 319, 199, 20); // clear screen
        
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
		delay(SHORT_DELAY);
	}
	
	table[24] = 0;
/*
	table[0] = colors[1];
	table[5] = colors[1];
	table[10] = colors[1];
	table[15] = colors[1];
	table[20] = colors[1];
*/
	line_found = -1;
	col_found = -1;

}

void flick_square(int x1, int y1, int x2, int y2,int cl){
		int i=0;
		int color;
		int flick_color = 44;
		while( i!=6){
			color = color == cl ? flick_color : cl;
			
			//rect_fill(x1*a_ratio,y1,x2*a_ratio,y2,color);	
			rect_fast((pan_horiz+selector%5*zoom)*a_ratio,(pan_vert+(selector/5)*zoom),(pan_horiz+selector%5*zoom+zoom)*a_ratio,(pan_vert+(selector/5)*zoom+zoom),0);
			rect_fast(x1*a_ratio,y1,x2*a_ratio,y2,(color == cl?flick_color:cl));	
			show_buffer();
			
			if(color == cl)
				delay(LONG_DELAY);	
			else
				delay(SHORT_DELAY);	
			i++;
		}
		rect_fast((pan_horiz+selector%5*zoom)*a_ratio,(pan_vert+(selector/5)*zoom),(pan_horiz+selector%5*zoom+zoom)*a_ratio,(pan_vert+(selector/5)*zoom+zoom),MWHITE);
		show_buffer();
}

void replace_line(){
	int i,no;
	no = five_found ? 5 : 4;
	flick_square((pan_horiz+line_found%5*zoom),(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + no*zoom),(pan_vert+(line_found/5)*zoom)+zoom,table[line_found]);
	
	for(i = 0; i<no;i++){
		table[line_found+i] = 0;
	}
	draw();
	show_buffer();
	delay(LONG_DELAY);
	
	for(i = 0; i<no;i++){
		table[line_found+i] = colors[random(5)];
		draw();
		show_buffer();
		delay(LONG_DELAY);
	}
	
	line_found = -1;
	five_found = false;
}

void replace_col(){
	int i,no;
	no = five_found ? 5 : 4;
	flick_square((pan_horiz+col_found%5*zoom),pan_vert+(col_found>4?zoom:0), (pan_horiz+col_found%5*zoom+zoom),pan_vert+no*zoom+(col_found>4?zoom:0),table[col_found]);		
	
	for(i=0;i<no;i++){
		table[col_found+(i*5)] = 0;
	}
	
	draw();
	show_buffer();
	delay(LONG_DELAY);
	
	for(i=0;i<no;i++){
		table[col_found+(i*5)] = colors[random(5)];
		draw();
		show_buffer();
		delay(LONG_DELAY);
	}
	col_found = -1;
	five_found = false;
}


int check_table_lines(){
	int i;
	for(i = 0; i <= 20 ; i += 5){
		if(hard_mode) {
			if( (table[i] == table[i+1]) && (table[i] == table[i+2]) && (table[i] == table[i+3] && (table[i] == table[i+4] ))){
					five_found = true;
				return i;
			}
		}else{
			if( (table[i] == table[i+1]) && (table[i] == table[i+2]) && (table[i] == table[i+3] )){
				if(table[i] == table[i+4] )
					five_found = true;
				return i;
			}
			if( (table[i+1] == table[i+2]) && (table[i+1] == table[i+3]) && (table[i+1] == table[i+4] )){
				return i+1;
			}
		}
	}
	
	return -1;
}

int check_table_cols(){
	int i;
	for(i = 0; i <= 4 ; i++){
		if(hard_mode) {
			if( (table[i] == table[i+5]) && (table[i] == table[i+10]) && (table[i] == table[i+15] ) && (table[i] == table[i+20])){
				five_found = true;
				return i;
			}
		}else{
			if( (table[i] == table[i+5]) && (table[i] == table[i+10]) && (table[i] == table[i+15] )){
				if(table[i] == table[i+20])
					five_found = true;
				return i;
			}
			if( (table[i+5] == table[i+10]) && (table[i+5] == table[i+15]) && (table[i+5] == table[i+20] )){
				return i+5;
			}
		}
	}
	
	return -1;
}

void check(){
	line_found = check_table_lines();
	if(line_found > -1){
		replace_line();
		if(!hard_mode && five_found)
			score +=2;
		else
			score++;
			
		five_found = false;
	}
	col_found = check_table_cols();
	if(col_found > -1){
		replace_col();
		if(!hard_mode && five_found)
			score +=2;
		else
			score++;
		
		five_found = false;
	}
}

void animate_move_cell(int pos,int actual_cell,int new_cell){

		int start_x, start_y, end_x, end_y, x, y;
		
		if(new_cell < 0 || new_cell > 24)
			return;
			
		
		start_x = (pan_horiz+actual_cell%5*zoom);
		start_y = (pan_vert+(actual_cell/5)*zoom);
		end_x = (pan_horiz+new_cell%5*zoom);
		end_y = (pan_vert+(new_cell/5)*zoom);

		x = start_x;
		y = start_y;
		
		while(x != end_x || y != end_y){
			
			rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,0);
			
			switch(pos){
				case LEFT:
					x -= 1;
					y = end_y;
				break;
				
				case RIGHT:
					x += 1;
					y = end_y;
				break;
				
				case UP:
					x = end_x;
					y -= 1;
				break;
				
				case DOWN:
					x = end_x;
					y += 1;
				break;
			}
			delay(DELAY);
			
			rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,table[actual_cell]);
			show_buffer();
		}	

}

void move_cell(int pos){
	
	switch(pos){
		case LEFT:
			if( selected_cell - 1  < 0) return;
			if(table[selected_cell - 1] == 0){
				table[selected_cell - 1] = table[selected_cell];
				
				animate_move_cell(pos,selected_cell,selected_cell - 1);
				
				table[selected_cell] = 0;
			}	
		break;
		
		case RIGHT:
			if( selected_cell + 1  > 24 ) return;
			if(table[selected_cell + 1] == 0){
				table[selected_cell + 1] = table[selected_cell];
				
				animate_move_cell(pos,selected_cell,selected_cell + 1);
				
				table[selected_cell] = 0;
			}	
		break;
		case UP:
			if( selected_cell - 5  < 0 ) return;
			if(table[selected_cell - 5] == 0){
				table[selected_cell - 5] = table[selected_cell];
				
				animate_move_cell(pos,selected_cell,selected_cell - 5);
				
				table[selected_cell] = 0;
			}	
		break;
		case DOWN:
			if( selected_cell + 5  > 24 ) return;
			if(table[selected_cell + 5] == 0){
				table[selected_cell + 5] = table[selected_cell];
				
				animate_move_cell(pos,selected_cell,selected_cell + 5);
				
				table[selected_cell] = 0;
			}	
		break;
	}
	
	selected_cell = -1;
	
}

bool usr_input(){

					if(get_key_status(SCAN_Q) || get_key_status(SCAN_ESC)){
                        return false;
					}
					
					if( get_key_status(SCAN_ENTER) ){
					
						if(table[selector] == 0)
							return true;
						
						while(get_key_status(SCAN_ENTER) );
							if(selected_cell > -1)
								selected_cell = -1;
							else
								selected_cell = selector;
						
						return true;
					}
					
					if(get_key_status(SCAN_LEFT_ARROW)){
						if(selector%5 == 0) return true;
						if(selector > 0)
							selector--;

						if(selected_cell > -1)
							move_cell(LEFT);
						draw();	
						show_buffer();
						while(get_key_status(SCAN_LEFT_ARROW)){}
						
						return true;
					}
					
					if(get_key_status(SCAN_RIGHT_ARROW)){
						if(selector%5 == 4) return true;
						if(selector < 24 )
							selector++;
							
						if(selected_cell > -1)
							move_cell(RIGHT);	
						
						draw();	
						show_buffer();
						while(get_key_status(SCAN_RIGHT_ARROW)){}
						
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
						
						return true;
					}
					
	return true;
}

int main(){
				
	ini_t *config = ini_load("colorl.ini");
	ini_sget(config, "colorl", "delay", "%d", &DELAY);
	ini_sget(config, "colorl", "hard_mode", "%d", &hard_mode);
	ini_free(config);				

	initialize();
	randomize(); 
	init_table();
	check();
		
	while(1){
		if(!usr_input())
			break;
		check();
		draw();
        show_buffer();
    }
				
	unhook_keyb_int();
	delay(LONG_DELAY);
	exit_graphic();
	return 1;
}
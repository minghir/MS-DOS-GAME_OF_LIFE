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

int zoom = 30;
float a_ratio = 1.2;

int colors[6];
int table[100];

//int current_color;
//int old_color;

int pan_vert = 13;
int pan_horiz = 60;

int clicks = 0;
int max_score = 17;

int line_found = -1;
int col_found = -1;

int selector = 24;
int selected_cell = -1;

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
	colors[4] = 44;
	//colors[5] = 27;
}

void end_win(){
	put_str_15(150,190,"WINNER",WHITE,240);
}

void end_loose(){
	put_str_15(150,190,"LOOSER",WHITE,240);
}


void print_score(){
	char fps_c[10];
	sprintf(fps_c,"%u",clicks );
	put_str_15(153,170,fps_c,WHITE,240);	
	
	//sprintf(fps_c,"%u:%u",clicks,max_score );
	
	/*
	if(clicks > 9)
		put_str_15(153,160,fps_c,WHITE,240);
	else
		put_str_15(157,160,fps_c,WHITE,240);
	*/
}

void clear_screen(){
        //rect_fill(0,0, 319, 199, BLACK);
		rect_fill(0,0, 319, 199, 17);
}


void draw(){
        int i,x,y,c;
        char buf[5];
        clear_screen();
        
		//rect_fill(0,zoom,(zoom)*a_ratio,zoom*2,current_color);
		//rect_fill(0,zoom*2,(zoom)*a_ratio,zoom*3,old_color);
		
		x=pan_horiz;
		y=pan_vert;

        //for( i= 1; i<=100; i++){
		for( i= 1; i<=25; i++){
		
				
		
				rect_fill(x*a_ratio,y,(x+zoom)*a_ratio,y+zoom,table[i-1]);
				
				if(i-1 == selector){
					if(selected_cell > -1)
					rect_fast(x*a_ratio,y,(x+zoom)*a_ratio-1,y+zoom-1,83);
					else
					rect_fast(x*a_ratio,y,(x+zoom)*a_ratio-1,y+zoom-1,BLACK);
				}
				
				
            //    if(i%10 == 0){
				if(i%5 == 0){
                        y += zoom;
                        x = pan_horiz;
                }else{
                        x += zoom;
                }
				
				
        }
		
		print_score();
		
		/*
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
		*/
		
		//print_score();
		
		
		
}

void init_table(){
	int i;
	//for (i = 0; i<100;i++){
	
	for (i = 0; i<25;i++){
		
	
	
		table[i] = colors[random(5)];
	//	draw();
//		show_buffer();
	//	delay(60);
	}
	
	
	
	table[3] = colors[2];
	//table[0] = colors[2];
	table[8] = colors[2];
	table[13] = colors[2];
	table[18] = colors[2];
	//table[20] = colors[2];
	
	
	table[24] = 0;
	
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

void replace_line(){
/*
	table[line_found] = 0;
	table[line_found+1] = 0;
	table[line_found+2] = 0;
	table[line_found+3] = 0;
		draw();
		show_buffer();
		delay(80);
	table[line_found] = WHITE;
	table[line_found+1] = WHITE;
	table[line_found+2] = WHITE;
	table[line_found+3] = WHITE;
		draw();
		show_buffer();
		delay(150);
	table[line_found] = 0;
	table[line_found+1] = 0;
	table[line_found+2] = 0;
	table[line_found+3] = 0;
		draw();
		show_buffer();
		delay(80);
	table[line_found] = WHITE;
	table[line_found+1] = WHITE;
	table[line_found+2] = WHITE;
	table[line_found+3] = WHITE;
		draw();
		show_buffer();
		delay(150);
	table[line_found] = 0;
	table[line_found+1] = 0;
	table[line_found+2] = 0;
	table[line_found+3] = 0;
		draw();
		show_buffer();
		delay(80);
	table[line_found] = WHITE;
	table[line_found+1] = WHITE;
	table[line_found+2] = WHITE;
	table[line_found+3] = WHITE;
		draw();
		show_buffer();
		delay(150);	
*/

		//y=pan_vert;
	//int line_no = line_found%5;	
	rect_fast((pan_horiz+line_found%5*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom)+zoom,RED);		
	show_buffer();
	delay(120);	
	rect_fast((pan_horiz+line_found%5*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom)+zoom,WHITE);
	show_buffer();
	delay(120);	
	rect_fast((pan_horiz+line_found%5*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom)+zoom,RED);		
	show_buffer();
	delay(120);	
	rect_fast((pan_horiz+line_found%5*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom)+zoom,WHITE);
	show_buffer();
	delay(120);	
	rect_fast((pan_horiz+line_found%5*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom)+zoom,RED);		
	show_buffer();
	delay(120);	
	rect_fast((pan_horiz+line_found%5*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom)+zoom,WHITE);
	show_buffer();
	delay(120);	
	rect_fast((pan_horiz+line_found%5*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom)+zoom,RED);		
	show_buffer();
	delay(120);	
	rect_fast((pan_horiz+line_found%5*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom), (pan_horiz+line_found%5*zoom + 4*zoom)*a_ratio,(pan_vert+(line_found/5)*zoom)+zoom,WHITE);
	show_buffer();
	delay(120);	

	table[line_found] = colors[random(5)];
		draw();
		show_buffer();
		delay(60);
	table[line_found+1] = colors[random(5)];
			draw();
		show_buffer();
		delay(60);
	table[line_found+2] = colors[random(5)];
			draw();
		show_buffer();
		delay(60);
	table[line_found+3] = colors[random(5)];
			draw();
		show_buffer();
		delay(60);
	line_found = -1;
}

void replace_col(){

	rect_fast((pan_horiz+col_found%5*zoom)*a_ratio,pan_vert+(col_found>4?zoom:0), (pan_horiz+col_found%5*zoom+zoom)*a_ratio,pan_vert+4*zoom+(col_found>4?zoom:0),RED);		
	show_buffer();
	delay(2000);	

	table[col_found] = colors[random(5)];
			draw();
		show_buffer();
		delay(60);
	table[col_found+5] = colors[random(5)];
			draw();
		show_buffer();
		delay(60);
	table[col_found+10] = colors[random(5)];
			draw();
		show_buffer();
		delay(60);
	table[col_found+15] = colors[random(5)];
			draw();
		show_buffer();
		delay(60);
	col_found = -1;
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
	selected_cell = -1;
	
	
	
	//delay(150);
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

bool usr_input(){

					if(get_key_status(SCAN_Q) || get_key_status(SCAN_ESC)){
                        return false;
					}
					
					if( get_key_status(SCAN_ENTER) ){

						while(get_key_status(SCAN_ENTER)){}
							selected_cell = selector;
							
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
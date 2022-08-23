
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


#define MIN(a,b) (((a)<(b))?(a):(b))

#define KEY_DELAY 150
#define ON_COLOR 19
#define OFF_COLOR 42

int zoom = 34;
int mouse = 1;
float a_ratio = 1.2;
unsigned long int tstart,tend, fps_counter = 0, fps_count = 0;
int moves = 0;
int cell = 0;
int edit_mode = 0;
//char end_table[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1};
//char start_table[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char end_table[25] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
char start_table[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//char start_table[25] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
char table[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

char labels[25] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y'};


char null1[25] = {0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0};
char null2[25] = {1,0,1,0,1,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,1,0,1,0,1};

char adjacency[25][25] = {
		{1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1}
		};

char inverted[23][23]={
		{0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0},
		{1,1,0,1,1,0,1,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0},
		{1,0,1,1,1,1,0,1,1,0,0,0,1,1,0,1,1,1,1,1,0,1,0},
		{1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1},
		{0,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,0,1,0,1,1,1,0},
		{0,0,1,0,1,0,1,1,0,1,0,0,1,0,0,0,0,0,1,1,0,0,0},
		{0,1,0,1,0,1,1,0,1,1,0,0,0,1,0,1,1,1,0,0,0,1,0},
		{1,0,1,0,0,1,0,1,1,0,0,0,0,0,1,1,0,1,0,1,1,0,1},
		{0,0,1,0,0,0,1,1,1,0,1,0,0,1,1,1,0,0,1,0,0,1,1},
		{1,0,0,0,0,1,1,0,0,0,1,0,1,0,1,0,1,1,0,1,0,0,1},
		{0,0,0,0,1,0,0,0,1,1,0,0,1,0,1,1,1,1,1,0,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1},
		{0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,0,0,1,1,0},
		{1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0},
		{1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,1,0,1,0,0},
		{0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,0,1},
		{0,0,1,1,0,0,1,0,0,1,1,1,0,0,1,0,1,1,1,0,0,0,1},
		{0,0,1,0,1,0,1,1,0,1,1,0,1,0,0,1,1,0,1,1,1,0,0},
		{0,1,1,0,0,1,0,0,1,0,1,0,0,1,1,0,1,1,1,0,0,0,1},
		{1,0,1,0,1,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,1,0,1},
		{0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,0,1,1,1,0},
		{0,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1},
		{0,0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,1,0}
		};


char solution[25];		

char temptable[25];
char temptable2[25];

char solvetemp1[25];
char solvetemp2[25];
char solvetemp3[25];

int valid_solution = 0;

int check_solution(){
	int tally4=0;
	int i,j;
	
	memcpy(temptable2,temptable,25);

	for (i=0;i<25;i++){
		if (solution[i]==1)  {
			for (j=0;j<25;j++)  {
				if (adjacency[i][j]==1)   {
					temptable2[j]=(temptable2[j]+1)%2;
				}
			}
		}
	}
	
	for (i=0;i<25;i++) {
		if (temptable2[i]==1)  {
			tally4++;
		}
	}
	
	valid_solution = !tally4 ? 1 : 0;
	
	return valid_solution;
}
	
int shorten_solution(){
	int tally0=0;
	int tally1=0;
	int tally2=0;
	int tally3=0;
	int i;
	for (i=0;i<25;i++){
		solvetemp1[i]=(solution[i]+null1[i])%2;
		solvetemp2[i]=(solution[i]+null2[i])%2;
		solvetemp3[i]=(solution[i]+null1[i]+null2[i])%2;
		if (solution[i]==1) {tally0++;}
		if (solvetemp1[i]==1) {tally1++;}
		if (solvetemp2[i]==1) {tally2++;}
		if (solvetemp3[i]==1) {tally3++;}
	}
	if (tally3==MIN(MIN(MIN(tally0,tally1),tally2),tally3)){
		//for (i=0;i<25;i++)  {
		//		solution[i]=solvetemp3[i];
		//}
		memcpy(solution,solvetemp3,25);
	}
	if (tally2==MIN(MIN(MIN(tally0,tally1),tally2),tally3)){
		//for (i=0;i<25;i++)  {
		//	solution[i]=solvetemp2[i];
		//}
		memcpy(solution,solvetemp2,25);
	}
	
	if (tally1==MIN(MIN(MIN(tally0,tally1),tally2),tally3)){
		//for (i=0;i<25;i++) {
		//	solution[i]=solvetemp1[i];
		//}
		memcpy(solution,solvetemp1,25);
	}
	return check_solution();
}



int solve(char *tbl){
	int i,j;
	
	memcpy(temptable,tbl,25);
	
	for (i=0;i<23;i++){
		solution[i]=0;
		for (j=0;j<23;j++){
			solution[i]+=(inverted[i][j]*temptable[j]);
		} 
		solution[i]=solution[i]%2;
	}
	
	solution[23]=0;
	solution[24]=0;
	return shorten_solution();
	//return 1;
}
		

void invert_table(char *tbl){
	int i;
	for(i = 0;i<25;i++){
		tbl[i] = tbl[i] == 1 ? 0 : 1;
	}
}

void quit(){

	exit_graphic();
	delay(200);
	unhook_keyb_int();
	exit(1);
}


void initialize(void){
	if(mouse){
		if(!InstallMouse()){
            //printf("\n\n\t Mouse driver not loaded.\n");
            //exit(1);
			mouse = 0;
		}

		SetMousePosi(1,1);
	}
    initialize_graphics(true);
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
    init_chars_15();
	randomize();
}


unsigned long int get_msec(){
         struct dostime_t t;
        _dos_gettime(&t);
        //return t.hour * 3600 * 1000 + t.minute * 60 * 1000  + t.second * 1000  + t.hsecond;
        return  t.second * 1000  + t.hsecond;
 }

void show_winner(){
	int i;
	while(1){
		if(get_key_status(SCAN_ESC)){
			delay(KEY_DELAY);
			//for(i=0;i<25;i++)table[i] = 0;
			memcpy(table,start_table,25);
			moves = 0;
			return;
		}
		
		put_str(100,100, "WINNER!!!", WHITE, 0);
		show_buffer();
	}
	
}

void clear_screen(){
        rect_fill(0,0, 319, 199, 0);
		rect_fast(0,0,319,199, WHITE);
}

void draw(){
        int i,x,y,xm = 240,ym = 15;
		int sol_color,color,sel_color,end_color;
		
		int x_pan = 15*a_ratio;
		int y_pan = 15;
		
		char buf[20];
		
        clear_screen();
		
        x=x_pan; 
		y=y_pan;
		
        for (i= 0 ; i<25;i++){
			
			//color = table[i] ? ON_COLOR : OFF_COLOR;
			color = table[i] ? OFF_COLOR : ON_COLOR;
            rect_fill(x,y,x+zoom*a_ratio,y+zoom, color);
			rect_fast(x,y,x+zoom*a_ratio,y+zoom, WHITE);
				//put_char_15(x+3,y,labels[i],WHITE,0);
			put_ch(x+14*a_ratio,y+14,labels[i],WHITE,color);
			
			//plot_pixel(xm,ym,color);
			
			//sol_color = end_table[i] ? ON_COLOR : OFF_COLOR;
			sol_color = solution[i] ? GREEN : ON_COLOR;
			rect_fill(xm,ym,xm+a_ratio*5,ym+5, sol_color);
			rect_fast(xm,ym,xm+a_ratio*5,ym+5, WHITE);
			
			end_color = end_table[i] ? OFF_COLOR : ON_COLOR;
			rect_fill(xm,ym+40,xm+a_ratio*5,ym+40+5, end_color);
			rect_fast(xm,ym+40,xm+a_ratio*5,ym+40+5, WHITE);
			
			if(cell == i){
				sel_color = table[i] == 1 ? BLUE : RED;
				rect_fast(x+1,y+1,x+zoom*a_ratio-1,y+zoom-1, sel_color);
				rect_fast(x+2,y+2,x+zoom*a_ratio-2,y+zoom-2, sel_color);
			}
				
            if(i%5 == 4){
                y = y + zoom;
                x = x_pan;
				xm = 240;
				ym += 5;
            }else{
				xm +=5*a_ratio;
                x += zoom*a_ratio;
            }

        }
		
	itoa(moves,buf,10);
	//rect_fill(280,66, 320, 72, 0);	
	put_str(240, 90,"M: ",BLUE,0);
	put_str(258, 90,buf,BLUE,0);
	
	
	put_str(230, 180,(edit_mode ? "EDIT MODE":""),BLUE,0);
	
		
		
	if(mouse){
		RestrictMousePtr(0,0,318,197);
		GetMouseStatus();
		put_str_15(GetPosX(),GetPosY(),"m",WHITE,240);
	}
}

bool check_end_table(){
	int i = 0;
	for(;i<25;i++){
		if(end_table[i] != table[i])
			return 0;
	}
	return 1;
}

void reset_table(){
	int i;
	for(i=0;i<25;i++)
		table[i] = 0;
}


void light_cell(){
	table[cell] = table[cell] == 1 ? 0 : 1;
						
	if(cell%5 != 0){ //left
		table[cell-1] = table[cell-1] == 1 ? 0 : 1;
	}
				
	if(cell%5 != 4){//right
		table[cell+1] = table[cell+1] == 1 ? 0 : 1;
	}
				
	if(cell > 4){//top
		table[cell-5] = table[cell-5] == 1 ? 0 : 1;
	}
						
	if(cell < 20 ){//bottom
		if(cell+5 <= 24) table[cell+5] = table[cell+5] == 1 ? 0 : 1;
	}
	moves++;
}

void random_table(){
	int i;
	
	for(i=0;i<25;i++)
		end_table[i] = random(2);
		//memcpy(table,end_table,25);
		moves = 0;
		
		if(!solve(end_table)){
			random_table();
		}
		
		memset(start_table,0,25);
		memcpy(table,start_table,25);			
		//invert_table(solution);
}

int main(){
		
		int x,y,i;
		
        initialize();
		memcpy(table,start_table,25);
		
		solve(end_table);
//		invert_table(solution);
		
                while(1){
					
					
					    draw();
                        show_buffer();
					
					
                    if(get_key_status(SCAN_ESC)){
                        quit();
                    }else if(get_key_status(SCAN_LEFT_ARROW)){
                        if(cell > 0)
							cell--;
						delay(KEY_DELAY);
						continue;
                    }else if(get_key_status(SCAN_UP_ARROW)){
                        if(cell - 5 >= 0)
							cell -=5;
						delay(KEY_DELAY);
						continue;
                    }
					
					if(get_key_status(SCAN_RIGHT_ARROW)){
                        if(cell < 24)
							cell++;
						delay(KEY_DELAY);
						continue;
                    }
					
					if(get_key_status(SCAN_DOWN_ARROW)){
                        if(cell + 5 <= 24)
							cell +=5;
						delay(KEY_DELAY);
						continue;
                    }
					
					if(get_key_status(SCAN_DELETE)){
						//memset(table,0,25);
						memcpy(table,start_table,25);
						moves = 0;
						delay(KEY_DELAY);
					}
					
					if(get_key_status(SCAN_INSERT)){
						random_table();
						delay(KEY_DELAY);
					}
					
					if(get_key_status(SCAN_E)){
						if(edit_mode){
							edit_mode = 0;
							memcpy(start_table,table,25);
						}else{
							edit_mode = 1;
						}
						delay(KEY_DELAY);
					}
					
					if(mouse){
					
						GetMouseStatus();
						x = GetPosX();
						y = GetPosY();
						
						if(GetButton() == 1){
							if( x <= 198*a_ratio && y <= 198){
								cell = (word) x/a_ratio/zoom - 15/a_ratio/zoom + (word) (y-15)/zoom*5 + (word)15/zoom;
								
								if(edit_mode){
									table[cell] = table[cell] ? 0 : 1;
								}else{
									light_cell();
								}
								delay(KEY_DELAY);
							}
						}
					}
					
					if(get_key_status(SCAN_ENTER)){
						
						if(edit_mode){
							table[cell] = table[cell] ? 0 : 1;
						}else{
							light_cell();
						}
						delay(KEY_DELAY);
                    }

					if(!edit_mode && check_end_table()){
						draw();
						show_buffer();
						show_winner();
					}
                    
                }
}


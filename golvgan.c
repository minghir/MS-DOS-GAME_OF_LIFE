
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
//#include "LVGA\\flog.h"


#define MAX_ZOOM 5

int zoom = 5;
float a_ratio = 1;
unsigned long int tstart,tend, fps_counter = 0, population = 0, generation = 0, fps_count = 0;
int delay_msec = 200;
int grid = 1;

//int pan_up = 0;
//int pan_down = 0;
//int pan_left = 0;
//int pan_right = 0;

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
		rect_fast(0,0, 199*a_ratio, 199, WHITE);
		//rect_fast(200*a_ratio,0,310,199, WHITE);
}

void print_fps(){

	char fps_c[10];
	
	rect_fill(300,20, 320, 199, 0);	
	
	itoa(fps_count,fps_c,10);
	rect_fill(280,10, 320, 15, 0);	
	put_str_15(260,10,"FPS:",BLUE,0);
	put_str_15(280,10,fps_c,BLUE,0);
	
	itoa(delay_msec,fps_c,10);
	put_str_15(260, 18,"DELAY:",BLUE,0);
	put_str_15(284, 18,fps_c,BLUE,0);
	
	itoa(zoom,fps_c,10);
	put_str_15(260, 26,"ZOOM:",BLUE,0);
	put_str_15(280, 26,fps_c,BLUE,0);
	put_str_15(284, 26,"x",BLUE,0);
	
	itoa(population,fps_c,10);
	rect_fill(280,34, 320, 39, 0);	
	put_str_15(260, 34,"POP:",BLUE,0);
	put_str_15(280, 34,fps_c,BLUE,0);
	
	
	itoa(generation,fps_c,10);
	rect_fill(280,42, 320, 42, 0);	
	put_str_15(260, 42,"GEN:",BLUE,0);
	put_str_15(280, 42,fps_c,BLUE,0);
	
	
	itoa(pan_horiz,fps_c,10);
	rect_fill(280,50, 320, 58, 0);	
	put_str_15(260, 50,"HOR:",BLUE,0);
	put_str_15(280, 50,fps_c,BLUE,0);
	
	itoa(pan_vert,fps_c,10);
	rect_fill(280,58, 320, 66, 0);	
	put_str_15(260, 58,"VER:",BLUE,0);
	put_str_15(280, 58,fps_c,BLUE,0);
	

}

void clear_screen(){
	rect_fill(0,0, 199*a_ratio, 199, 0);	
}


void draw(char *firstgen, char *nextgen, int size){
	
	unsigned short p, k, max_count;
	
	int x,y,max_x,max_y,color;
//	int grid = 1;

	char tmp[200];
	
	clear_screen();
	print_fps();
	
	//rect_fill(0,0, 199*a_ratio, 199, 240);	
	rect_fast(0,0, 199*a_ratio, 199, WHITE);
	rect_fast(199*a_ratio,0,319,199, WHITE);
	//return;
	
	
	x=1;
	y=1;
	
	//sprintf(tmp,"%hu",max_count);
	//flog(tmp);
	//flog("teeeeeest");
	//put_str(210, 40,tmp,8,0);
	k = 0;
	//p = 0;// + pan_right - pan_left;
	/*
	for(x = 1; x <= 198;  x++){
		for(y=1;y<=198;y++){
			color =  nextgen[y*size+x] == 1 ? RED : 240;
			rect_fill(x,y, x+zoom*a_ratio, y+zoom, color);	
		}
	}
	
return;	
*/
	p = size*k  + pan_vert*size  + pan_horiz;

	while(p < size*size){
		
		
	if( x+zoom <= 198*a_ratio &&  y+zoom <= 198){// && nextgen[p]){
		//if( x+zoom <= 199*a_ratio &&  y+zoom <= 199 ){
		
			//color =  nextgen[p] == 1 ? RED : ( firstgen[p] == 1 ? YELLOW : 240 );
			
			color =  nextgen[p] == 1 ? RED : 240;
			
			//color = RED;
			
		
			if(zoom > 1){
				
				
				if(grid){
					rect_fast(x,y, x+zoom*a_ratio, y+zoom, GREEN );	
					rect_fill(x+1,y+1, x+zoom*a_ratio-1, y+zoom-1, color);	
				}else{
					rect_fill(x,y, x+zoom*a_ratio, y+zoom, color);	
				}
				
				//if(grid == 1) 
				
				
			}else{
				plot_pixel(x,y,color);
			}
		}
				
		if(p%size == size-1 ){//} || x >= 198*a_ratio){
			
			y += zoom;
			x = 1;
			
			k++;
			//p= size*k + pan_vert*size  + pan_horiz;
			p = size*k  + pan_vert*size  + pan_horiz;
			
			

		}else{
			x += zoom*a_ratio;
			//p = p + pan_right*size + 1;// - pan_left + 1;
			
			//x += zoom;
			p++;
		}
		
		//p = p*zoom >= 198*k ? k*size : p++;;
		//p++;
		
		
	}
}



int main(){

		int size = 25;
		
		unsigned short sizeoftable, i, j;
		int tstart_upd,tend_upd;
		unsigned long int max_gen = 1000000;
		
		char *table;
		char *nextgen;
		
		char tmp[100];
		
		int n_top = 0;
		int n_bottom = 0;
		int n_left = 0;
		int n_right = 0;
		int n_topleft = 0;
		int n_topright = 0;
		int	n_bottomleft = 0;
		int n_bottomright = 0;
		int live_neighbours = 0;
		
ini_t *config = ini_load("golvga.ini");
ini_sget(config, "game", "speed", "%d", &delay_msec);
ini_sget(config, "game", "table_size", "%d", &size);
ini_sget(config, "game", "max_gen", "%llu", &max_gen);
ini_free(config);
		
		sizeoftable = size*size;
		
		table = (char *) malloc(sizeoftable);
		nextgen = (char *) malloc(sizeoftable);
		
		
        initialize();
		randomize(); 
		
		/*
		for(i = 0;i<sizeoftable;i++){
			//table[i] = 0;//random(2);
			table[i] = random(2);
			nextgen[i] = 0;
		}
		*/
		
		//memset(table,0,size*size);
		memset(table,random(2),sizeoftable);
		memcpy(table,nextgen,sizeoftable);	
		
		/*
		table[1] = 1;
		table[12] = 1;
		table[20] = 1;
		table[21] = 1;
		table[22] = 1;
		*/
		
		
		
		table[0] = 1;
		table[1] = 1;
		table[38] = 1;
		table[39] = 1;
		
		table[40] = 1;
		table[41] = 1;
		table[78] = 1;
		table[79] = 1;
		
		/*
		table[44] = 1;
		table[54] = 1;
		table[64] = 1;
		
		
		
		table[80] = 1;
		table[90] = 1;
		table[81] = 1;
		table[91] = 1;
		
		table[89] = 1;
		table[99] = 1;
		table[88] = 1;
		table[98] = 1;
		//table[22] = 1;
		*/
		
		draw(table, table, size);
		show_buffer();
		
		
		tstart_upd = tstart = get_msec();
		j=0;
		fps_counter = 0;
        while(1){
			
			for(i = 0;i<sizeoftable;i++){
				
				n_top = table[( i - size )];
				n_bottom = table[( i + size )];
				
				n_left = table[(i - 1)];
				n_right = table[(i + 1)];
				
				n_topleft = table[( i - size - 1)];
				n_topright = table[( i - size + 1)];
				
				n_bottomleft = table[( i + size - 1)];
				n_bottomright = table[( i + size + 1)];
				
				
				if(i%size == 0){ //left
					n_left = 0;
					n_topleft = 0;
					n_bottomleft = 0;
				}
				
				if(i%size == size-1){//right
					n_right = 0;
					n_topright = 0;
					n_bottomright = 0;
				}
				
				//if(i>=0 && i<=size-1){//top
				if(i<=size-1){//top
					n_top = 0;
					n_topright = 0;
					n_topleft = 0;
				}
				
				if(i >= sizeoftable - size && i< sizeoftable){//bottom
					n_bottom = 0;
					n_bottomleft = 0;
					n_bottomright = 0;
				}
				
				live_neighbours =  n_top + n_bottom + n_left + n_right + n_topleft + n_topright + n_bottomleft + n_bottomright;
			
				nextgen[i] = table[i] == 1 && live_neighbours == 2 || live_neighbours == 3  ? 1 : 0;
				population += nextgen[i];
				
			}
	
			
			//draw(table, nextgen, size);
			//show_buffer();
			//memcpy(table,nextgen,sizeoftable);	
			
			//delay(msec);	
			
			//memcpy(table,nextgen,sizeoftable);
			
			fps_counter++;
			
			j++;
			generation++;
			generation = max_gen == generation ? 0 : generation;
			tend_upd = tend = get_msec();
			
			if( (tend - tstart) >= 10 ){
				
				if(get_key_status(a_Key)){
					
					if( zoom < MAX_ZOOM ){
						zoom +=  1;
					}else{
						zoom = zoom;
					}
				
				}
				
				if(get_key_status(e_Key)){
					
					if(zoom > 1){
						zoom -= 1;
					}else{
						zoom = zoom;
					}
					
					
				}
				
				
				if(get_key_status(d_Key)){
					
					//if( (pan_horiz + zoom)*a_ratio >= size*zoom*a_ratio + 10  - 199*a_ratio )
						if( (pan_horiz * zoom)*a_ratio >= (size*zoom+2*zoom)*a_ratio - 199*a_ratio )
					//	if( pan_horiz + zoom  >= size*zoom  - 199*a_ratio/zoom )
						pan_horiz = pan_horiz;
					else
						pan_horiz += zoom;
					//	   delay_msec = delay_msec < 5000 ? delay_msec + 50 : delay_msec;
				}
					
				if(get_key_status(s_Key)){
					//	   delay_msec = delay_msec > 0 ? delay_msec - 50 : delay_msec;

					if( pan_horiz - zoom <= 0 )
						pan_horiz = 0;
					else	
						pan_horiz -= zoom;
					
				}
				
				 if(get_key_status(o_Key)){
					 
					 if(pan_vert - zoom <= 0)
						 pan_vert = 0;
					 else
						pan_vert -= zoom;
				 }
				 
				if(get_key_status(l_Key)){
					 
					 if(pan_vert*zoom  >= size*zoom +10 - 199 )
						 pan_vert = pan_vert;
					 else
						pan_vert += zoom;
				}
				 
				 
				if(get_key_status(q_Key)){ //exit program on 'q' pressed
				
					
					//flog("q pressed");
						exit_graphic();
					//flog("after exit graphic");
					delay(200);
						unhook_keyb_int();
					//flog("after exit key");
					delay(200);
						break;
				}
				 
			}
			
			
			if( (tend - tstart) >= 1000 ){
				print_fps();
				fps_count = fps_counter;
				fps_counter = 0;
				tstart = get_msec();
			}
			
				
			
			if( (tend_upd - tstart_upd) >= delay_msec || !delay_msec ){
				draw(table, nextgen, size);
				show_buffer();
				memcpy(table,nextgen,sizeoftable);	
				population = 0;
				tstart_upd = get_msec();
			}

    }
	
	delay(500);	
	free(table);
	free(nextgen);
	//flog("after free table");
	//flog_stop();
    return 0;
	
}


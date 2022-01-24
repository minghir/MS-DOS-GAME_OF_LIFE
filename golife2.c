
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
#include "LVGA\\mouse.h"


#define MAX_ZOOM 5
#define KEY_DELAY 150



int zoom = 5;
//int zoom = 1;
float a_ratio = 1.2;
//unsigned short int tstart,tend, fps_counter = 0, population = 0, generation = 0, fps_count = 0, cel=0;
word tstart,tend, fps_counter = 0, population = 0, generation = 0, fps_count = 0, cel=0;
int delay_msec = 200;
int grid = 1;
int paused = 1;
int size = 25;


int pan_vert = 0;
int pan_horiz = 0;


char *table;
char *nextgen;

void initialize(void)
{
	
	if(!InstallMouse()){
            printf("\n\n\t Mouse driver not loaded.\n");
            exit(1);
    }
//	RestrictMousePtr(2,2,318,198);
	SetMousePosi(1,1);
	
	initialize_graphics(true);
   	ctrlbrk(ctrlbrk_handler);
   	hook_keyb_int();
	init_chars_15();
}


//unsigned short int get_msec(){
word get_msec(){
	 struct dostime_t t;
	_dos_gettime(&t);
	//return t.hour * 3600 * 1000 + t.minute * 60 * 1000  + t.second * 1000  + t.hsecond;
	return  t.second * 1000  + t.hsecond;
 }



void print_fps(){

	char fps_c[20];
	word size_of_v_bar , size_of_h_bar ;
	int x,y;
	
	rect_fill(199*a_ratio+1,1, 318, 198, 0);
	
	rect_fast(0,0, 199*a_ratio, 199, WHITE);
	rect_fast(199*a_ratio,0,319,199, WHITE);
	
	
	//rect_fill(300,20, 320, 199, 0);	
	
	itoa(fps_count,fps_c,10);
	//rect_fill(280,10, 320, 15, 0);	
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
	//rect_fill(280,34, 320, 39, 0);	
	put_str_15(260, 34,"POP:",BLUE,0);
	put_str_15(280, 34,fps_c,BLUE,0);
	
	
	itoa(generation,fps_c,10);
	//rect_fill(280,42, 320, 42, 0);	
	put_str_15(260, 42,"GEN:",BLUE,0);
	put_str_15(280, 42,fps_c,BLUE,0);
	
	
	itoa(pan_horiz,fps_c,10);
	//rect_fill(280,50, 320, 58, 0);	
	put_str_15(260, 50,"HOR:",BLUE,0);
	put_str_15(280, 50,fps_c,BLUE,0);
	
	itoa(pan_vert,fps_c,10);
	//rect_fill(280,58, 320, 66, 0);	
	put_str_15(260, 58,"VER:",BLUE,0);
	put_str_15(280, 58,fps_c,BLUE,0);
	
	itoa(GetPosX(),fps_c,10);
	//rect_fill(280,66, 320, 72, 0);	
	put_str_15(260, 66,"MX:",BLUE,0);
	put_str_15(280, 66,fps_c,BLUE,0);
	
	itoa(GetPosY(),fps_c,10);
	//rect_fill(280,72, 320, 80, 0);	
	put_str_15(260, 72,"MY:",BLUE,0);
	put_str_15(280, 72,fps_c,BLUE,0);
	
	//itoa(((int)GetPosX()/a_ratio/zoom + (int)GetPosY()/zoom*10),fps_c,10);
	
	//itoa((int)GetPosX()/a_ratio/zoom + (int)GetPosY()/zoom*size,fps_c,10);
	x = GetPosX();
	y = GetPosY();
				
	if( x <= 198*a_ratio && y <= 198){
		cel = x/a_ratio/zoom + pan_horiz + y/zoom*size+pan_vert*size;
	}
	
	//itoa(cel,fps_c,10);
    sprintf(fps_c,"%u",( cel > size * size ? 0 : cel + 1 ) );
	//ultoa(cel,fps_c,10);
	//rect_fill(280,80, 320, 88, 0);	
	put_str_15(260, 80,"CEL:",BLUE,0);
	put_str_15(280, 80,fps_c,BLUE,0);
	
	//rect_fill(270,190, 320, 200, 0);	
	put_str_15(270, 190,(paused ? "PAUSED":""),BLUE,0);
	
	
	
	
	//size_of_h_bar = ( (198*a_ratio)/size ) * size - ( size - 198*a_ratio/zoom + 8 ) - 2 ;
	size_of_h_bar =  198*a_ratio - ( size - 198*a_ratio/zoom + 9 )  ;
	if(size_of_h_bar <= 197*a_ratio){
		itoa(size_of_h_bar,fps_c,10);
		//rect_fill(280,72, 320, 80, 0);	
		put_str_15(260, 88,"HBAR:",BLUE,0);
		put_str_15(280, 88,fps_c,BLUE,0);
		
		size_of_h_bar = size_of_h_bar >= 198*a_ratio ? 198*a_ratio : size_of_h_bar;
		rect_fill(1 + pan_horiz,197, pan_horiz + size_of_h_bar , 198, WHITE);	
	}
	

	
	//size_of_v_bar = 198*a_ratio - ( size - 198*a_ratio/zoom  + 13) - 2 ;
	//
	size_of_v_bar =  ( 198*a_ratio - ( size - 198*a_ratio/zoom  + 7)  ) /a_ratio;
	if(size_of_v_bar <= 198 ){
		size_of_v_bar = size_of_v_bar >= 198 ? 198 : size_of_v_bar;
		
		itoa(size_of_v_bar,fps_c,10);
		//rect_fill(280,72, 320, 80, 0);	
		put_str_15(260, 96,"VBAR:",BLUE,0);
		put_str_15(280, 96,fps_c,BLUE,0);
		
		rect_fill(197*a_ratio, 1 + pan_vert/a_ratio , 198*a_ratio, pan_vert/a_ratio + size_of_v_bar , WHITE);	
	}
	//rect_fast(200*a_ratio,0,319,200, WHITE);
	
}



void draw( char *nextgen, int size){
	
	word p, k, max_count;
	
	int x,y,max_x,max_y,color,m_x,m_y;
//	int grid = 1;

	char tmp[200];
	
	//rect_fill(0,0, 199*a_ratio, 199, 0);
	
/*
	x=1;
	y=1;
	

	k = 0;
	
	p = size*k  + pan_vert*size  + pan_horiz;

	while(p < size*size){
		
		
	if( x+zoom <= 198*a_ratio &&  y+zoom <= 198){// && nextgen[p]){
		
			
			color =  nextgen[p] == 1 ? RED : 240;
		
			if(zoom > 1){
				
				
				if(grid){
					rect_fast(x,y, x+zoom*a_ratio, y+zoom, 0 );	
					rect_fill(x+1,y+1, x+zoom*a_ratio-1, y+zoom-1, color);	
					
				}else{
					rect_fill(x,y, x+zoom*a_ratio, y+zoom, color);	
				}
				
				
				
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
			p++;
		}
		
	}
*/	
	print_fps();
	
	//m_x = GetPosX() > 318 ? 318 : GetPosX();
	//m_y = GetPosY() > 197 ? 197 : GetPosY();
	//SetMousePosi(m_x,m_y);
	RestrictMousePtr(0,0,318,197);
	put_str_15(GetPosX(),GetPosY(),"m",WHITE,240);
	//put_str_15(m_x,m_y ,"m",WHITE,240);
	
}


void on_usr_input(){
		word i;
		int x,y;
	
		/*
			if(get_key_status(a_Key)){
					
					if( zoom < MAX_ZOOM ){
						//pan_horiz = 0;
						//pan_vert = 0;
						zoom +=  1;
						delay(KEY_DELAY);
					}else{
						zoom = zoom;
					}
					
				}
				
				if(get_key_status(e_Key)){
					
					if(zoom > 1){
						//pan_horiz =0;
						//pan_vert = 0;
						zoom -= 1;
						delay(KEY_DELAY);
					}else{
						zoom = zoom;
					}
				}
		*/
				
				if(get_key_status(n_Key)){
					memcpy(table,nextgen,size * size);	
					generation++;
					delay(KEY_DELAY);
				}
				
				
				if(get_key_status(home_Key)){
					pan_horiz = 0;
					pan_vert = 0;
				}
				
				
				if(get_key_status(end_Key)){
					pan_horiz = size - 198*a_ratio/zoom + 9;
					pan_vert =  size - 198/zoom;
				}
				
				if(get_key_status(right_Key)){
						//if( (pan_horiz * zoom)*a_ratio +zoom*a_ratio >= ( (size*zoom)*a_ratio  - 199*a_ratio ) + zoom*a_ratio )
					if(pan_horiz >= size - 198*a_ratio/zoom + 8 )	
						pan_horiz = pan_horiz;
					else
						pan_horiz += 1;
					//	   delay_msec = delay_msec < 5000 ? delay_msec + 50 : delay_msec;
				}
				
				if(get_key_status(down_Key)){
					 if(pan_vert >= size - 198/zoom  )
						 pan_vert = pan_vert;
					 else
						pan_vert += 1;
				}
					
				if(get_key_status(left_Key)){
					//	   delay_msec = delay_msec > 0 ? delay_msec - 50 : delay_msec;

					if( pan_horiz  <= 0 )
						pan_horiz = 0;
					else	
						pan_horiz -= 1;
					
				}
				
				 if(get_key_status(up_Key)){
					 
					 if(pan_vert  <= 0)
						 pan_vert = 0;
					 else
						pan_vert -= 1;
				 }
				
				
				if(get_key_status(space_Key)){
					paused = paused == 1 ? 0 : 1;
					delay(KEY_DELAY);
				}
				
				if(get_key_status(del_Key)){
					
					memset(table,0,size*size);
					//memset(nextgen,0,size*size);
					generation = 0;
					paused = 1;
					/*
					for(i = 0; i < size * size; i++){
						table[i] = 0;
						nextgen[i] = 0;
					}
					*/
					delay(KEY_DELAY);
				}
				
				if(get_key_status(ins_Key)){
					for(i = 0; i < size * size; i++){
						table[i] = random(2);
						nextgen[i] = random(2);
						generation = 0;
						//table[i] = random(2);
						//nextgen[i] = random(2);
					}
					
					//memcpy(nextgen,memcpy(table,nextgen,	,size*size);	
					delay(KEY_DELAY);
				}
				
				
								
				 
				if(get_key_status(q_Key)){ //exit program on 'q' pressed
					//delay(250);
					exit_graphic();
					delay(200);
					unhook_keyb_int();
					delay(500);	
					free(table);
					free(nextgen);
					exit(0);
				}
				
				GetMouseStatus();
				x = GetPosX();
				y = GetPosY();
				
				if(GetButton() == 1 && x <= 198*a_ratio && y <= 198){
					//cel = (unsigned short int) x/a_ratio/zoom + pan_horiz + (unsigned short int) y/zoom*size + (unsigned short int)  pan_vert*size;
					cel = (word) x/a_ratio/zoom + pan_horiz + (word) y/zoom*size + (word)  pan_vert*size;
						table[cel] =  1;
						nextgen[cel] =  1;
					//memcpy(nextgen,table,sizeoftable);	
				}
				
				if(GetButton() == 2 && x <= 198*a_ratio && y <= 198){
					cel = (word) x/a_ratio/zoom + pan_horiz + (word) y/zoom*size + (word)  pan_vert*size;
					//cel = (unsigned short int) x/a_ratio/zoom + pan_horiz + (unsigned short int) y/zoom*size + (unsigned short int)  pan_vert*size;
					//cel = x/a_ratio/zoom + pan_horiz + y/zoom*size+pan_vert*size;
					table[cel] =  0;
					nextgen[cel] =  0;
					//memcpy(nextgen,table,sizeoftable);	
				}
}


int main(){

		
		
		word sizeoftable, i, popcount=0;
		int tstart_upd,tend_upd;
		word max_gen = 1000000;
		
		word p, k, max_count;
	
		int x,y,color;
		
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
ini_sget(config, "game", "delay", "%d", &delay_msec);
ini_sget(config, "game", "table_size", "%d", &size);
ini_sget(config, "game", "max_gen", "%llu", &max_gen);
ini_free(config);
		
		sizeoftable = size*size;
		
		table = (char *) malloc(sizeoftable);
		nextgen = (char *) malloc(sizeoftable);
		
		
        initialize();
		randomize(); 
		
		for(i = 0;i<sizeoftable;i++){
			table[i] = 0;//random(2);
			//table[i] = random(2);
			nextgen[i] = 0;
		}
		
		/*
		table[1] = 1;
		table[12] = 1;
		table[20] = 1;
		table[21] = 1;
		table[22] = 1;
		
		
		
		
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
		
		draw(table, size);
		show_buffer();
		
		
		tstart_upd = tstart = get_msec();
		
		fps_counter = 0;
        while(1){
			
			popcount=0;
			
			x=1;
			y=1;
			k = 0;
		 
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
					popcount += table[i];
					
					///////////////////
					if( x+zoom <= 198*a_ratio &&  y+zoom <= 198){
						
						//color =  nextgen[i] == 1 ? RED : 240;
						color =  table[i] == 1 ? RED : 240;
						
						rect_fast(x,y, x+zoom*a_ratio, y+zoom, 0 );	
						rect_fill(x+1,y+1, x+zoom*a_ratio-1, y+zoom-1, color);	
						
					}
					
					if(i%size == size-1 ){//} || x >= 198*a_ratio){
			
						y += zoom;
						x = 1;
						
						//k++;
						//p= size*k + pan_vert*size  + pan_horiz;
						//p = size*k  + pan_vert*size  + pan_horiz;
						
						

					}else{
						x += zoom*a_ratio;
						//p++;
					}
					
					////////////////////////////
					
			}	
				
	
			population = popcount;
			
			
			fps_counter++;
			
			
			
			tend_upd = tend = get_msec();

			
			if( (tend - tstart) >= 1000 ){
				print_fps();
				fps_count = fps_counter;
				fps_counter = 0;
				tstart = get_msec();
			}
			
				
			on_usr_input();		
			
			if( (tend_upd - tstart_upd) >= delay_msec ){
				
				if(!paused){
					memcpy(table,nextgen,sizeoftable);	
					generation++;
				}
				
				
				tstart_upd = get_msec();
				
				
			}else{
				//draw(table, table, size);
			}
			
				
				
				if(!paused)
					draw(nextgen, size);
				else
					draw(table, size);
				
				
				
				
				//ShowMouse();
				//put_str_15(GetPosX(), GetPosY(),"m",WHITE,240);
				show_buffer();

    }
	
}

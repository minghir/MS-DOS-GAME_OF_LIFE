
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

void initialize(void)
{
	initialize_graphics(true);
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
}



void draw_grid(){
		
}


void draw(){
	int p,j,i;
	int size = 100;
	
	Circle(100,100,20,YELLOW);
	/*
	for (i = 0; i<320;i++){
		for ( j = 0;j<200;j++){
//			plot_pixel(i,j,2);
			Circle(i,j,20,8);
		}
	}
	
	
	j=0;
	i=0;
	
	
	for(p = 0;p<size;p++){
		if(p%size == size-1){
			
			j++;
			i = 0;
		}else{
			i++;
		}
		
		
		//plot_pixel(i,j,8);
		
	}
	*/
}



int main(){

        initialize();

        while(1){

			if(get_key_status(q_Key)){ //exit program on 'q' pressed
				exit_graphic();
				unhook_keyb_int();
	           return 0;
            }

            if(get_key_status(d_Key)){

            }
			
			if(get_key_status(s_Key)){

            }

			draw();
			show_buffer();
    }
}


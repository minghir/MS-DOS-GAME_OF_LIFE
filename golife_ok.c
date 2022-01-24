
#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <math.h>

//#include "LVGA\\lvga.h"
#include "LVGA\\keyb.h"

void initialize(void)
{
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
}





int main(){

		int size = 10;
		int sizeoftable = 100;
		char table[100];
		char nextgen[100];
		char neighbours[100];
		int i,j,k,p,r;
		
		int top = 0, n_top = 0;
		int bottom = 0, n_bottom = 0;
		int left = 0, n_left = 0;
		int right = 0, n_right = 0;
		int topleft = 0, n_topleft = 0;
		int topright = 0, n_topright = 0;
		int	bottomleft = 0, n_bottomleft = 0;
		int bottomright = 0, n_bottomright = 0;
		int live_neighbours = 0;
		
		
		char rr;
		
        initialize();

		
		randomize(); 
		
		for(i = 0;i<100;i++){
			table[i] = 0;//random(2);
			//table[i] = random(2);
			nextgen[i] = 0;
			neighbours[i]=0;
		}
		
		
		
		/*
		table[1] = 1;
		table[2] = 1;
		table[3] = 1;
		table[4] = 1;
		table[5] = 1;
		table[6] = 1;
		table[7] = 1;
		table[8] = 1;
		table[9] = 1;
		table[10] = 1;
		//table[11] = 1;
		//table[12] = 1;
		//table[21] = 1;
		*/
		/*
		table[8] = 1;
		table[9] = 1;
		table[18] = 1;
		table[19] = 1;
		
		table[81] = 1;
		table[82] = 1;
		table[91] = 1;
		table[92] = 1;
		
		//table[78] = 1;
		table[1] = 1;
		table[2] = 1;
		table[11] = 1;
		table[12] = 1;
		
		table[9] = 1;
		table[10] = 1;
		table[19] = 1;
		table[20] = 1;
		
		table[81] = 1;
		table[82] = 1;
		table[91] = 1;
		table[92] = 1;
		
		
		table[89] = 1;
		table[90] = 1;
		table[99] = 1;
		table[100] = 1;
		*/
		
		
		table[78] = 1;
		table[79] = 1;
		table[80] = 1;
		table[88] = 1;
		table[99] = 1;
		/*
		table[96] = 1;
		table[97] = 1;
		table[98] = 1;
		table[99] = 1;
		table[100] = 1;
		*/
		
		clrscr();
		for(p = 0;p<100;p++){
				if(p%size == 0){
					printf(" %d\n",table[p]);
				}
				else{
					printf(" %d",table[p] );
				}
			}
			
			printf("\n");
		for(p = 0;p<=sizeoftable;p++){
				if(p%size == 0){
					printf(" %d\n",neighbours[p]);
				}
				else{
					printf(" %d",neighbours[p]);
				}
			}	
			
			delay(300);	
		
			
		
        while(1){
//		for(j = 1 ; j<= 10;j++){


			if(get_key_status(q_Key)){
				unhook_keyb_int();
                return 0;
            }

            if(get_key_status(d_Key)){
                                
            }

			
			
			
			//for(i = 1;i<=sizeoftable;i++){
				i = 0;
				while(i<100){
				
				
				n_top = n_bottom = n_left = n_right = n_topleft = n_topright = n_bottomleft  = n_bottomright = live_neighbours  = 0;
				
				n_top = table[( i - size )];
				n_bottom = table[( i + size )];
				
				n_left = table[(i - 1)];
				n_right = table[(i + 1)];
				
				n_topleft = table[( i - size - 1)];
				n_topright = table[( i - size + 1)];
				
				n_bottomleft = table[( i + size - 1)];
				n_bottomright = table[( i + size + 1)];
				
				
				if(i%size == 0){ //left
				if(i==90)printf("sunt la stanga\n");
					n_left = 0;
					n_topleft = 0;
					n_bottomleft = 0;
				}
				
				if(i%size == size-1){//right
				if(i==90)printf("sunt la dreapta\n");
					n_right = 0;
					n_topright = 0;
					n_bottomright = 0;
				}
				
				if(i>=0 && i<=size){//top
				if(i==90)printf("sunt la sus\n");
					n_top = 0;
					n_topright = 0;
					n_topleft = 0;
				}
				
				if(i > sizeoftable - size && i<= sizeoftable){//bottom
					if(i==90)printf("sunt la jos\n");
					n_bottom = 0;
					n_bottomleft = 0;
					n_bottomright = 0;
				}
				
				live_neighbours =  n_top + n_bottom + n_left + n_right + n_topleft + n_topright + n_bottomleft + n_bottomright;
				/*
				if(i == 90){
					//delay(3000);
					printf("\n");
					printf("%d %d %d\n",n_topleft,n_top,n_topright);
					printf("%d %d %d\n",n_left,i,n_right);
					printf("%d %d %d\n",n_bottomleft,n_bottom,n_bottomright);
					printf("\n%d\n",live_neighbours);
					printf("%d:%d\n",i + size,table[( i + size )]);
					delay(3000);
				}
				*/
				neighbours[i] = live_neighbours;
			
				
				switch(live_neighbours){
					case 0:
						nextgen[i] = 0;
					break;
					
					case 1:
						nextgen[i] = 0;
					break;
					
					case 2:
						nextgen[i] = table[i] == 1 ? 1 : 0;
						//nextgen[i] = table[i];
					break;
					
					case 3:
						nextgen[i] = table[i] == 1 ? 1 : 1;
						//nextgen[i] = 1;
					break;
					
					default:	
						nextgen[i] = 0;
					break;
				}
				
				i++;
			}
			
			
			

			clrscr();
			for(p = 0;p<sizeoftable;p++){
				if(p%size == size-1){
					printf(" %d\n",nextgen[p]);
				}
				else{
					printf(" %d",nextgen[p] );
				}
			}
		
			
			printf("\n");
			for(p = 0;p<=sizeoftable;p++){
				if(p%size == size-1){
					printf(" %d\n",neighbours[p]);
				}
				else{
					printf(" %d",neighbours[p]);
				}
			}

			
			delay(300);	
			
			memcpy(&table,&nextgen,sizeof(nextgen));
		
			
    }
}


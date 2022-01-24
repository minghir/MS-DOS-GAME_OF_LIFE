
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
		int table[100];
		int nextgen[100];
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
		}
		
		table[1] = 1;
		table[2] = 1;
		table[11] = 1;
		table[12] = 1;
		
		
		table[9] = 1;
		table[10] = 1;
		table[19] = 1;
		table[20] = 1;
		
		table[91] = 1;
		table[92] = 1;
		table[81] = 1;
		table[82] = 1;
		
		table[89] = 1;
		table[90] = 1;
		table[99] = 1;
		table[100] = 1;
		
        while(1){
//		for(j = 1 ; j<= 10;j++){


			if(get_key_status(q_Key)){
				unhook_keyb_int();
                return 0;
            }

            if(get_key_status(d_Key)){
                                
            }

			
			for(i = 0;i<sizeoftable;i++){
				
			
				
			
				top = i>=0 && i<size ? 0 : 1;
				bottom = i > sizeoftable - size && i< sizeoftable ? 0 : 1;
				left = i%size == 0 ? 0 : 1; 
				right = i%size == 1 ? 0 : 1; 
				
				topleft =  top + left == 2  ? 1 : 0;
				topright =  top + right  == 2  ? 1 : 0;
				bottomleft = bottom + left == 2 ? 1 : 0;
				bottomright  = bottom + right == 2 ? 1 : 0;
				
				n_top = top == 1 ? table[( i - size )] : 0 ;
				n_bottom = bottom == 1 ? table[( i + size )] : 0 ;
				n_left = left == 1 ? table[(i - 1)]  : 0 ;
				n_right = right == 1 ? table[(i + 1)] : 0 ;
				
				n_topleft = topleft == 1 ? table[( i - size - 1)] : 0 ;
				n_topright = topright == 1 ? table[( i - size + 1)] : 0;
				n_bottomleft = bottomleft == 1 ? table[( i + size - 1)] : 0;
				n_bottomright = bottomright == 1 ? table[( i + size + 1)] : 0;
				
				live_neighbours =  n_top + n_bottom + n_left + n_right + n_topleft + n_topright + n_bottomleft + n_bottomright;
				
				
		
				
				
				if(i==36){
					printf("Table[%d]=:%d\n",i,table[i]);
					printf(" %d %d %d\n",topleft, top, topright );
					printf(" %d %d %d\n",left, 1, right );
					printf(" %d %d %d\n",bottomleft, bottom, bottomright );
					printf("aa:(%d)\n",live_neighbours);
					
					printf(" %d %d %d\n",n_topleft,n_top,n_topright );
					printf(" %d %d %d\n",n_left,table[i], n_right );
					printf(" %d %d %d\n",n_bottomleft, n_bottom, n_bottomright );
					printf("aa:(%d)|%d\n",( i + size + 1),table[( i + size + 1)]);
					
					printf(" %d %d %d\n",table[22],table[23],table[24] );
					printf(" %d %d %d\n",table[28],table[29], table[30] );
					printf(" %d %d %d\n",table[24],table[35],table[36]);
					printf("aa:(%d)|%d\n",( i + size + 1),table[( i + size + 1)]);
					
					for(k = 1;k<=sizeoftable;k++){
							if(k%size == 0){
								printf(" %d\n",table[k]);
							}
							else{
								printf(" %d",table[k]);
							}
						}
					
				}				
				
				
		
				
				
				
				switch(live_neighbours){
					case 0:
						nextgen[i] = 0;
					break;
					
					case 1:
						nextgen[i] = 0;
					break;
					
					case 2:
						nextgen[i] = table[i] == 1 ? 1 : 0;
					break;
					
					case 3:
						nextgen[i] = table[i] == 1 ? 1 : 1;
					break;
					
					default:	
						nextgen[i] = 0;
					break;
				}
				
				
			}
			
			
			

			clrscr();
			
			/*
			for(p = 1;p<=sizeoftable;p++){
				if(p%size == 0){
					printf(" %d\n",table[p]);
				}
				else{
					printf(" %d",table[p]);
				}
			}
			printf("\n\n");
			*/
			
			for(p = 0;p<sizeoftable;p++){
				if(p%size == 1){
					printf(" %c\n",(nextgen[p] == 0? ' ': 223));
				}
				else{
					printf(" %c",(nextgen[p] == 0? ' ':223));
				}
			}

			
			delay(500);	
			
			memcpy(&table,&nextgen,sizeof(nextgen));
			/*
			for(r = 1; r<=sizeoftable; r++){
				table[r] = nextgen[r];
			}
			*/
			
    }
}


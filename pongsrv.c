
#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <math.h>

#include "LVGA\\lvga.h"
#include "LVGA\\keyb.h"
#include "LVGA\\serial.h"
#include "msg.h"

#define GO_UP 1
#define GO_DOWN 0
#define GO_RIGHT 1
#define GO_LEFT 0

#define BALL_RADIUS 4
#define PL_BAR_X_POS 2
#define PL_BAR_SIZE_HEIGHT 20
#define PL_BAR_SIZE_WIDTH 2
#define PL1_COLOR 49
#define PL2_COLOR 60

int x,y,direction_y,direction_x, page, pl1_score, pl2_score, ball_color;

int last_ball_pos_x[2];
int last_ball_pos_y[2];
int last_pl1_pos_y[2];
int last_pl2_pos_y[2];

// serial vars
char* errors[] =
{
    "Successful",
    "Unknown error",
    "Port not open",
    "Port already open",
    "No UART found on that comport",
    "Invalid comport",
    "Invalid BPS",
    "Invalid data bits",
    "Invalid parity",
    "Invalid stop bits",
    "Invalid handshaking",
    "Invalid fifo threshold",
    "Passed in a NULL pointer",
    "",
    "",
    ""
};

char* handshaking[] =
{
    "none",
    "xon/xoff",
    "rts/cts",
    "dtr/dsr",
    "",
    ""
};


	int len;
    char buff[100];
	char buff2[100],old_buff2[100];
	char old_buff[100];
    char ch;
    int rc;
    int com = COM_1;
//serial vars	




void draw_table(){
        rect_fast(0,0, SCREEN_WIDTH - 1 , SCREEN_HEIGHT - 1, 8 );
}

void print_score(){
        char p1s[10],p2s[10];
        //      sprintf(s,"%d",ball_delay);
        draw_table();
        sprintf(p1s,"%d",pl1_score);
        sprintf(p2s,"%d",pl2_score);

        //settextjustify(CENTER_TEXT,CENTER_TEXT);
        //setfillstyle(SOLID_FILL,BLACK);


        //bar( getmaxx()/5*2-10,90,getmaxx()/5*2+10,110);
        //bar( getmaxx()/5*3-10,90,getmaxx()/5*3+10,110);

        //setcolor(GREEN);
        //setcolor(DARKGRAY);
        put_str(150, 10,p1s,PL1_COLOR,0);
        put_str(170, 10,p2s,PL2_COLOR,0);
}

void draw_ball(int x,int y){
        //erase ball on old possition

        //circle_fill(last_ball_pos_x[page],last_ball_pos_y[page],BALL_RADIUS,0);
		rect_fill(0,0,320,200,0);
        //circle_fill(last_ball_pos_x[page],last_ball_pos_y[page],BALL_RADIUS - 2,0);

        //setcolor(BLACK);
        //circle(last_ball_pos_x[page],last_ball_pos_y[page],BALL_RADIUS);
        //circle(last_ball_pos_x[page],last_ball_pos_y[page],BALL_RADIUS-2);

        //draw ball
        //setcolor(ball_color);
        //circle(x,y,BALL_RADIUS);
        circle_fill(x,y,BALL_RADIUS,ball_color);
		//circle_slow(x,y,BALL_RADIUS,ball_color);
        //circle_fill(x,y,BALL_RADIUS - 2,ball_color);
        //circle(x,y,BALL_RADIUS-2);

		print_score();
		
        last_ball_pos_x[page] = x;
        last_ball_pos_y[page] = y;
}


void draw_pl1_bar(int y){
        // erase bar
        rect_fill(PL_BAR_X_POS ,last_pl1_pos_y[page],PL_BAR_X_POS + PL_BAR_SIZE_WIDTH ,last_pl1_pos_y[page] + PL_BAR_SIZE_HEIGHT, 0);
        // draw bar
        rect_fill(PL_BAR_X_POS ,y,PL_BAR_X_POS + PL_BAR_SIZE_WIDTH,y + PL_BAR_SIZE_HEIGHT, 49);
        last_pl1_pos_y[page] = y;
}

void draw_pl2_bar(int y){
        // erase bar
        rect_fill(SCREEN_WIDTH - PL_BAR_X_POS ,last_pl2_pos_y[page],SCREEN_WIDTH - ( PL_BAR_X_POS + PL_BAR_SIZE_WIDTH + 1) ,last_pl2_pos_y[page] + PL_BAR_SIZE_HEIGHT, 0);
        // draw bar
        rect_fill( SCREEN_WIDTH - PL_BAR_X_POS - 1 ,y,SCREEN_WIDTH - ( PL_BAR_X_POS + PL_BAR_SIZE_WIDTH +1) ,y + PL_BAR_SIZE_HEIGHT, 60);
        last_pl2_pos_y[page] = y;
}


void calculate_ball_pos(){
        if(x == (SCREEN_WIDTH - (BALL_RADIUS + 1))){
                        direction_x = GO_LEFT;
                }

                if(x == ( BALL_RADIUS )){
                        direction_x = GO_RIGHT;
                }

                if(y == ( SCREEN_HEIGHT - ( BALL_RADIUS + 1)))
						direction_y = GO_UP;

        if( y == (BALL_RADIUS) )
            direction_y = GO_DOWN;

        if(direction_y == GO_DOWN){
            y = y + 1;
                }else{
            y = y - 1;
                }

                if(direction_x == GO_RIGHT){
                        x = x + 1;
                }else{
                        x = x - 1;
                }
}


void initialize(void)
{
    initialize_graphics(true);
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
}

 

 void init_serial(){
 char buf;
  if((rc=serial_open(com, 19200L, 8, 'n', 1, SER_HANDSHAKING_XONXOFF)) != SER_SUCCESS)
    {
        printf("Can't open port! (%s)\n", errors[-rc]);
        return 0;
    }
	
	printf("Opened COM%d. Base 0x%03x, IRQ %d, %ld %d%c%d, Handshaking %s\n", com+1,
	serial_get_base(com), serial_get_irq(com), serial_get_bps(com), serial_get_data(com),
	(char)serial_get_parity(com), serial_get_stop(com), handshaking[serial_get_handshaking(com)]);
	
	serial_set_fifo_threshold(com, 14);
	
	while( 1 ){
			printf("WAIT FOR CLIENT:\n");
			if((len=serial_read(com, buff, 14)) > 0){
				if(strcmp(buff,"pong_cl") == 0){
					printf("WELCOME CLIENT:\n");
					break;
				}
			}
	}
	
 }
 
 void exit_serial(){
	serial_write(com, "CLOSE",5);
	if((rc=serial_close(com)) != SER_SUCCESS)
        printf("Can't close serial port! (%s)\n", errors[-rc]);
 }
 

int main(){
//FILE *dbg;

        int tmp ,player_counter=0, i,color,p=0;
        int pl1_y = SCREEN_HEIGHT/2 - PL_BAR_SIZE_HEIGHT / 2, pl2_y = SCREEN_HEIGHT/2 - PL_BAR_SIZE_HEIGHT / 2;

        //unsigned long int ball_delay = 900000, ball_counter=0;
		int ball_delay = 30;
		int ball_counter=0;
        int player_delay = 10;//(int)ball_delay / 3;
        
		char cur_serial_key;

        x=SCREEN_WIDTH/2,y=SCREEN_HEIGHT/2;
		page = 0;

        pl1_score = pl2_score = 0;
		direction_y = GO_DOWN;
        direction_x = GO_LEFT;
        ball_color = 33;

        last_pl1_pos_y[0] = SCREEN_HEIGHT/2;
        last_pl1_pos_y[1] = SCREEN_HEIGHT/2;;
        last_pl2_pos_y[0] = SCREEN_HEIGHT/2;;
        last_pl2_pos_y[1] = SCREEN_HEIGHT/2;;

		init_serial();
		initialize();
		
		init_msgi();
		//print_msgi();
		//delay(3000);
		
        draw_table();

        while(1){
		
		//player_counter = (int)ball_delay / 3;


                                if(get_key_status(SCAN_Q)){
                                        unhook_keyb_int();
                                        exit_graphic();
										exit_serial();
                                         return 0;
                                }

								

                                if(player_counter >= player_delay){
										
                                        if(get_key_status(SCAN_D)){ //d
                                                        if(pl1_y < SCREEN_HEIGHT - PL_BAR_SIZE_HEIGHT - 2 )
                                                                pl1_y += 1;
											//serial_write(com, "l", 1);
                                        }

                                        if(get_key_status(SCAN_E)){ //e
                                                        if(pl1_y  >   1  )
                                                                pl1_y -= 1;
											//			serial_write(com, "o", 1);
														
                                        }
										
										
										
										
										//cur_serial_key = '0';										
										
										
										/*
										memset(buff2, 0, strlen(buff2));
										sprintf(buff2,"p:%d|%d",pl1_y,pl2_y);
										
										printf("Send:%s\n",buff2);
										//strcpy(old_buff,buff);
										serial_write(com, buff2 ,14);
										*/
                                        player_counter = 0;
                               }
								
								
								if(cur_serial_key == 'O')
											if(pl2_y  >   1  )
												pl2_y -= 1;
											
										if(cur_serial_key == 'L')
											if(pl2_y < SCREEN_HEIGHT - PL_BAR_SIZE_HEIGHT - 2 )
                                               pl2_y += 1;
								
										
								
								
								
								


				if(ball_counter >= ball_delay){
				
                if(x == BALL_RADIUS + PL_BAR_SIZE_WIDTH + 1){
                        if( y > pl1_y && y < pl1_y + PL_BAR_SIZE_HEIGHT){
                                pl1_score = pl1_score + 1;
                                ball_color = PL1_COLOR;
                                //direction_x = 1 - direction_x;
                                direction_x = GO_RIGHT;
								
								if(pl1_score%3 == 0 && ball_delay >= 10)
									ball_delay -= 5;
                        }
                }

                //if(x + BALL_RADIUS/2 - 1 == SCREEN_WIDTH - 20){
                if(x == SCREEN_WIDTH - ( BALL_RADIUS + PL_BAR_SIZE_WIDTH + 3 ) ){
                        if(y > pl2_y && y < pl2_y + PL_BAR_SIZE_HEIGHT){
                                pl2_score++;
                                ball_color = PL2_COLOR;
                                direction_x = GO_LEFT;
								
								if(pl2_score%3 == 0 && ball_delay >= 10)
									ball_delay -= 5;
									
                        }
                }

				
                
                        calculate_ball_pos();
                        ball_counter = 0;
                
				
				
				//{b_x,b_y,p1_y,p2_y,}
				//sprintf(buff,"|%d|%d|%d|%d|",x,y,pl1_y,pl2_y);
				
				//sprintf(buff,"|%d|%d|%d|%d|",x,y,pl1_y,pl2_y);
				//sprintf(buff,"%d|%d|%d|%d",100,100,pl1_y,pl2_y);
				

				
					memset(buff, 0, strlen(buff));
					memset(old_buff, 0, strlen(old_buff));
					//pl1_y = 100;
					/*ball_c = 0;
					if(ball_color == PL1_COLOR)
						ball_c = 1;
						else if(ball_color == PL2_COLOR)
						ball_c = 2;
						*/
					sprintf(buff,"%s%s%s%s%s%s%s",msgi[x],msgi[y],msgi[pl1_y],msgi[pl1_score], msgi[pl2_score],msgi[ball_color],msgi[ball_delay]);
					//printf(buff,"%s%s%s%s\n",msgi[x],msgi[y],msgi[pl1_y],msgi[pl2_y]);
					//printf("Send:%s : %d\n",buff,ball_color);
				//	sprintf(buff,"b:%d|%d",x,y);
					if(strcmp(buff,old_buff) != 0){
						//printf("Send:%s\n",buff);
						strcpy(old_buff,buff);
						serial_write(com, buff ,14);
					//	printf("Send:%s\n",buff);
										//memset(buff2, 0, strlen(buff2));
									
						//serial_clear_rx_buffer(com);
					}else{
						//printf("Nimic de transmis\n");
					}
				}
				
				
				memset(buff2, 0, strlen(buff2));
				if((len=serial_read(com, buff2, 1)) == 1){
					cur_serial_key = buff2[0];
					/*		
					if(strcmp(buff2,old_buff2) != 0){
					//printf("Clienr rec:%s\n",buff2);
							strcpy(old_buff2,buff2);
							cur_serial_key = buff2[0];
							
							//sscanf (buff2,"p:%d",&cur_serial_key);
							printf("Rec %s\n",buff2);
							serial_clear_rx_buffer(com);
						}

						//serial_clear_rx_buffer(com);
					*/
				}else{
					cur_serial_key = '0';	
				}
				
		
                //print_score();
				
                draw_ball(x,y);
                draw_pl1_bar(pl1_y);
                draw_pl2_bar(pl2_y);
				show_buffer();

				
                ball_counter++;
                player_counter++;
    }
}


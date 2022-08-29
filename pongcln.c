
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


typedef struct{ 
	char *text;
}message;

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


	int len,len2;
    char buff[100];
	char buff2[100];
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

        put_str(150, 10,p1s,PL1_COLOR,0);
        put_str(170, 10,p2s,PL2_COLOR,0);
}


void draw_ball(int x,int y){
		rect_fill(0,0,320,200,0);
        circle_fill(x,y,BALL_RADIUS,ball_color);
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




 

 void init_serial(){
  if((rc=serial_open(com, 19200L, 8, 'n', 1, SER_HANDSHAKING_XONXOFF)) != SER_SUCCESS)
    {
        printf("Can't open port! (%s)\n", errors[-rc]);
        return 0;
    }
	
	printf("Opened COM%d. Base 0x%03x, IRQ %d, %ld %d%c%d, Handshaking %s\n", com+1,
	serial_get_base(com), serial_get_irq(com), serial_get_bps(com), serial_get_data(com),
	(char)serial_get_parity(com), serial_get_stop(com), handshaking[serial_get_handshaking(com)]);
	
	serial_set_fifo_threshold(com, 14);
	
	serial_write(com, "pong_cl", 7);
	
 }
 
 void exit_serial(){
	if((rc=serial_close(com)) != SER_SUCCESS)
        printf("Can't close serial port! (%s)\n", errors[-rc]);
 }

void initialize(void)
{
    initialize_graphics(true);
    ctrlbrk(ctrlbrk_handler);
    hook_keyb_int();
}

 
 void quit(){
	unhook_keyb_int();
     exit_graphic();
	  exit_serial();
 }
 
 typedef struct message{
	char data[20];
	message *next;
 }msg;

int main(){
//FILE *dbg;

        int tmp, ball_counter=0,player_counter=0, i,color,p=0;
        int pl1_y = SCREEN_HEIGHT/2 - PL_BAR_SIZE_HEIGHT / 2, pl2_y = SCREEN_HEIGHT/2 - PL_BAR_SIZE_HEIGHT / 2;

        int ball_delay = 0;
        int player_delay = 10;// = (int)ball_delay / 3;
        
		char cur_serial_key;

        x=SCREEN_WIDTH/2,y=SCREEN_HEIGHT/2;
		page = 0;

        pl1_score = pl2_score = 0;
		direction_y = GO_DOWN;
        direction_x = GO_LEFT;
        ball_color = 20;

        last_pl1_pos_y[0] = SCREEN_HEIGHT/2;
        last_pl1_pos_y[1] = SCREEN_HEIGHT/2;;
        last_pl2_pos_y[0] = SCREEN_HEIGHT/2;;
        last_pl2_pos_y[1] = SCREEN_HEIGHT/2;;



		initialize();
        init_serial();
		
		init_msgi();
		

        draw_table();

        while(1){
			//player_counter = (int)ball_delay / 3;

                                if(get_key_status(SCAN_Q)){
                                    quit();
                                    return 0;
                                }
								
								
								if(player_counter >= player_delay){
								
										if(get_key_status(SCAN_E)){
											if(pl2_y  >   1  )
														pl2_y -= 1;
											
												serial_write(com, "O" ,1);
												
												//serial_clear_tx_buffer(com);
										}
										
										if(get_key_status(SCAN_D)){
											if(pl2_y < SCREEN_HEIGHT - PL_BAR_SIZE_HEIGHT - 2 )
													   pl2_y += 1;
											
											serial_write(com, "L" ,1);
												
												//serial_clear_tx_buffer(com);
										}
										player_counter = 0;
								}

								
								if((len=serial_read(com, buff, 14)) == 14){
									x = msga[buff[0]][buff[1]];
									y = msga[buff[2]][buff[3]];
									pl1_y = msga[buff[4]][buff[5]];
									pl1_score = msga[buff[6]][buff[7]];
									pl2_score = msga[buff[8]][buff[9]];
									ball_color = msga[buff[10]][buff[11]];
									ball_delay = msga[buff[12]][buff[13]];
								}
								//printf("Rec:%s - %d\n",buff,ball_delay);
	
									
									
									draw_ball(x,y);					
								    draw_pl1_bar(pl1_y);
									draw_pl2_bar(pl2_y);
									 
									show_buffer();
									
	
								


				
                ball_counter++;
                player_counter++;
    }
}



#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>


void main(){
	
	int i = 0, j=0;
	
	char **files;
	struct dirent *dir;
	DIR *dr;
	dr = opendir("SAVE");
	
	
	//strcpy(filename,"        ");
	while(dir = (struct dirent *)readdir(dr) != NULL){
		//if(dir->d_type == 0x8) //if is file
			files[i] = (char *) malloc(8);
			strcpy(files[i],dir->d_name);
			i++;
	}
	
	while(i > 0){
		printf("AICI:%s\n",files[i]);
		i--;
	}
	
	
//	paused = 1;
/*	
	
	while( 1 ){
	
		
		
		
		rect_fast(100,90, 200, 100, WHITE);
		put_str_15(102+6,93,&files[0],WHITE,0);
		
		RestrictMousePtr(0,0,318,197);
		GetMouseStatus();
		put_str_15(GetPosX(),GetPosY(),"m",WHITE,240);
		
		
		
		show_buffer();
	}
	*/
	free(files);
	//ctrlbrk(ctrlbrk_handler);
   	//hook_keyb_int();
	
}
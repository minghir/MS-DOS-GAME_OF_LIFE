
#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <conio.h>
#include <mem.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>

/*
int find_ch(char ch, char *str){
	int i;
	//while(str[i] != '\0') 
		for (i = 0;i<=strlen(str);i++ )
		if(str[i] == ch)
			return i;
		
	return -1;
}

char* substr(char *str,int start, int end){
	int i = start;
	int j=0;
	char *buffer;
	buffer = (char *)malloc(end-start);
	
	for(;i<=end;i++){
		buffer[j] = str[i];
	}
	str = (char *)realloc(str,end-start);
	strcpy(str,buffer);
	free(buffer);

	return str;
	
}
*/
void main(){
	
	int i = 0, j=0,pos;
	
	char **files, *ptr_pos;
	struct dirent *entry;
	DIR *dir;
	dir = opendir(".\\SAVES");

	while( (entry = readdir(dir)) != NULL){

			if(strcmp (entry->d_name,"..") == 0 || strcmp (entry->d_name,".") == 0 ) continue;
		
			files[i] = (char *) malloc(20);
			strcpy(files[i],entry->d_name);

			files[i][strlen(files[i])-4]='\0';
			
			
			
			//strcpy(files[i],substr(files[i],0,find_ch('.',files[i]) ));

			i++;
	}
	
	i--;
	

	while(i >= 0){
		printf("%d:%s\n",i,files[i]);
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
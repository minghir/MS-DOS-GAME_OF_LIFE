#include <stdio.h>

char *table1, *table2;

void quit(){
	printf("eliberez1\n");
	free(table1);
	printf("eliberez2\n");
	free(table2);
	exit(0);
}

int main(){
	
	int i;
	
	printf("aloc1\n");
	table1 = (char *)malloc(256*256*256);
	printf("aloc2\n");
	table2 = (char *)malloc(256*256*256);
	
	for(i = 0;i<256*256*256;i++){
	table1[i] = 0;//random(2);
			//table[i] = random(2);
	table2[i] = 1;
	}
	
	
	
	memcpy(table1,table2,256*256*256);	
	
	quit();
	
	
	
}
#include <stdio.h>
#include <string.h>


int main(){
	

	char in;
	int linecount = 0;
	for(in = getchar(); in != EOF; in=getchar()){
		if(in=='\n'){linecount++;}
	}
	printf("%d \n",linecount);



}

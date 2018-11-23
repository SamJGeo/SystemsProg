#include <stdio.h>
#include <string.h>
#include <ctype.h>



int main(){
	
	char c[100];
	int count =0;
	for(c[count]=getchar();count<100 || getchar() == "\n" ;c[count]=getchar()){
		count+=1;
	

		printf("%c",c[count]);
	}


	


}

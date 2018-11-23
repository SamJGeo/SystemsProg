#include <stdio.h>

int main(){
	int tot = 0;

	for(int i=0;i<2;i++){
		tot += getchar()-'0';
		}
	printf("%d\n",tot);
}



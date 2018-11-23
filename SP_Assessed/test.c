#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


void test(char * hostname){
    int i;
    //findingthe index of the suffix
    for(i = strlen(hostname)-1; (hostname[i]!= '.');i--);
    if(i==0){
        fprintf(stderr, "invalid tld");
    }
    fprintf(stdout,"%d",i);
        //making a copy of the hostname suffix and converting to lower case
    char *hnCopy =malloc(sizeof(char)*5);
    strcpy(hnCopy,hostname+1+i);
    for(i=0;i<strlen(hnCopy);i++){
    hnCopy[i] = tolower(hnCopy[i]);
    }
    fprintf(stdout,"%s\n",hnCopy);
}


int main(){
    char * name = "sam.co.uk";
    test(name);


}
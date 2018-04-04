#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

char KEY_LIST[100][100]={"double","int","struct","break","else","long","switch","case","char","return","const","float","short","unsigned","continue","for","signed","void","goto","sizeof","do","if","while","default"};
char DATA_TYPE_LIST[100][100]={"int","char"};
char DEC[100][100]={"if","else","while", "for"};

void process_id(char a[], char b[]){
	int i,j;
	for(i=0; i<2; i++){
		if(strcmp(a, DATA_TYPE_LIST[i])==0){
			strcpy(b, "2");
			strcat(b, a);
			return;
		}
	}
	if(strcmp(a, "main")==0){
		strcpy(b, "3");
		strcat(b,a);
		return;
	}
	for(i=0; i<24; i++){
		if(strcmp(a, KEY_LIST[i])==0){
			strcpy(b, "9");
			strcat(b,a);
			return;
		}
	}
	strcpy(b, "0");
	strcat(b,a);
	return;
}

//This function will append 0 if identifier, 1 if number, 2 if keyword, 3 if main, 4 if other characters, 5 if spaces and newline, 6 if relop, 7 if addop, 8 if mulop
void getNextToken(FILE *fp, char output[]){
	char c=getc(fp);
	if(c==EOF){
		//fseek(fp, ftell(fp)-1, SEEK_SET);
		fseek(fp, -1, SEEK_CUR);
		output[0]='\0';
	}
	else if(c>='A' && c<='Z' || c>='a' && c<='z'){
		//0 for ID, 2 if datatype, 9 if keyword
		int i=0;
		char a[100], b[100];
		a[i++]=c;
		c=getc(fp);
		while((c>='A' && c<='Z' || c>='a' && c<='z' || c=='_' || c>='0' && c<='9') && c!=EOF){
			a[i++]=c;
			c=getc(fp);
		}
		a[i]='\0';
		//fseek(fp, ftell(fp)-1, SEEK_SET);
		fseek(fp, -1, SEEK_CUR);
		process_id(a, b);
		strcpy(output, b);
	}
	else if(c>='0' && c<='9'){
		int i=2;
		output[0]='1';
		output[1]=c;
		c=getc(fp);
		while(c>='0' && c<='9'){
			output[i++]=c;
			c=getc(fp);
		}
		output[i]='\0';
		//fseek(fp, ftell(fp)-1, SEEK_SET);
		fseek(fp, -1, SEEK_CUR);
	}
	else if(c=='=' || c=='(' || c==')' || c=='{' || c=='}' || c==';'||c==',' || c=='[' || c==']'){
		char d=c;
		c=getc(fp);
		if(c=='='){
			output[0]='6';
			output[1]=d;
			output[2]=c;
			output[3]='\0';
		}
		else{
			fseek(fp, -1, SEEK_CUR);
			output[0]='4';
			output[1]=d;
			output[2]='\0';
		}
	}
	else if(c=='+' || c=='-'){
		output[0]='7';
		output[1]=c;
		output[2]='\0';
	}
	else if(c=='!'){
		char d=c;
		c=getc(fp);
		if(c=='=')
		{
			output[0]='6';
			output[1]=d;
			output[2]=c;
			output[3]='\0';
		}
	}
	else if(c=='>' || c=='<'){
		char d=c;
		c=getc(fp);
		if(c=='='){
			output[0]='6';
			output[1]=d;
			output[2]=c;
			output[3]='\0';
		}
		else{
			fseek(fp, -1, SEEK_CUR);
			output[0]='6';
			output[1]=d;
			output[2]='\0';
		}
	}
	else if(c=='*' || c=='/' || c=='%'){
		output[0]='8';
		output[1]=c;
		output[2]='\0';
	}
	else if(c=='\n' || c==' ' || c=='\t'){
		getNextToken(fp, output);
	}
	else{
		output[0]='\0';
	}
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

FILE *fp, *op;
char id[1000][1000];
char data[1000][1000];
char ret[1000][1000];
char ar[1000];
int size[1000];
int id_coun[1000];
char temp[1000];
int fill=0;

char KEY_LIST[100][100]={"double","int","struct","break","else","long","switch","case","char","return","const","float","short","unsigned","continue","for","signed","void","goto","sizeof","do","if","while","default"};

int isAlpha(char c){
	if(c>='A' && c<='Z' || c>='a' && c<='z')
		return 0;
	return -1;
}

int isNum(char c){
	if(c>='0' && c<='9')
		return 0;
	return -1;
}

int isKey(char a[]){
	int i;
	for(i=0; i<24; i++){
		if(strcmp(a, KEY_LIST[i])==0){
			if(strcmp(a, "int")==0 || strcmp(a, "char")==0 || strcmp(a, "double")==0){
				//printf("Type: %s", a);
				strcpy(temp, a);
			}
			return 0;
		}
	}
	return -1;
}

int isPresent(char a[]){
	int i;
	for(i=0; i<100; i++){
		if(strcmp(a, id[i])==0){
			return id_coun[i];
		}
	}
	return -1;
}

int insertVal(char a[], int isf, int isarr, int s, int arg){
	int coun=1;
	int i=0;
	while(i<fill){
		i++;
		coun++;
	}
	printf("Id: %s\n", a);
	strcpy(id[i], a);
	if(isf==1){
		strcpy(data[i], "FUNC");
		strcpy(ret[i], temp);
		ar[i]=arg+'0';
	}
	else{
		strcpy(data[i], temp);
		if(strcmp(temp, "int")==0){
			if(isarr==1)
				size[i]=sizeof(int)*s;
			else
				size[i]=sizeof(int);
		}
		if(strcmp(temp, "double")==0){
			if(isarr==1)
				size[i]=sizeof(double)*s;
			else
				size[i]=sizeof(double);
		}
		if(strcmp(temp, "char")==0){
			if(isarr==1)
				size[i]=sizeof(char)*s;
			else
				size[i]=sizeof(char);
		}
	}
	fill++;
	id_coun[i]=coun;
	return id_coun[i];
}

int main (){
	fp=fopen("p1.c", "rb");
	op=fopen("output.c", "w");
	if(!fp){
		printf("Error 404");
		exit(0);
	}
	char c=getc(fp);
	while(c!=EOF){
		if(c=='#'){
			while(c!='\n' && c!=EOF){
				c=getc(fp);
			}
		}
		else if(c=='\n' || c==' ' || c=='\t'){
			putc(c, op);
		}
		else if(c=='(' || c==')' || c=='{' || c=='}' || c==';'||c==',' || c=='[' || c==']'){
			putc('<',op);
			putc(c,op);
			putc('>',op);
		}
		else if(c=='=' || c=='+' || c=='-' || c=='>' || c=='<' || c=='*' || c=='/' || c=='!'){
			char d=c;
			c=getc(fp);
			if(c!=EOF){
				if(c=='=' || c=='+' || c=='-'){
					putc('<', op);
					putc(d, op);
					putc(c, op);
					putc('>',op);
				}
				else{
					fseek(fp, -1, SEEK_CUR);
					putc('<', op);
					putc(d, op);
					putc('>',op);
				}
			}
		}
		else if(isAlpha(c)==0){
			char a[100];
			int i=0;
			a[i++]=c;
			c=getc(fp);
			while(c!=EOF && isAlpha(c)==0 || isNum(c)==0 || c=='_'){
				a[i++]=c;
				c=getc(fp);
			}
			a[i]='\0';
			fseek(fp, -1, SEEK_CUR);
			if(isKey(a)==0){
				fprintf(op, "<%s>", a);
			}
			else{
				if(isPresent(a)==-1){
					int isf=0, isarr=0, si=0, index=0, arg=0;
					c=getc(fp);
					if(c!=EOF){
						if(c=='('){
							isf=1;
							char d=c;
							d=getc(fp);
							while(d!=')'){	
								if(d==',')
									arg++;
								d=getc(fp);
								index++;
							}
							if(index>0)
								arg++;
							index++;
							fseek(fp, -index, SEEK_CUR);
						}
						else if(c=='['){
							isarr=1;
							char d=c;
							d=getc(fp);
							while(d!=']'){
								int t=d-'0';
								si=si*10+t;
								d=getc(fp);
								index++;
							}
							index++;
							fseek(fp, -index, SEEK_CUR);
						}
					}
					fseek(fp, -1, SEEK_CUR);
					int val=insertVal(a, isf, isarr, si, arg);
					fprintf(op, "<id, %d>", val);
				}
				else{
					int val=isPresent(a);
					fprintf(op, "<id, %d>", val);
				}
			}
		}
		else if(isNum(c)==0){
			char a[100];
			int i=0;
			a[i++]=c;
			c=getc(fp);
			while(c!=EOF && isNum(c)==0){
				a[i++]=c;
				c=getc(fp);
			}
			a[i]='\0';
			fseek(fp, -1, SEEK_CUR);
			fprintf(op, "<num, %s>", a);
		}
		else if(c=='"'){
			putc('<', op);
			putc(c, op);
			c=getc(fp);
			while(c!=EOF && c!='"'){
				putc(c, op);
				c=getc(fp);
			}
			putc(c, op);
			putc('>', op);
		}
		// else{
		// 	putc('<', op);
		// 	putc(c, op);
		// 	putc('>', op);
		// }
		c=getc(fp);
	}
	int i;
	printf("Name\tType\tSize\tReturn type\tNo of args\n");
	for(i=0; i<fill; i++){
		printf("%s\t", id[i]);
		printf("%s\t", data[i]);
		printf("%d\t", size[i]);
		printf("%s\t", ret[i]);
		printf("%c\n", ar[i]);
	}
}
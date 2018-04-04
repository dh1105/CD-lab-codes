#include "lexel.c"

void Progam();
void Decleration();
void Data_type();
void ID_List();
void A();
void Ass();
void B();
void Statement_list();
void Statement();
void expn();
void simple_expn();
void eprime();
void term();
void seprime();
void factor();
void tprime();
void mulop();
void relop();
void addop();
void decision_state();
void looping_state();
void dprime();

FILE *fp;
char output[100];
char token[100];
char backup[100];

void not_parsed(){
	printf("\nError");
	exit(0);
}

char* getNextToken2(FILE *fp){
	if(strlen(backup)==0){
		getNextToken(fp, output);
		return output;
	}
	else{
		strcpy(output, backup);
		backup[0]='\0';
		return output;
	}
}

void expn(){
	simple_expn();
	eprime();
}

void eprime(){
	strcpy(token, getNextToken2(fp));
	printf("Eprime: %s\n", token);
	strcpy(backup, token);
	if(token[0]=='6'){
		relop();
		simple_expn();
	}
}

void seprime(){
	strcpy(token, getNextToken2(fp));
	printf("Seprime: %s\n", token);
	strcpy(backup, token);
	if(token[0]=='7'){
		addop();
		term();
		seprime();
	}
}

void addop(){
	strcpy(token, getNextToken2(fp));
	printf("Addop: %s\n", token);
	if(token[0]!='7'){
		printf("Addop expected\n");
		not_parsed();
	}
}

void relop(){
	strcpy(token, getNextToken2(fp));
	printf("Relop: %s\n", token);
	if(token[0]!='6'){
		printf("Relop expected");
		not_parsed();
	}
}

void simple_expn(){
	term();
	seprime();
}

void term(){
	factor();
	tprime();
}

void factor(){
	strcpy(token, getNextToken2(fp));
	printf("Factor: %s\n", token);
	if(token[0]!='1' && token[0]!='0'){
		printf("Identifier or number expected");
		not_parsed();
	}
}

void tprime(){
	strcpy(token, getNextToken2(fp));
	printf("Tprime: %s\n", token);
	strcpy(backup, token);
	if(token[0]=='8'){
		mulop();
		factor();
		tprime();
	}
}

void mulop(){
	strcpy(token, getNextToken2(fp));
	printf("Mulp: %s\n", token);
	if(token[0]!='8'){
		printf("Mulop expected");
		not_parsed();
	}
}

void Program(){
	strcpy(token, getNextToken2(fp));
	printf("Program: %s\n", token);
	if(strcmp(token, "3main")!=0){
		printf("main not present");
		not_parsed();
	}
	strcpy(token, getNextToken2(fp));
	printf("Program1: %s\n", token);
	if(strcmp(token, "4(")!=0){
		printf("( expected after main");
		not_parsed();
	}
	strcpy(token, getNextToken2(fp));
	printf("Program2: %s\n", token);
	if(strcmp(token, "4)")!=0){
		printf(") expected after (");
		not_parsed();
	}
	strcpy(token, getNextToken2(fp));
	printf("Program3: %s\n", token);
	if(strcmp(token, "4{")!=0){
		printf("{ expected after ()");
		not_parsed();
	}
	Decleration();
	Statement_list();
	strcpy(token, getNextToken2(fp));
	printf("Program4: %s\n", token);
	if(strcmp(token, "4}")!=0){
		printf("} expected after program");
		not_parsed();
	}
}

void Decleration(){
	strcpy(token, getNextToken2(fp));
	printf("Decleration1: %s\n", token);
	strcpy(backup, token);
	if(token[0]=='2'){
		Data_type();
		ID_List();
		strcpy(token, getNextToken2(fp));
		printf("Decleration2: %s\n", token);
		if(strcmp(token, "4;")!=0){
			printf("; expected after decleration");
			not_parsed();
		}
		Decleration();
	}
} 

void ID_List(){
	strcpy(token, getNextToken2(fp));
	printf("ID_List: %s\n", token);
	if(token[0]!='0'){
		printf("Identifier1 expected");
		not_parsed();
	}
	A();
}

void Data_type(){
	strcpy(token, getNextToken2(fp));
	printf("Data type: %s\n", token);
	if(token[0]!='2'){
		printf("data type expected");
		not_parsed();
	}
}

void Ass(){
	strcpy(token, getNextToken2(fp));
	printf("Ass1: %s\n", token);
	if(token[0]!='0'){
		printf("Identifier expected");
		not_parsed();
	}
	strcpy(token, getNextToken2(fp));
	printf("Ass1: %s\n", token);
	if(strcmp(token, "4=")){
		printf("= expected");
		not_parsed();
	}
	expn();
}

void B(){
	strcpy(token, getNextToken2(fp));
	printf("B1: %s\n", token);
	if(strcmp(token, "4,")==0){
		ID_List();
	}
	else{
		strcpy(backup, token);
	}
}

void A(){
	strcpy(token, getNextToken2(fp));
	printf("A1: %s\n", token);
	if(strcmp(token, "4,")==0){
		ID_List();
	}
	else if(strcmp(token, "4[")==0){
		strcpy(token, getNextToken2(fp));
		printf("A2: %s\n", token);
		if(token[0]!='1'){
			printf("Number expected");
			not_parsed();
		}
		strcpy(token, getNextToken2(fp));
		printf("A2: %s\n", token);
		if(strcmp(token, "4]")!=0){
			printf("] expected after number");
			not_parsed();
		}
		B();
	}
	else{
		strcpy(backup, token);
	}
}

void Statement_list(){
	strcpy(token, getNextToken2(fp));
	strcpy(backup, token);
	printf("SL1: %s\n", token);
	if(token[0]=='0' || token[0]=='9'){
		Statement();
		Statement_list();
	}
}

void Statement(){
	strcpy(token, getNextToken2(fp));
	printf("State: %s\n", token);
	strcpy(backup, token);
	if(token[0]=='0'){
		Ass();
		strcpy(token, getNextToken2(fp));
		printf("State: %s\n", token);
		if(strcmp(token, "4;")!=0){
			printf("; expected after assignment");
			not_parsed();
		}
	}
	else if(token[0]=='9'){
		if(strcmp(token, "9if")==0){
			strcpy(token,getNextToken2(fp));
			decision_state();
		}
		else if(strcmp(token, "9for")==0 || strcmp(token, "9while")==0){
			looping_state();
		}
	}
	else{
		printf("Error assigment or decision or loop expected");
		not_parsed();
	}
}

void decision_state(){
	strcpy(token,getNextToken2(fp));
	printf("Decision: %s\n", token);
	if(strcmp(token, "4(")!=0){
		printf("( expected after if");
		not_parsed();
	}
	expn();
	strcpy(token,getNextToken2(fp));
	printf("Decision1: %s\n", token);
	if(strcmp(token, "4)")!=0){
		printf(") expected after if");
		not_parsed();
	}
	strcpy(token,getNextToken2(fp));
	printf("Decision1: %s\n", token);
	if(strcmp(token, "4{")!=0){
		printf("{ expected after if");
		not_parsed();
	}
	Statement_list();
	strcpy(token,getNextToken2(fp));
	printf("Decision1: %s\n", token);
	if(strcmp(token, "4}")!=0){
		printf("} expected after Statement_list");
		not_parsed();
	}
	dprime();
}

void dprime(){
	strcpy(token,getNextToken2(fp));
	printf("Dprime: %s\n", token);
	strcpy(backup, token);
	if(strcmp(token, "9else")==0){
		strcpy(token,getNextToken2(fp));
		printf("Dprime1: %s\n", token);
		if(strcmp(token, "4{")!=0){
			printf("{ expected after else");
			not_parsed();
		}
		Statement_list();
		strcpy(token,getNextToken2(fp));
		printf("Dprime2: %s\n", token);
		if(strcmp(token, "4}")!=0){
			printf("} expected after Statement_list1");
			not_parsed();
		}
	}
}

void looping_state(){
	strcpy(token,getNextToken2(fp));
	printf("Loop: %s\n", token);
	if(strcmp(token, "9while")==0){
		strcpy(token,getNextToken2(fp));
		if(strcmp(token, "4(")!=0){
			printf("Open ( paranthesis expected");
			exit(0);
		}
		expn();
		strcpy(token,getNextToken2(fp));
		if(strcmp(token, "4)")!=0){
			printf("Close ) paranthesis expected");
			exit(0);
		}
		strcpy(token,getNextToken2(fp));
		if(strcmp(token, "4{")!=0){
			printf("Open { paranthesis expected");
			exit(0);
		}
		Statement_list();
		strcpy(token,getNextToken2(fp));
		if(strcmp(token, "4}")!=0){
			printf("Open } paranthesis expected");
			exit(0);
		}
	}
	else if(strcmp(token, "9for")==0){
		strcpy(token,getNextToken2(fp));
		if(strcmp(token, "4(")!=0){
			printf("Open ( paranthesis expected");
			exit(0);
		}
		Ass();
		strcpy(token,getNextToken2(fp));
		printf("For1: %s\n", token);
		if(strcmp(token, "4;")!=0)
		{
			printf("Missing semicolon after statement\n");
			not_parsed();
		}
		expn();
		strcpy(token,getNextToken2(fp));
		printf("For2: %s\n", token);
		if(strcmp(token, "4;")!=0)
		{
			printf("Missing semicolon after statement\n");
			not_parsed();
		}
		Ass();
		strcpy(token,getNextToken2(fp));
		if(strcmp(token, "4)")!=0){
			printf("Open ) paranthesis expected");
			exit(0);
		}
		strcpy(token,getNextToken2(fp));
		if(strcmp(token, "4{")!=0){
			printf("Open { paranthesis expected");
			exit(0);
		}
		Statement_list();
		strcpy(token,getNextToken2(fp));
		if(strcmp(token, "4}")!=0){
			printf("Open } paranthesis expected");
			exit(0);
		}
	}
}

int main(){
	fp=fopen("input.c", "rb");
	if(!fp){
		printf("Error 404");
		exit(0);
	}
	Program();
	printf("Parsed");
}
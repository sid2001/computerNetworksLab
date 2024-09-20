%{
	#include <stdio.h>
	int valid = 0;
	char st[10], name[10];
%}

%%
^\n[A-Z]{5}[0-9]{4}[A-Z]$   { 
    //printf("Valid string: %s\n", yytext);
    sprintf(st,"%s",yytext);
    valid = 1;
    //printf("%s\n%s\n",yytext,st);
    return 0;
}
.|\n { return 0; }  
%%

int main() {
    printf("Enter name: ");
    scanf("%s",name);
    printf("Enter PAN: ");
    
    yylex();
    //printf("%c\n%c\n",st[4],name[0]);
    if(st[5]==name[0]&&valid==1){
    	printf("Valid PAN\n");
    }
    else {
        printf("Invalid string\n");
    }
    return 0;
}

int yywrap() {
    return 1;
}

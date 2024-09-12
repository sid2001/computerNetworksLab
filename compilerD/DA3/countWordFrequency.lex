%{
#include <stdio.h>
#include <string.h>

char searchWord[100];
int count = 0;        
%}

%%
[a-zA-Z]+ {
    if (strcmp(yytext, searchWord) == 0) {
        count++;
    }
}
.|\n ; 
%%

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <word> <filename>\n", argv[0]);
        return 1;
    }

    strcpy(searchWord, argv[1]);
    FILE *file = fopen(argv[2], "r"); 

    if (!file) {
        printf("Error: Could not open file %s\n", argv[2]);
        return 1;
    }

    yyin = file; 
    yylex();    

    printf("The word '%s' occurred %d times in the file %s.\n", searchWord, count, argv[2]);
    fclose(file); 

    return 0;
}

int yywrap() {
    return 1;
}

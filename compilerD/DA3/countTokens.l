%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int operators = 0, identifiers = 0, keywords = 0, specialSymbols = 0;

char *keywordList[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static", "struct", "switch",
    "typedef", "union", "unsigned", "void", "volatile", "while"
};

int isKeyword(char *word) {
    for (int i = 0; i < sizeof(keywordList) / sizeof(char*); i++) {
        if (strcmp(keywordList[i], word) == 0)
            return 1;
    }
    return 0;
}
%}

%%
[a-zA-Z_][a-zA-Z0-9_]* { 
    if (isKeyword(yytext)) {
        keywords++;
    } else {
        identifiers++;
    }
}

"+"|"-"|"*"|"/"|"%"|"++"|"--"|"="|"+="|"-="|"*="|"/="|"%="|"=="|"!="|"&&"|"||"|"!"|"&"|"|"|">"|"<"|">="|"<=" {
    operators++;
}

"("|")"|"{"|"}"|"["|"]"|";"|"," {
    specialSymbols++;
}

[ \t\n]+  ; 

.|\n  ; 
%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    yyin = file; 
    yylex(); 

    printf("Number of keywords: %d\n", keywords);
    printf("Number of identifiers: %d\n", identifiers);
    printf("Number of operators: %d\n", operators);
    printf("Number of special symbols: %d\n", specialSymbols);

    fclose(file);
    return 0;
}

int yywrap() {
    return 1;
}

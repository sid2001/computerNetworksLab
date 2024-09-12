%{
#include <stdio.h>

FILE *inputFile;
%}

%%
[<][^>]*[>] { 
    printf("HTML Tag: %s\n", yytext);  
    fflush(stdout); 
}

. ;  
\n ; 

%%

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        fflush(stdout);
        return 1;
    }
    inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("Error: Cannot open file %s\n", argv[1]);
        fflush(stdout);
        return 1;
    }

    printf("File %s opened successfully.\n", argv[1]);
    fflush(stdout);

    yyin = inputFile;

    yylex();

    fclose(inputFile);

    printf("File processing completed.\n");
    fflush(stdout);

    return 0;
}

int yywrap() {
    return 1; 
}

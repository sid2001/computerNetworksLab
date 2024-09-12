%{
#include <stdio.h>
#include <string.h>

FILE *inputFile; // File pointer for reading input
%}

%%
"<[^>]+>" { 
    printf("HTML Tag: %s\n", yytext); 
}

. ; /* Ignore other characters */
\n ; /* Ignore newlines */

%%

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file provided as an argument
    inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    // Set the input file for Lex
    yyin = inputFile;
    
    // Run the lexer
    yylex();

    // Close the file
    fclose(inputFile);

    return 0;
}

int yywrap() {
    return 1;
}

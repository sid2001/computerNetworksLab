%{
#include <stdio.h>
%}

%%

(http|https):\/\/(www\.)?[a-zA-Z0-9.-]+\.(com|org|in|[a-zA-Z]{2,}) {
    printf("Valid URL: %s\n", yytext);
}

. { printf("Invalid URL: %s\n", yytext); }
\n {  }

%%

int main() {
    printf("Enter a URL to check for validity:\n");
    yylex(); 
    return 0;
}

int yywrap() {
    return 1; 
}

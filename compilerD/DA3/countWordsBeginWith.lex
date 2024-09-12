%%
[Cc][a-zA-Z]*[tT] { 
    printf("%s\n", yytext); 
}
. ;
\n ;
%%

int main() {
    yylex();
    return 0;
}

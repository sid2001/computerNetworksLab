%{
int hasTwoVowels(char *word) {
    int count = 0;
    for (int i = 0; word[i]; i++) {
        if (strchr("aeiouAEIOU", word[i]))
            count++;
        if (count >= 2)
            return 1;
    }
    return 0;
}
%}

%%
[a-zA-Z]+ {
    if (hasTwoVowels(yytext)) {
        printf("%s\n", yytext);
    }
}
. ;
\n ;
%%

int main() {
    yylex();
    return 0;
}

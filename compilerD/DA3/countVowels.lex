%{
int vowels = 0, consonants = 0;
%}

%%
[aeiouAEIOU] { vowels++; }
[a-zA-Z] { consonants++; }
.  ;
\n { 
    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    vowels = consonants = 0;
}
%%

int main() {
    yylex();
    return 0;
}

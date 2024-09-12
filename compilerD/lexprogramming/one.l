%{
#include<stdio.h>
%}
sign [+-]?
digit [0-9]+
exp ([eE]{sign}{digit})
%%
\+?{digit} printf("\n Number is positive...\n");
\-{digit} printf("\n Number is negative...\n");
{sign}{digit}?\.{digit}? printf("\n Number is real...\n");
{sign}{digit}(\.{digit}?)?{exp} printf("\n Number is exponential...\n");
%%
int yywrap()
{
return 1;
}
int main()
{
char myString[100];
fgets (myString, sizeof(myString), stdin);
yy_scan_string(myString);
yylex();
}

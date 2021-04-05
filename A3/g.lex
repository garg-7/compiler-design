%{
    int vows = 0, cons = 0;
%}
VOWELS [aeiouAEIOU]
%%
{VOWELS}          vows+=1;
[a-zA-Z]         cons+=1;
.                  ;
\n                 ;                         
%%

int main(int argc, char *argv[])
{
    yylex();
    printf("Number of vowels: %d\n", vows);
    printf("Number of consonants: %d\n", cons);
}
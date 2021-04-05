%{
    int c;// count of number of words   
    int begin = 0; 
%}
%x S
%%
[a-zA-Z]       {BEGIN(S); begin = 1;}
.               ;
"\n"            ;
<S>[a-zA-Z]    ;
<S>[^(a-zA-Z)] {BEGIN(INITIAL); c++; begin = 0;}
%%

int main()
{
    yylex();
    if (begin == 1)
        c++;
    printf("Number of words: %d\n", c);
}
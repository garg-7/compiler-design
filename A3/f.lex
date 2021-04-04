%{
    int c = 0;  // number of words 
    int begin = 0;  
    int len = 0;
%}
%x S
%%
[a-z|A-Z]       {BEGIN(S); begin = 1; len = 1;}
.               ;
"\n"            ;
<S>[a-z|A-Z]    len ++;
<S>[^(a-z|A-Z)] {
                    BEGIN(INITIAL);
                    fprintf(stderr, "Length of current word: %d\n", len);
                    if (len > 5 && len < 10)
                        c++;
                    len = 0;
                    begin = 0;
                }
%%

int main()
{
    yylex();
    if (begin == 1 && len > 5 && len < 10 )
        c++;
    printf("Number of words with length in [6, 9]: %d\n", c);
}
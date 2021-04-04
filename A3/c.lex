%{
    // assumptions: only whitespaces i.e. ' ', '\t', '\n'
    // are delimiters. 
    int c, begin = 0; // count of number of tokens    
%}

%%
" "             |
"\t"            |
"\n"            {
                    if (begin==1)
                        c++;
                    begin = 0;
                }
.               begin=1;
%%

int main()
{
    yylex();
    if (begin == 1)
        c++;
    printf("Number of tokens: %d\n", c);
}
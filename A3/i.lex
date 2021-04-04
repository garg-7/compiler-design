%{
    char *word;
    char *curr;
    int maxLen = 0;
    int begin = 0;  
    int len = 0;
%}
%x S
%%
[a-z|A-Z]       {BEGIN(S); curr[len] = yytext[0]; 
                    begin = 1; len = 1;}
.               ;
"\n"            ;
<S>[a-z|A-Z]    {curr[len] = yytext[0]; len +=1; }
<S>[^(a-z|A-Z)] {
                    curr[len] = '\0';
                    // fprintf(stderr, "Length of current word: %d\n", len);
                    if (len > maxLen){
                        strcpy(word, curr);
                        maxLen = len;
                    }
                    curr[0] = '\0'; 
                    len = 0;
                    begin = 0;
                    BEGIN(INITIAL);
                }
%%

int main()
{
    word = (char *)malloc(100*sizeof(char));
    curr = (char *)malloc(100*sizeof(char));
    yylex();
    if(begin==1){
        curr[len] = '\0';
        // fprintf(stderr, "Length of current word: %d\n", len);
        if (len > maxLen){
            strcpy(word, curr);
            maxLen = len;
        }
        curr[0] = '\0'; 
        len = 0;
        begin = 0;
    }
    printf("Longest word: %s\n", word);
    printf("It's length: %d\n", maxLen);
    free(word);
}
%{
    #include <string.h>
    #include <stdlib.h>
    char *word;
    int found = 0;
    int begin = 0;
    int len = 0;
    char *curr;
%}
%x S
%%
[a-zA-Z]       {BEGIN(S); curr[len] = yytext[0];
                    begin = 1; len = 1;}
.               ;
"\n"            ;
<S>[a-zA-Z]    {curr[len] = yytext[0]; len ++;}
<S>[^(a-zA-Z)] {
                    curr[len] = '\0';
                    // fprintf(stderr, "Current word: %s\n", curr);
                    if (strcmp(curr, word)==0) {
                        found += 1; 
                    }
                    len = 0;
                    begin = 0;
                    curr[0] = '\0';
                    BEGIN(INITIAL);
                }
%%

int main(int argc, char *argv[]) {
    word = argv[1];
    curr = (char*)malloc(100*sizeof(char));
    free(curr);
    yylex();
    if (begin==1){
        curr[len] = '\0';
        if (strcmp(curr, word)==0) {
            found += 1; 
        }
        len = 0;
        begin = 0;
        curr[0] = '\0';
    }
    printf("Frequency of '%s': %d\n", word, found);
    printf("NOTE: Result is from a case-sensitive lookup.\n");
}
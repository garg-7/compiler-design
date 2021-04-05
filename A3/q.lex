/* ASSUMPTIONS: - The local part can contain only a-z, A-Z, 0-9, /, _, .
                - The local part cannot begin/end with a dot.
                - There cannot be consecutive dots anywhere.
                - The domain part cannot begin/end with a dot.
                - The domain part can contain only a-z, A-Z, 0-9, -, .
                - No whitespaces are allowed anywhere in the address.
*/
%{
    #include <stdlib.h>
    char *curr;
    int len = 0;
    void strappend(char*, char*, int*);
%}
%x S1 S2 S3 S4 S5 S6 S7

%%
[ \n\t]*                    BEGIN(INITIAL);
"."                         {
                                printf("Dot in the beginning not allowed.\n");
                                printf("Invalid email address.\n");
                                return 0;
                            }
[a-zA-Z0-9/_]+              {strappend(curr, yytext, &len); BEGIN(S2);}
.                           {
                                printf("Invalid character {%c} detected in the local part.\n", yytext[0]);
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S2>[-]+                    |
<S2>"."                     {strappend(curr, yytext, &len); BEGIN(S3);}
<S2>"@"                     {strappend(curr, yytext, &len); BEGIN(S4);}
<S2>.                       |
<S2><<EOF>>                 |
<S2>"\n"                    {
                                printf(". or @ expected but got {%c} in the local part.\n", yytext[0]);
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S3>"."                     {
                                printf("Consecutive dots not allowed.\n");
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S3>[a-zA-Z0-9/_]+          {strappend(curr, yytext, &len); BEGIN(S2);}
<S3>.                       |
<S3>"\n"                    {
                                printf("Unexpected character {%c} in the local part.\n", yytext[0]);
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S4>"-"                     {
                                printf("Cannot begin domain with a hyphen.\n");
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S4>[a-zA-Z0-9]+             {strappend(curr, yytext, &len); BEGIN(S5);}
<S4>.                       |
<S4>"\n"                    {
                                printf("[code: 1] Invalid character: {%c} in the domain part.\n", yytext[0]);
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S5>[-]+                    {strappend(curr, yytext, &len); BEGIN(S4);}
<S5>"."                     {strappend(curr, yytext, &len); BEGIN(S6);}
<S5>.                       |
<S5>"\n"                    {
                                printf("[code: 2] Invalid character: {%c} in the domain part.\n", yytext[0]);
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S6>[a-zA-Z0-9]+            {
                                // printf("I'm in %d\n", YY_START); 
                                strappend(curr, yytext, &len); BEGIN(S7);}
<S6>"."                     {
                                printf("Consecutive dots not permitted in the domain part.\n");
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S6>.                       |
<S6>"\n"                    {
                                printf("[code: 3] Invalid character: {%c} in the domain part.\n", yytext[0]);
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S7>[-]+                     |
<S7>"."                     {strappend(curr, yytext, &len); BEGIN(S6);}
<S7>[ \t\n]*                {BEGIN(S1);}
<S7>.                       {
                                printf("[code: 4] Invalid character: {%c} in the domain part.\n", yytext[0]);
                                printf("Invalid email address.\n");
                                return 0;
                            }
<S7><<EOF>>                 |
<S1><<EOF>>                 {
                                printf("%s is a valid email address.\n", curr);
                                return 0;
                            }
<S1>.                       {
                                printf("The email address till now was fine: %s\n", curr);
                                printf("Expected end of address. Some extra character was found. Exiting.\n");
                                return 0;
                            }


%%

void strappend(char* s1, char* s2, int *l1){
    int initial = *l1;
    for(int i=initial;i<initial+strlen(s2);i++){
        s1[i] = s2[i-initial];
        *l1 += 1;
    }
    return;
}

int main()
{
    curr = (char*)malloc(200*sizeof(char));
    yylex();
    free(curr);
}
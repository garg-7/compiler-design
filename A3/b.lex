%{
    #include <ctype.h>    
    int c = 0;  // to capitalize first letter in file
    int c1 = 0; // to check if a symbol precedes the first letter
%}
%x S1
%%
[^(a-z|A-Z| |\t|\n)]        { c1 ++; REJECT; }
[a-z|A-Z]           {
                        // to handle the first letter in the file
                        if (c==0 && c1==0) {
                            char ch = yytext[0];
                        if (islower(ch)) {
                            fprintf(stderr, "'%c' was lower case\n", yytext[0]);
                            ch = toupper(ch);
                        }
                        fprintf(stdout, "%c", ch);
                        c++;
                        }
                        else
                            ECHO;
                    }
"."                 |
"?"                 |
"!"                 {
                        // any of the sentence termination symbols.
                        BEGIN(S1);
                        ECHO;
                    }
<S1>[a-z|A-Z]       {
                        char ch = yytext[0];
                        if (islower(ch)) {
                            fprintf(stderr, "'%c' was lower case\n", yytext[0]);
                            ch = toupper(ch);
                        }
                        fprintf(stdout, "%c", ch);
                        BEGIN(INITIAL);
                    }
%%
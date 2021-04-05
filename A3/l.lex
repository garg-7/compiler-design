/* ASSUMPTIONS  - negative numbers do not have a space
                between the minus sign and the number.
                - positive and negative "numbers" includes
                whole as well as fractional numbers
                - no space between the dot and numbers on 
                either side in case of decimal numbers */
%{
    #include <stdlib.h>
    int cNeg = 0; // count of negative numbers
    int cPos = 0; // count of positive numbers
    int cDec = 0; // count of decimal numbers
    char * curr;
    int len = 0;
    void strappend(char*, char*, int*);
%}
%x S1 S2 S3 S4 S5
%%

"-"             {BEGIN(S1); len = 0;
                    curr[0] = yytext[0]; len+=1;}
<S1>"-"         {BEGIN(S1); len = 0; 
                    curr[0] = yytext[0]; len+=1;}
<S1>[0-9]       {cNeg+=1; strappend(curr, yytext, &len); 
                    BEGIN(S2);}
<S1>.           |
<S1>"\n"        {
                    // curr[len] = '\0';
                    // printf("Number found: %s\n", curr);
                    curr[0] = '\0';
                    len = 0;
                    BEGIN(INITIAL);
                }
<S2>[0-9]*      {strappend(curr, yytext, &len); }
<S2>"."         {BEGIN(S3);}
<S2>.           |
<S2>"\n"        {
                    curr[len] = '\0';
                    printf("Number found: %s\n", curr);
                    curr[0] = '\0';
                    len = 0;
                    BEGIN(INITIAL);
                }
<S3>[0-9]       {cDec+=1; curr[len] = '.'; len+=1; 
                    strappend(curr, yytext, &len); 
                     BEGIN(S4);}
<S3>.           |
<S3>"\n"        {
                    curr[len] = '\0';
                    printf("Number found: %s\n", curr);
                    curr[0] = '\0';
                    len = 0;
                    BEGIN(INITIAL);
                }
<S4>[0-9]*      {strappend(curr, yytext, &len);}
<S4>.           |
<S4>"\n"        {
                    curr[len] = '\0';
                    printf("Number found: %s\n", curr);
                    curr[0] = '\0';
                    len = 0;
                    BEGIN(INITIAL);
                }

[0-9]           {cPos+=1; curr[len] = yytext[0]; len+=1; 
                    BEGIN(S5);}
<S5>[0-9]*      { strappend(curr, yytext, &len);}
<S5>"."         BEGIN(S3);
<S5>.           |
<S5>"\n"        {
                    curr[len] = '\0';
                    printf("Number found: %s\n", curr);
                    curr[0] = '\0';
                    len = 0;
                    BEGIN(INITIAL);
                }

.               |
"\n"            {curr[0] = '\0'; len=0;};
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
    curr = (char*)malloc(1000*sizeof(char));
    yylex();
    if (YY_START==5 || YY_START==4 || YY_START==3 ||
    YY_START==2) {
        curr[len] = '\0';
        printf("Number found: %s\n", curr);
        curr[0] = '\0';
        len = 0;
    }
       free(curr);
    printf("Number of negative numbers: %d\n", cNeg);
    printf("Number of positive numbers: %d\n", cPos);
    printf("Number of decimal numbers: %d\n", cDec);

}
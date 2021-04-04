/* ASSUMPTIONS  - negative numbers do not have a space
                between the minus sign and the number.
                - positive and negative "numbers" includes
                whole as well as fractional numbers
                - no space between the dot and numbers on 
                either side in case of decimal numbers */
%{
    int cNeg = 0; // count of negative numbers
    int cPos = 0; // count of positive numbers
    int cDec = 0; // count of decimal numbers
%}
%x S1 S2 S3 S4 S5
%%

"-"             BEGIN(S1);
<S1>"-"         BEGIN(S1);
<S1>[0-9]       {cNeg+=1; BEGIN(S2);}
<S1>.           |
<S1>"\n"        {BEGIN(INITIAL);}
<S2>[0-9]*      ;
<S2>"."         BEGIN(S3);
<S2>.           |
<S2>"\n"        {BEGIN(INITIAL);}
<S3>[0-9]       {cDec+=1; BEGIN(S4);}
<S3>.           |
<S3>"\n"        {BEGIN(INITIAL);}
<S4>[0-9]*      ;
<S4>.           |
<S4>"\n"        {BEGIN(INITIAL);}

[0-9]           {cPos+=1; BEGIN(S5);}
<S5>[0-9]*      ;
<S5>"."         BEGIN(S3);
<S5>.           |
<S5>"\n"        {BEGIN(INITIAL);}

.               |
"\n"            ;
%%

int main()
{
    yylex();
    printf("Number of negative numbers: %d\n", cNeg);
    printf("Number of positive numbers: %d\n", cPos);
    printf("Number of decimal numbers: %d\n", cDec);

}
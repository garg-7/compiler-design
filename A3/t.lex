%x S1 S2 S3 S4 S5
%%
"\""        {
                BEGIN(S4);
                ECHO;
            }
<S4>"\""    {
                BEGIN(INITIAL);
                ECHO;
            }
<S4>"\n"    |
<S4>.       ECHO;
"/"         BEGIN(S1);
<S1>"*"     BEGIN(S2);
<S1>"/"     BEGIN(S5);
<S5>"\n"    BEGIN(INITIAL);
<S5>.       ;
<S2>[^*]    ;
<S2>"*"     BEGIN(S3);
<S3>"*"     ;
<S3>[^*/]   BEGIN(S2);
<S3>"/"     BEGIN(INITIAL);
%%
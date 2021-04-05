/* ASSUMPTION: - 
*/
%{
    #include <stdlib.h>
    char *curr;
    int len = 0;
    void strappend(char*, char*, int*);
%}
%x S1 S2 S3

%%
[ \n\t]*        BEGIN(S1);
[0-9]+          {strappend(curr, yytext, &len); BEGIN(S2);}
.               {
                    printf("Not a valid number [code: 1].\n");
                    return -10;
                }

<S1>[0-9]+      {strappend(curr, yytext, &len); BEGIN(S2);}
<S1>.           {
                    printf("Not a valid number [code: 2].\n");
                    return -10;
                }
<S2>[ \n\t]*    {BEGIN(S3);}
<S2>.           {
                    printf("Not a valid number [code: 3].\n");
                    return -10;
                }
<S3><<EOF>>     {   
                    if (len==10)
                        printf("%s is a valid 10-digit mobile number.\n", curr);
                    else 
                        printf("%s is NOT a valid 10-digit mobile number.\n", curr);
                    return 0;
                }
<S3>.           |
<S3>"\n"        {
                    printf("Not a valid number [code: 4].\n");
                    return -10;
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
    curr = (char*)malloc(100*sizeof(char));
    int check = yylex();
    if (check==-10);
    else {
        if (len==0)
        printf("No number found.\n");
    }
}
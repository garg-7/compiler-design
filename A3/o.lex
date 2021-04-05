/* ASSUMPTION: - 
*/
%{
    #include <stdlib.h>
    #include <math.h>
    char *curr;
    int len = 0;
    void strappend(char*, char*, int*);
    int checkForPrime(long long);
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
                    long long checkThis = atoi(curr);
                    if(checkThis == 1 || checkThis == 0) {
                        printf("%d is neither prime nor composite.\n", (int)checkThis);
                        return 0;
                    }
                    int result = checkForPrime(checkThis);
                    if (result==0)
                        printf("%s is NOT a prime.\n", curr);
                    else if(result==1)
                        printf("%s is a prime.\n", curr);
                    return 0;
                }
<S3>.           |
<S3>"\n"        {
                    printf("Not a valid number [code: 4].\n");
                    return -10;
                } 
%%

void strappend(char* s1, char* s2, int *l1) {
    int initial = *l1;
    for(int i=initial;i<initial+strlen(s2);i++){
        s1[i] = s2[i-initial];
        *l1 += 1;
    }
    return;
}

int checkForPrime(long long c) {
    long long sqrtC = sqrt(c);
    /* printf("%lld is the rounded sqrt.\n", sqrtC); */
    int factors = 0;
    for(int i=1;i<=sqrtC; i++){
        if (c%i==0)
            factors += 1;
        if (factors>1){
            break;
        }
    }
    if (factors==1)
        return 1;
    else if (factors>1)
        return 0;
    else { 
        printf("There was some problem in prime check.\n");
        return -1;
    }
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
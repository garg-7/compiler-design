/* ASSUMPTION: - 
*/
%{
    #include <stdlib.h>
    #include <math.h>
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
<S3><<EOF>>     return 0;
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
        else{
            printf("Number obtained: %s\n", curr);
            printf("Number of digits = %d\n", len);
            long long sum = 0;
            for(int i=0;i<len;i++){
                sum += (long long)pow(curr[i]-'0', len);
            }
            /* printf("Sum: %lld\n", sum); */
            if (sum==atoi(curr)){
                printf("Entered number is an Armstrong number.\n");
            }
            else {
                printf("Sum of digits powered by %d: %lld\n", len, sum);
                printf("Entered number is NOT an Armstrong number.\n");
            }
        }
    }
}
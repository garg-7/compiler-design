/* ASSUMPTION: - The generic IPv4 address is being talked about.
               - Leading and trailing whitespaces are allowed.
                 No spaces should be present within the address.
*/
%{
    #include <stdlib.h>
    char *curr ;
    int len = 0;
    void strappend(char*, char*, int*);
    int part_count = 0; 
%}
%x S1 S2 S3 S4
%%
[ \n\t]*     BEGIN(S4);
<S4>[0-9]   {BEGIN(S1); curr[len] = yytext[0]; len+=1;}
<S1>[0-9]*  strappend(curr, yytext, &len);
<S1>"."     {
                // if the number till now is good, go further
                // otherwise break
                int part = atoi(curr);
                if (part <=255 && part >=0){
                    part_count += 1;
                    BEGIN(S4);
                    curr[0]='\0';
                    len = 0;
                }
                else
                {
                    printf("Invalid IP address.\n");
                    return 0;
                }
                if (part_count == 3)
                    BEGIN(S2);
            }
<S1>.       |
<S1>"\n"    {
                printf("Invalid IP address.\n");
                return 0;
            }

<S2>[0-9]+[ \t\n]*  {
                strappend(curr, yytext, &len);
                int part = atoi(curr);
                if (part <=255 && part >=0){
                    BEGIN(S3);
                }
                else {
                    printf("Invalid IP address.\n");
                    return 0;
                }
            }
<S2>.       |
<S2>"\n"    {
                printf("Invalid IP address.\n");
                return 0;
            }
<S3><<EOF>> {
                        printf("Entered IP is valid.\n");
                        return 0;
                    }
<S3>.       {
                printf("Invalid IP address.\n");
                return 0;
            }
<S4>.           |
<S4>"\n"        {
                printf("Invalid IP address.\n");
                return 0;
            }
.           {
                printf("Invalid IP address.\n");
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
    curr = (char*)malloc(20*sizeof(char));
    yylex();
}
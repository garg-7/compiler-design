%{
    int lNo = 2;
%}

%%
"\n"    {
            printf("\n%3d ", lNo);
            lNo+=1;
        }


%%

int main() {
    printf("%3d ", 1);
    yylex();
}
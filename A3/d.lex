%{
    int tabs=0, lines=0, spaces=0;
%}
%%
" "         spaces++;
"\n"        lines++;
"\t"        tabs++;
.              ;
%%

int main() {
    yylex();

    printf("Number of spaces: %d\n", spaces);
    printf("Number of tabs: %d\n", tabs);
    printf("Number of lines: %d\n", lines+1);
}
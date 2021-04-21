#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_UNIT_SIZE 100
#define MAX_NUM_TERMINALS 50

// #define DEBUG

typedef struct set
{
    char **values;
    int max_size;
} set;

typedef struct setsq
{
    set **sets;
    int max_size;
} setsq;

bool isEmpty(set *s)
{
    if (strcmp(s->values[0], "") != 0)
        return false;
    else
        return true;
}

int isPresent(set *s, char *el)
{
    // printf("Checking for %s.\n", el);
    for (int i = 0; i < (s->max_size); i++)
    {
        if (strcmp(s->values[i], el) == 0)
        {
            return true;
        }
    }
    return false;
}

int getIndex(set *s, char *el)
{
    // printf("Getting index of %s.\n", el);
    for (int i = 0; i < (s->max_size); i++)
    {
        if (strcmp(s->values[i], el) == 0)
        {
            return i;
        }
    }
    return -1;
}

int getSize(set *s)
{
    int count = 0;
    for (int i = 0; i < s->max_size; i++)
    {
        if (strcmp(s->values[i], "") != 0)
            count += 1;
        else
            break;
    }
    return count;
}

void addElement(set *s, char *el)
{
    if (isPresent(s, el) == false)
    {
        // printf("Adding %s.\n", el);
        // printf("%d\n", s->max_size);
        for (int i = 0; i < (s->max_size); i++)
        {
            if (strcmp(s->values[i], "") == 0)
            {
                strcpy(s->values[i], el);
                // printf("%s added.\n", el);
                return;
            }
            // printf("%dth position is full.\n", i);
        }
        printf("[ERROR] No space in the set to enter %s.\n", el);
        exit(1);
    }
}

void removeElement(set *s, char *el)
{
    // printf("Removing %s.\n", el);
    for (int i = 0; i < (s->max_size); i++)
    {
        if (strcmp(s->values[i], el) == 0)
        {
            s->values[i][0] = '\0';
            printf("%s removed.\n", el);
            return;
        }
    }

    printf("[ERROR] The element {%s} is not even present in the set.\n", el);
    exit(2);
}

void printSet(set *s)
{
    if (isEmpty(s))
        printf("EMPTY SET.\n");
    else
    {
        printf("{\n");
        for (int i = 0; i < s->max_size; i++)
        {
            if (strcmp(s->values[i], "") != 0)
            {
                printf("  [%s]\n", s->values[i]);
            }
        }
        printf("}\n");
    }
    return;
}

void addSet(set *dst, set src)
{
    int srcSize = getSize(&src);
    for (int i = 0; i < srcSize; i++)
    {
        addElement(dst, src.values[i]);
    }
}

// !! IMPORTANT: source must be NULL-terminated. !!
int getToken(char *source, char *tok, int *loc)
{
    int currSpaceNum = 0;
    int count = 0;

    for (int i = 0;;i++)
    {
        if (source[i] != ' ' && source[i] != 0)
        {
            if (currSpaceNum == *loc)
            {
                // printf("adding %c\n", source[i]);
                tok[count] = source[i];
                count += 1;
            }
            else
            {
                // printf("ignoring %c\n", source[i]);
            }
        }
        else if (source[i] == ' ')
        {
            currSpaceNum += 1;
            // printf("got space\n");
            if (count > 0)
            {
                // printf("token filled, returning\n");
                *loc += 1;
                tok[count] = '\0';
                return 0;
            }
        }
        else if (source[i] == '\0')
        {
            // printf("got null\n");
            if (count > 0)
            {
                // printf("token filled, returning\n");
                *loc += 1;
                tok[count] = '\0';
                return 0;
            }
            return -1;
        }
        // if (i==50)break;
    }
    return -1;
}

void getHead(char *dst, char *src)
{
    int counter = 0;
    getToken(src, dst, &counter);
    return;
}

void getBody(char *dst, char *src)
{
    int counter = 0;
    char *tmp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    getToken(src, tmp, &counter);
    while (getToken(src, tmp, &counter) != -1)
    {   
        if (strcmp(tmp, "_")==0) { 
            // lookahead starts after underscore.
            break;
        }
        strcat(dst, tmp);
        strcat(dst, " ");
    }
    if (strlen(dst) > 0)
    {
        dst[strlen(dst) - 1] = 0; // remove the extra space at the end
    }
    return;
}

void getLookahead(char *dst, char *src)
{
    int counter = 0;
    // get the last token in the item
    while (getToken(src, dst, &counter) != -1);
    return;
}

int getStuffAfterDot(char *dst, char *rem, char *src){
    char tmp[MAX_UNIT_SIZE];
    int counter = 0;
    bool checkThis = false;
    bool startAccumulating = false;
    while(getToken(src, tmp, &counter)!=-1){
        if (strcmp(tmp, ".")==0){
            checkThis = true;
        }
        else if (checkThis) {
            strcpy(dst, tmp);
            startAccumulating = true;
            checkThis = false;
        }
        else if (startAccumulating){
            if (strcmp(tmp, "_")==0){
                // end has been reached
                // remove the extra space and return
                if (strlen(rem) > 0)
                {
                    rem[strlen(rem) - 1] = 0; // remove the extra space at the end
                }
                return 1;
            }
            strcat(rem, tmp);
            strcat(rem, " ");
        }
    }
    if (strlen(dst)>0) {
        return 1;
    }
    return -1;
}

int getBodyTillDot(char *dst, char *src) {
    char tmp[MAX_UNIT_SIZE];
    int counter = 0;
    while(getToken(src, tmp, &counter)!=-1) {
        if (strcmp(tmp, ".")==0) {
            if (strlen(dst) > 0) {
                dst[strlen(dst) - 1] = 0; // remove the extra space at the end
            }
            return 1;
        }
        else {
            strcat(dst, tmp);
            strcat(dst, " ");
        }
    }
    return -1;
}


// ensure that all the epsilon productions are at the end
set getFirstOfToken(char *tok, set* p, set *nonTerminals, set *terminals){
    printf("FIRST asked for token [%s]\n", tok);
    set dst;
    dst.max_size = MAX_NUM_TERMINALS;
    for(int i=0;i<dst.max_size;i++){
        dst.values = (char**)malloc(dst.max_size*sizeof(char*));
        if (dst.values==NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for(int i=0;i<dst.max_size;i++)
        {
            dst.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            if (dst.values[i] == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for set's values[i].\n");
                exit(3);
            }
            dst.values[i][0] = '\0';
        }
    }
    if (isPresent(terminals, tok)==true){
        printf("adding [%s] to the first set of [%s].\n", tok, tok);
        addElement(&dst, tok);
    }
    else {
        // token is non-terminal
        for(int i=0;i<getSize(p);i++)
        {
            // iterate over all the productions
            char *head = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
            getHead(head, p->values[i]);
            if (strcmp(head, tok)==0){
                printf("  it's production being considered: [%s]\n", p->values[i]);
                char *part = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                int counter = 0;
                getToken(p->values[i], part, &counter);
                while(getToken(p->values[i], part, &counter)!=-1){
                    addSet(&dst, getFirstOfToken(part, p, nonTerminals, terminals));
                    char *tmp = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                    
                    int retVal = getToken(p->values[i], part, &counter);
                    
                    // if this was the last token in the production
                    // don't remove the "?"
                    if (retVal==-1){
                        if (isPresent(&dst, "?")==true) {
                            break; // anyway would have exited
                        }
                        else {
                            break;
                        }
                    }
                    // if this wasn't the last token
                    // remove the epsilon and move on
                    else {
                        counter -= 1;   //revert changes from the check
                        if (isPresent(&dst, "?")==true) {
                            removeElement(&dst, "?");
                        }
                        else {
                            break;
                        }
                    }
                }
            }
        }
    }
    return dst;
}

set getFirstOfString(char *src, set *p, set *nonTerminals, set *terminals){
    // get the First set for the source string src.
    // put in the first in the set passed.
    printf("FIRST asked for [%s]\n", src);
    set dst;
    dst.max_size = MAX_NUM_TERMINALS;
    for(int i=0;i<dst.max_size;i++){
        dst.values = (char**)malloc(dst.max_size*sizeof(char*));
        if (dst.values==NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for(int i=0;i<dst.max_size;i++)
        {
            dst.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            if (dst.values[i] == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for set's values[i].\n");
                exit(3);
            }
            dst.values[i][0] = '\0';
        }
    }

    char *tok = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
    int counter = 0;
    while(getToken(src, tok, &counter)!=-1)
    {
        printf("  FIRST being calculated for: [%s]\n", tok);
        addSet(&dst, getFirstOfToken(tok, p, nonTerminals, terminals));
        
        int retVal = getToken(src, tok, &counter);
        if (retVal==-1){
            // this was the last one.
            // so, don't remove the epsilon if present
            break;
        }
        else{
            // isn't the last one so, 
            // remove the epsilon if present.
            if (isPresent(&dst, "?")==true){
                removeElement(&dst, "?");
                continue;
            }
            else {
                break;
            }
        }
    }
    return dst;
}

set closure(set src, set *p, set *nonTerminals, set *terminals){
    // dst -> set pointer to place thre returned set in 
    // src -> set to get a closure of
    // p -> set of productions
    set dst;
    dst.max_size = MAX_NUM_TERMINALS;
    for(int i=0;i<dst.max_size;i++){
        dst.values = (char**)malloc(dst.max_size*sizeof(char*));
        if (dst.values==NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for(int i=0;i<dst.max_size;i++)
        {
            dst.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            if (dst.values[i] == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for set's values[i].\n");
                exit(3);
            }
            dst.values[i][0] = '\0';
        }
    }
    addSet(&dst, src);
    int n = getSize(p);     // number of productions
    while(true){
        int initialSize = getSize(&dst);
        for(int i=0;i<getSize(&dst);i++){
            printf("considering the item: [%s]\n", dst.values[i]);
            char *tmp = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
            char *rem = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
            rem[0] = 0;
            int retVal = getStuffAfterDot(tmp, rem, dst.values[i]);
            if (retVal==1){
                if (isPresent(nonTerminals, tmp)==true){
                    printf("non-terminal right after dot: [%s]\n", tmp);
                    printf("after the nonterminal: [%s]\n", rem);
                    // there is a non terminal after the dot
                    if (strlen(rem)>0)
                        strcat(rem, " ");
                    char *lookahead = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                    getLookahead(lookahead, dst.values[i]);
                    strcat(rem, lookahead);
                    printf("    finding out FIRST of: [%s]\n", rem);
                    // get the first set of something
                    set first = getFirstOfString(rem, p, nonTerminals, terminals);
                    printSet(&first);
                    for (int j=0;j<n;j++){
                        char * head = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                        getHead(head, p->values[j]);
                        if (strcmp(head, tmp)==0){
                            printf("  [%s]'s production being considered: [%s]\n", tmp, p->values[j]);
                            // get the lookahead
                            for(int k=0;k<getSize(&first);k++){
                                if (isPresent(terminals, first.values[k])==true)
                                {
                                    // collect the item to be added
                                    char *newItem = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                                    strcpy(newItem, head);
                                    strcat(newItem, " . ");
                                    char *body = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                                    getBody(body, p->values[j]);
                                    strcat(newItem, body);
                                    strcat(newItem, " _ ");
                                    strcat(newItem, first.values[k]);
                                    printf("      Item being added: [%s]\n", newItem);
                                    addElement(&dst, newItem);
                                }
                            }

                        }
                    }
                }
            }
        }

        // if there was no change in this iteration, move on
        if(getSize(&dst)==initialSize)
            break;
    }
    printf("Set being returned by closure: ");
    printSet(&dst);
    return dst;
}

set GOTO(set src, set *p, set *nonTerminals, set *terminals){
    
    set dst;
    dst.max_size = MAX_NUM_TERMINALS;
    for(int i=0;i<dst.max_size;i++){
        dst.values = (char**)malloc(dst.max_size*sizeof(char*));
        if (dst.values==NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for(int i=0;i<dst.max_size;i++)
        {
            dst.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            if (dst.values[i] == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for set's values[i].\n");
                exit(3);
            }
            dst.values[i][0] = '\0';
        }
    }

    for(int i=0;i<getSize(&src);i++) {
        char *tmp = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
        char *rem = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
        rem[0] = 0;
        int retVal = getStuffAfterDot(tmp, rem, src.values[i]);
        if (retVal==1)
        {
            // there is something left to consume
            char *tmp2 = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
            int check = getBodyTillDot(tmp2, src.values[i]);
            if (check==1){
                // proper body present in tmp2
                char *newItem = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                newItem[0] = 0;
                char *tmp3 = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                int c = 0;
                getToken(src.values[i], tmp3, &c);
                strcat(newItem, tmp3); // add the head of the production
                strcat(newItem, " ");
                strcat(newItem, tmp2); // add the body till the dot
                if (strlen(tmp2)>0)
                    strcat(newItem, " ");
                strcat(newItem, tmp);   // consume the next token
                strcat(newItem, " .");  // add the dot
                if (strlen(rem)>0){
                    strcat(newItem, " ");
                    strcat(newItem, rem);   // add the remaining part of the after dot stuff
                }
                strcat(newItem, " - ");
                char *lookahead = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                getLookahead(lookahead, src.values[i]);
                strcat(newItem, lookahead);
                addElement(&dst, newItem);
            }
        }
    }
    return closure(dst, p, nonTerminals, terminals);
}

void init(set *s, int n){

    s->max_size = n;
    s->values = (char**)malloc(s->max_size*sizeof(char*));
    if (s->values==NULL)
    {
        fprintf(stderr, "Failed to allocate memory for set.\n");
        exit(3);
    }
    for(int i=0;i<s->max_size;i++)
    {
        s->values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        if (s->values[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set's values[i].\n");
            exit(3);
        }
        s->values[i][0] = '\0';
    }
    return;
}

int main()
{
    int n = 0;
    printf("Enter the start state: ");
    char *start = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    fscanf(stdin, "%[^\n]s", start);
    getchar();
    printf("Enter the number of productions: ");
    scanf("%d", &n);
    getchar();

    set *nonTerminals = (set*)malloc(sizeof(set));
    set *terminals = (set*)malloc(sizeof(set));
    set *p = (set*)malloc(sizeof(set));

    init(nonTerminals, n);
    init(terminals, MAX_NUM_TERMINALS);
    init(p, n+1);
    
    printf("Enter the productions one by one.\n");
    for (int i = 0; i < n; i++)
    {
#ifdef DEBUG
        printf("%dth: ", i);
#endif
        fscanf(stdin, "%[^\n]s", p->values[i]);
        getchar();
        p->values[i][strlen(p->values[i])] = '\0';
    }

    // char *inp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    // printf("Enter the input string: ");
    // fscanf(stdin, "%[^\n]s", inp);
    // getchar();
    // int len = strlen(inp);
    // inp[len] = ' ';
    // inp[len + 1] = '$';
    // inp[len + 2] = '\0';

    // populate the non-terminals set using the productions
    for (int i = 0; i < n; i++)
    {
        char *head = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        int counter = 0;
        getToken(p->values[i], head, &counter);
        addElement(nonTerminals, head);
    }
    // populate the terminals set using the productions
    for (int i = 0; i < n; i++)
    {
        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        int counter = 0;
        getToken(p->values[i], token, &counter);
        while (getToken(p->values[i], token, &counter) != -1)
        {
            if (isPresent(terminals, token) == false && isPresent(nonTerminals, token) == false)
            {
                addElement(terminals, token);
            }
        }
    }
    addElement(terminals, "$");
    addElement(terminals, "?");

    printf("The set of non-terminals:\n");
    printSet(nonTerminals);

    printf("The set of terminals:\n");
    printSet(terminals);


    // augment the grammar
    char *newStart = (char*)malloc(MAX_NUM_TERMINALS*sizeof(char));
    strcpy(newStart, start);
    strcat(newStart, "'");
    char *augmentationProduction = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
    strcpy(augmentationProduction, newStart);
    strcat(augmentationProduction, " ");
    strcat(augmentationProduction, start);
    addElement(p, augmentationProduction);

    printf("The augmented grammar: ");
    printSet(p);

    set *res = (set*)malloc(sizeof(set));
    init(res, 100);

    // create a set of sets
    setsq sq;
    sq.max_size = MAX_NUM_TERMINALS;
    sq.sets = (set**)malloc(sizeof(set*));
    for(int i=0;i<n;i++){
        sq.sets[i] = (set*)malloc(sizeof(set));
        init(sq.sets[i], 50);
    }

    set initialSet;
    initialSet.max_size = MAX_NUM_TERMINALS;
    for(int i=0;i<initialSet.max_size;i++){
        initialSet.values = (char**)malloc(initialSet.max_size*sizeof(char*));
        if (initialSet.values==NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for(int i=0;i<initialSet.max_size;i++)
        {
            initialSet.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            if (initialSet.values[i] == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for set's values[i].\n");
                exit(3);
            }
            initialSet.values[i][0] = '\0';
        }
    }
    
    char *firstItem = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
    firstItem[0] = 0;
    strcat(firstItem, newStart);
    strcat(firstItem, " . ");
    strcat(firstItem, start);
    strcat(firstItem, " _ ");
    strcat(firstItem, "$");
    addElement(&initialSet, firstItem);
    
    // printSet(&initialSet);

    *sq.sets[0] = closure(initialSet, p, nonTerminals, terminals);

    printSet(sq.sets[0]);
}
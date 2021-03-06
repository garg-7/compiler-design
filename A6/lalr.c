#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_UNIT_SIZE 100
#define MAX_NUM_TERMINALS 50
#define MAX_STACK_SIZE 100

// #define DEBUG_CLOSURE
// #define DEBUG_GOTO
// #define DEBUG_PARSING
// #define DEBUG_LR1
// #define DEBUG_ACTION

typedef struct set
{
    char **values;
    int max_size;
} set;

typedef struct setsq
{
    set **sets;
    int max_size;
    int curr;
} setsq;

typedef struct stack
{
    int nums[MAX_STACK_SIZE];
    int curr;
} stack;

int pop(stack *s)
{
    if (s->curr > -1)
    {
        int item = s->nums[s->curr];
        s->curr--;
        return item;
    }
    else
    {
        printf("Popping from empty stack.\n");
        exit(0);
    }
}

void push(int n, stack *s)
{
    s->curr += 1;
    s->nums[s->curr] = n;
    return;
}
bool isEmpty(set *s)
{
    if (strlen(s->values[0]) == 0)
        return true;
    else
        return false;
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

bool areIdentical(set *s1, set *s2)
{
    if (getSize(s1) != getSize(s2))
    {
        return false;
    }
    else
    {
        int matches = 0;
        for (int i = 0; i < s1->max_size; i++)
        {
            if (s1->values[i][0] == 0)
                break;
            for (int j = 0; j < s2->max_size; j++)
            {
                if (strcmp(s1->values[i], s2->values[j]) == 0)
                {
                    matches += 1;
                }
            }
        }
        if (matches == getSize(s1))
            return true;
        else
            return false;
    }
}

bool contains(setsq *s1, set *s2)
{
    for (int i = 0; i < s1->curr; i++)
    {
        if (isEmpty(s1->sets[i]) == true)
            break;
        if (areIdentical(s1->sets[i], s2) == true)
        {
            return true;
        }
    }
    return false;
}

// !! IMPORTANT: source must be NULL-terminated. !!
int getToken(char *source, char *tok, int *loc)
{
    int currSpaceNum = 0;
    int count = 0;

    for (int i = 0;; i++)
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
    dst[0] = 0;
    int counter = 0;
    char tmp[MAX_UNIT_SIZE];
    getToken(src, tmp, &counter); // ignore the head
    while (getToken(src, tmp, &counter) != -1)
    {
        if (strcmp(tmp, "_") == 0)
        {
            // lookahead starts after underscore.
            break;
        }
        strcat(dst, tmp); // add the current token to the body
        strcat(dst, " ");
    }
    // there will always be some non zero length body
    // so remove the extra space
    dst[strlen(dst) - 1] = 0; // remove the extra space at the end
    return;
}

void getLookahead(char *dst, char *src)
{
    int counter = 0;
    // get the last token in the item
    while (getToken(src, dst, &counter) != -1)
        ;
    return;
}

int getStuffAfterDot(char *dst, char *rem, char *src)
{
    char tmp[MAX_UNIT_SIZE];
    int counter = 0;
    bool checkThis = false;
    bool startAccumulating = false;
    while (getToken(src, tmp, &counter) != -1)
    {
        if (strcmp(tmp, ".") == 0)
        {
            checkThis = true;
        }
        else if (checkThis)
        {
            if (strcmp(tmp, "_") == 0)
            {
                //nothing after the dot, straightaway
                // got the underscore i.e. the lookahead
                return -1;
            }
            strcpy(dst, tmp);
            startAccumulating = true;
            checkThis = false;
        }
        else if (startAccumulating)
        {
            if (strcmp(tmp, "_") == 0)
            {
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
    if (strlen(dst) > 0)
    {
        return 1;
    }
    return -1;
}

int getBodyTillDot(char *dst, char *src)
{
    dst[0] = 0;
    char tmp[MAX_UNIT_SIZE];
    int counter = 0;
    getToken(src, tmp, &counter); // ignoring the head
    while (getToken(src, tmp, &counter) != -1)
    {
        if (strcmp(tmp, ".") == 0)
        {
            if (strlen(dst) > 0)
            {
                dst[strlen(dst) - 1] = 0; // remove the extra space at the end
                return 1;
            }
            else
            {
                // dst is empty that means there was no
                // body before the dot i.e. nothing in the item
                // has been consumed yet
                return -1;
            }
        }
        else
        {
            strcat(dst, tmp);
            strcat(dst, " ");
        }
    }
    return -1;
}

// ensure that all the epsilon productions are at the end
set getFirstOfToken(char *tok, set *p, set *nonTerminals, set *terminals)
{
    // printf("FIRST asked for token [%s]\n", tok);
    set dst;
    dst.max_size = MAX_NUM_TERMINALS;
    for (int i = 0; i < dst.max_size; i++)
    {
        dst.values = (char **)malloc(dst.max_size * sizeof(char *));
        if (dst.values == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for (int i = 0; i < dst.max_size; i++)
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
    if (isPresent(terminals, tok) == true)
    {
        // printf("adding [%s] to the first set of [%s].\n", tok, tok);
        addElement(&dst, tok);
    }
    else
    {
        // token is non-terminal
        for (int i = 0; i < getSize(p); i++)
        {
            // iterate over all the productions
            char *head = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            getHead(head, p->values[i]);
            if (strcmp(head, tok) == 0)
            {
                // printf("  it's production being considered: [%s]\n", p->values[i]);
                char *part = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                int counter = 0;
                getToken(p->values[i], part, &counter);
                while (getToken(p->values[i], part, &counter) != -1)
                {
                    addSet(&dst, getFirstOfToken(part, p, nonTerminals, terminals));
                    char tmp[MAX_UNIT_SIZE];

                    int retVal = getToken(p->values[i], part, &counter);

                    // if this was the last token in the production
                    // don't remove the "?"
                    if (retVal == -1)
                    {
                        if (isPresent(&dst, "?") == true)
                        {
                            break; // anyway would have exited
                        }
                        else
                        {
                            break;
                        }
                    }
                    // if this wasn't the last token
                    // remove the epsilon and move on
                    else
                    {
                        counter -= 1; //revert changes from the check
                        if (isPresent(&dst, "?") == true)
                        {
                            removeElement(&dst, "?");
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    return dst;
}

set getFirstOfString(char *src, set *p, set *nonTerminals, set *terminals)
{
    // get the First set for the source string src.
    // put in the first in the set passed.
    // printf("FIRST asked for [%s]\n", src);
    set dst;
    dst.max_size = MAX_NUM_TERMINALS;
    for (int i = 0; i < dst.max_size; i++)
    {
        dst.values = (char **)malloc(dst.max_size * sizeof(char *));
        if (dst.values == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for (int i = 0; i < dst.max_size; i++)
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

    char *tok = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    int counter = 0;
    while (getToken(src, tok, &counter) != -1)
    {
        // printf("  FIRST being calculated for: [%s]\n", tok);
        addSet(&dst, getFirstOfToken(tok, p, nonTerminals, terminals));

        int retVal = getToken(src, tok, &counter);
        if (retVal == -1)
        {
            // this was the last one.
            // so, don't remove the epsilon if present
            break;
        }
        else
        {
            // isn't the last one so,
            // remove the epsilon if present.
            if (isPresent(&dst, "?") == true)
            {
                removeElement(&dst, "?");
                continue;
            }
            else
            {
                break;
            }
        }
    }
    return dst;
}

set closure(set src, set *p, set *nonTerminals, set *terminals)
{
    // dst -> set pointer to place thre returned set in
    // src -> set to get a closure of
    // p -> set of productions
    set dst;
    dst.max_size = MAX_NUM_TERMINALS;
    for (int i = 0; i < dst.max_size; i++)
    {
        dst.values = (char **)malloc(dst.max_size * sizeof(char *));
        if (dst.values == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for (int i = 0; i < dst.max_size; i++)
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
    int n = getSize(p); // number of productions
    while (true)
    {
        int initialSize = getSize(&dst);
        for (int i = 0; i < getSize(&dst); i++)
        {
#ifdef DEBUG_CLOSURE
            printf("considering the item: [%s]\n", dst.values[i]);
#endif
            char tmp[MAX_UNIT_SIZE];
            char rem[MAX_UNIT_SIZE];
            rem[0] = 0;
            int retVal = getStuffAfterDot(tmp, rem, dst.values[i]);
            if (retVal == 1)
            {
                if (isPresent(nonTerminals, tmp) == true)
                {
#ifdef DEBUG_CLOSURE
                    printf("non-terminal right after dot: [%s]\n", tmp);
                    printf("after the nonterminal: [%s]\n", rem);
#endif
                    // there is a non terminal after the dot
                    if (strlen(rem) > 0)
                        strcat(rem, " ");
                    char *lookahead = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                    getLookahead(lookahead, dst.values[i]);
                    strcat(rem, lookahead);
#ifdef DEBUG_CLOSURE
                    printf("    finding out FIRST of: [%s]\n", rem);
#endif
                    // get the first set of something
                    set first = getFirstOfString(rem, p, nonTerminals, terminals);
#ifdef DEBUG_CLOSURE
                    printSet(&first);
#endif
                    for (int j = 0; j < n; j++)
                    {
                        char *head = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                        getHead(head, p->values[j]);
                        if (strcmp(head, tmp) == 0)
                        {
#ifdef DEBUG_CLOSURE
                            printf("  [%s]'s production being considered: [%s]\n", tmp, p->values[j]);
#endif
                            // get the lookahead
                            for (int k = 0; k < getSize(&first); k++)
                            {
                                if (isPresent(terminals, first.values[k]) == true)
                                {
                                    // collect the item to be added
                                    char *newItem = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                                    strcpy(newItem, head);
                                    strcat(newItem, " . ");
                                    char *body = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                                    getBody(body, p->values[j]);
                                    strcat(newItem, body);
                                    strcat(newItem, " _ ");
                                    strcat(newItem, first.values[k]);
#ifdef DEBUG_CLOSURE

                                    printf("      Item being added: [%s]\n", newItem);
#endif
                                    addElement(&dst, newItem);
                                }
                            }
                        }
                    }
                }
            }
        }

        // if there was no change in this iteration, move on
        if (getSize(&dst) == initialSize)
            break;
    }
    // printf("Set being returned by closure: ");
    // printSet(&dst);
    return dst;
}

set GOTO(set src, char *X, set *p, set *nonTerminals, set *terminals)
{

    set dst;
    dst.max_size = MAX_NUM_TERMINALS;
    for (int i = 0; i < dst.max_size; i++)
    {
        dst.values = (char **)malloc(dst.max_size * sizeof(char *));
        if (dst.values == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for (int i = 0; i < dst.max_size; i++)
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

    for (int i = 0; i < getSize(&src); i++)
    {
        char tmp[MAX_UNIT_SIZE];
        char rem[MAX_UNIT_SIZE];
        rem[0] = 0;
        int retVal = getStuffAfterDot(tmp, rem, src.values[i]);
        if (retVal == 1)
        {
            if (strcmp(tmp, X) == 0)
            {
#ifdef DEBUG_GOTO
                printf("Can consume [%s], in case of [%s]\n", X, src.values[i]);
#endif
                // there is something left to consume
                char tmp2[MAX_UNIT_SIZE];
                getBodyTillDot(tmp2, src.values[i]);
                char newItem[MAX_UNIT_SIZE];
                newItem[0] = 0;
                char tmp3[MAX_UNIT_SIZE];
                int c = 0;
                getToken(src.values[i], tmp3, &c);
                strcat(newItem, tmp3); // add the head of the production
                strcat(newItem, " ");
                strcat(newItem, tmp2); // add the body till the dot
                if (strlen(tmp2) > 0)
                    strcat(newItem, " ");
                strcat(newItem, tmp);  // consume the next token
                strcat(newItem, " ."); // add the dot
                if (strlen(rem) > 0)
                {
                    strcat(newItem, " ");
                    strcat(newItem, rem); // add the remaining part of the after dot stuff
                }
                strcat(newItem, " _ ");
                char lookahead[MAX_UNIT_SIZE];
                getLookahead(lookahead, src.values[i]);
                strcat(newItem, lookahead);
#ifdef DEBUG_GOTO
                printf("        Item being added to GOTO: [%s]\n", newItem);
#endif
                addElement(&dst, newItem);
            }
        }
    }
#ifdef DEBUG_GOTO
    printf("Returning closure of: \n");
    printSet(&dst);
#endif
    return closure(dst, p, nonTerminals, terminals);
}

void init(set *s, int n)
{

    s->max_size = n;
    s->values = (char **)malloc(s->max_size * sizeof(char *));
    if (s->values == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for set.\n");
        exit(3);
    }
    for (int i = 0; i < s->max_size; i++)
    {
        s->values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        if (s->values[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set's values[i].\n");
            exit(3);
        }
        s->values[i][0] = 0;
        if (strlen(s->values[i]) != 0)
        {
            printf("Something's wrong\n");
            exit(1);
        }
    }
    return;
}

void getCore(char *dst, char *src)
{

    // works under the assumption that
    // there will always be something before the
    // lookahead - kind of trivially always true

    dst[0] = 0;
    int counter = 0;
    char tmp[MAX_UNIT_SIZE];

    while (getToken(src, tmp, &counter) != -1)
    {
        if (strcmp(tmp, "_") == 0)
        {
            dst[strlen(dst) - 1] = 0; // removing the extra space
            return;
        }
        else
        {
            strcat(dst, tmp);
            strcat(dst, " ");
        }
    }
    printf("There is no lookahead! Something's not right.\n");
    printf("Exiting...\n");
    exit(7);
}

void getCoreSet(set *dst, set *src)
{

    // return src's core in dst

    for (int i = 0; i < getSize(src); i++)
    {
        char tmp[MAX_UNIT_SIZE];
        getCore(tmp, src->values[i]);
        addElement(dst, tmp);
    }
    return;
}
bool isCoreSame(set *s1, set *s2)
{
    set *core1 = (set *)malloc(sizeof(set));
    init(core1, 50);

    set *core2 = (set *)malloc(sizeof(set));
    init(core2, 50);

    getCoreSet(core1, s1);
    getCoreSet(core2, s2);

#ifdef DEBUG
    printf("Set 1: \n");
    printSet(s1);
    printf("Core of set 1: \n");
    printSet(core1);

    printf("Set 2: \n");
    printSet(s2);
    printf("Core of set 2: \n");
    printSet(core2);
#endif

    if (areIdentical(core1, core2) == true)
        return true;
    else
        return false;
}

int numSpaces(char *s)
{
    int count = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] == ' ')
            count += 1;
    }
    return count;
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

    set *nonTerminals = (set *)malloc(sizeof(set));
    set *terminals = (set *)malloc(sizeof(set));
    set *p = (set *)malloc(sizeof(set));

    init(nonTerminals, n);
    init(terminals, MAX_NUM_TERMINALS);
    init(p, n + 1);

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
    char *newStart = (char *)malloc(MAX_NUM_TERMINALS * sizeof(char));
    strcpy(newStart, start);
    strcat(newStart, "'");
    char *augmentationProduction = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    strcpy(augmentationProduction, newStart);
    strcat(augmentationProduction, " ");
    strcat(augmentationProduction, start);
    addElement(p, augmentationProduction);
    addElement(nonTerminals, newStart);

    printf("The augmented grammar: ");
    printSet(p);

    set *res = (set *)malloc(sizeof(set));
    init(res, 100);

    // create a set of sets
    setsq sq;
    sq.max_size = MAX_NUM_TERMINALS;
    sq.sets = (set **)malloc(MAX_NUM_TERMINALS * sizeof(set *));
    for (int i = 0; i < sq.max_size; i++)
    {
        sq.sets[i] = (set *)malloc(sizeof(set));
        if (sq.sets[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        init(sq.sets[i], 50);
    }
    sq.curr = 0;

    set initialSet;
    initialSet.max_size = MAX_NUM_TERMINALS;
    for (int i = 0; i < initialSet.max_size; i++)
    {
        initialSet.values = (char **)malloc(initialSet.max_size * sizeof(char *));
        if (initialSet.values == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        for (int i = 0; i < initialSet.max_size; i++)
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

    char *firstItem = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    firstItem[0] = 0;
    strcat(firstItem, newStart);
    strcat(firstItem, " . ");
    strcat(firstItem, start);
    strcat(firstItem, " _ ");
    strcat(firstItem, "$");
    addElement(&initialSet, firstItem);

    // printSet(&initialSet);

    *sq.sets[sq.curr] = closure(initialSet, p, nonTerminals, terminals);
    // printSet(sq.sets[0]);
    // exit(9);
    sq.curr += 1;
    // get the set of item sets
    int prevCount = 0;
    while (true)
    {
        int initialCount = sq.curr;
        for (int i = prevCount; i < initialCount; i++)
        {
#ifdef DEBUG_LR1
            printf("The item set being considered: \n");
            printSet(sq.sets[i]);
#endif

            for (int j = 0; j < getSize(nonTerminals); j++)
            {
#ifdef DEBUG_LR1
                printf("  The non-terminal being considered: [%s]\n", nonTerminals->values[j]);
#endif
                set GOTOReceived = GOTO(*sq.sets[i], nonTerminals->values[j], p, nonTerminals, terminals);
#ifdef DEBUG_LR1
                printf("    The GOTO set received: \n");
                printSet(&GOTOReceived);
#endif
                if (isEmpty(&GOTOReceived) == false && contains(&sq, &GOTOReceived) == false)
                {
#ifdef DEBUG_LR1
                    printf("    This GOTO set wasn't empty and wasn't previously in the set of item sets, so adding at number %d...\n", sq.curr);
#endif
                    if (sq.sets[sq.curr] == NULL)
                    {
                        printf("sorry set is NULL");
                        exit(6);
                    }
                    addSet(sq.sets[sq.curr], GOTOReceived);
                    sq.curr += 1;
                }
            }
            for (int j = 0; j < getSize(terminals); j++)
            {
#ifdef DEBUG_LR1
                printf("  The terminal being considered: [%s]\n", terminals->values[j]);
#endif
                set GOTOReceived = GOTO(*sq.sets[i], terminals->values[j], p, nonTerminals, terminals);
#ifdef DEBUG_LR1
                printf("    The GOTO set received: \n");
                printSet(&GOTOReceived);
#endif
                if (isEmpty(&GOTOReceived) == false && contains(&sq, &GOTOReceived) == false)
                {
#ifdef DEBUG_LR1
                    printf("    This GOTO set wasn't empty and wasn't previously in the set of item sets, so adding at number %d...\n", sq.curr);
#endif
                    if (sq.sets[sq.curr] == NULL)
                    {
                        printf("sorry set is NULL");
                        exit(6);
                    }
                    addSet(sq.sets[sq.curr], GOTOReceived);
                    sq.curr += 1;
                }
            }
        }
#ifdef DEBUG_LR1
        printf("At the end of a whole iteration, the sets we have are: \n");
        for (int i = 0; i < sq.curr; i++)
        {
            printSet(sq.sets[i]);
        }
#endif
        if (sq.curr == initialCount)
            break;
        prevCount = initialCount;
    }

// print out the LR(1) items
#ifdef DEBUG_LR1
    printf("LR(1) item sets created are: \n");
    for (int i = 0; i < sq.curr; i++)
    {
        printSet(sq.sets[i]);
    }
#endif
    // create an isDone array to maintain whether
    // a set has been handled or not

    // for every set i
    // mark it as done
    // iterate over all sets i+1 to n
    // see if the core is the same for both the sets
    // if yes, take a union mark that inner set as done
    // move on.
    // if no, check the next inner set

    // printf("Should be 0: %d\n", isCoreSame(sq.sets[11], sq.sets[12]));
    // exit(1);

    bool isDone[sq.curr];
    for (int i = 0; i < sq.curr; i++)
        isDone[i] = false;

    // assign the final set to which any current set belongs
    // after merging
    int idx[sq.curr];

    // the final set C'
    setsq final;
    final.max_size = MAX_NUM_TERMINALS;
    final.sets = (set **)malloc(MAX_NUM_TERMINALS * sizeof(set *));
    for (int i = 0; i < final.max_size; i++)
    {
        final.sets[i] = (set *)malloc(sizeof(set));
        if (final.sets[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for set.\n");
            exit(3);
        }
        init(final.sets[i], 50);
    }
    final.curr = 0;
    printf("\nNumber of item sets before merging: [%d]\n\n", sq.curr);
    for (int i = 0; i < sq.curr; i++)
    {
        if (isDone[i] == false)
        {
            // ith item set hasn't been handled yet

            // the accumulator (union) set
            set *tmp = (set *)malloc(sizeof(set));
            init(tmp, 100);
            addSet(tmp, *sq.sets[i]);
            idx[i] = final.curr;
            for (int j = i + 1; j < sq.curr; j++)
            {
                if (isCoreSame(sq.sets[i], sq.sets[j]) == true)
                {
                    printf("Merging state %d and %d.\n", i, j);
                    addSet(tmp, *sq.sets[j]);
                    isDone[j] = true;
                    idx[j] = final.curr;
                }
            }
            *final.sets[final.curr] = *tmp;
            final.curr += 1;
        }
    }

#ifdef DEBUG_LR1
    printf("Merged sets: \n");
    for (int i = 0; i < final.curr; i++)
    {
        printSet(final.sets[i]);
    }
#endif

    printf("\nNumber of item sets after merging: [%d]\n", final.curr);
    // initialize the action table
    char *action[final.curr][getSize(terminals)];
    for (int i = 0; i < final.curr; i++)
    {
        for (int j = 0; j < getSize(terminals); j++)
        {
            action[i][j] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            action[i][j][0] = 0;
        }
    }
    // exit(9);
    // fill in the action table

    for (int i = 0; i < sq.curr; i++)
    {
#ifdef DEBUG_ACTION
        printf("The set being handled: ");
        printSet(sq.sets[i]);
#endif
        // sleep(1);
        for (int j = 0; j < getSize(sq.sets[i]); j++)
        {
#ifdef DEBUG_ACTION
            printf("  The item being handled: [%s]\n", sq.sets[i]->values[j]);
#endif
            // sleep(1);
            char tmp[MAX_UNIT_SIZE];
            tmp[0] = 0;
            char rem[MAX_UNIT_SIZE];
            rem[0] = 0;

            int check = getStuffAfterDot(tmp, rem, sq.sets[i]->values[j]);
            if (check == 1)
            {
#ifdef DEBUG_ACTION
                printf("    There is something left to be consumed in the item.\n");
#endif
                // sleep(1);
                // there is something after the dot
                if (isPresent(terminals, tmp) == true)
                {
#ifdef DEBUG_ACTION
                    printf("      There is a terminal after the dot.\n");
#endif
                    set tmpSet = GOTO(*sq.sets[i], tmp, p, nonTerminals, terminals);
#ifdef DEBUG_ACTION
                    printf("      GOTO of set %d for terminal [%s]: \n", i, tmp);
                    printSet(&tmpSet);
#endif
                    // sleep(1);
                    for (int k = 0; k < sq.curr; k++)
                    {
#ifdef DEBUG_ACTION
                        printf("        Item set %d: \n", k);
                        printSet(sq.sets[k]);
#endif
                        // sleep(2);

                        if (areIdentical(&tmpSet, sq.sets[k]) == true)
                        {
#ifdef DEBUG_ACTION
                            printf("      Its GOTO matched to item set no. [%d]\n", k);
#endif
                            char stringToAdd[10];
                            stringToAdd[0] = 0;
                            char strFromInt[MAX_UNIT_SIZE];
                            sprintf(strFromInt, "%d", idx[k]);

                            strcat(stringToAdd, "s_");
                            strcat(stringToAdd, strFromInt);

                            // if not already the same then add
                            if (strcmp(action[idx[i]][getIndex(terminals, tmp)], stringToAdd) != 0)
                                strcat(action[idx[i]][getIndex(terminals, tmp)], stringToAdd);
                        }
                    }
                    // exit(99);
                }
            }
            else
            {
#ifdef DEBUG_ACTION
                printf("    Nothing is left to be consumed in the item.\n");
#endif
                // sleep(1);
                // there is nothing after the dot
                // get head of the current item
                char *head1 = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                getHead(head1, sq.sets[i]->values[j]);
                if (strcmp(newStart, head1) == 0)
                {
#ifdef DEBUG_ACTION
                    printf("    Head of the item is the start state.\n");
                    printf("    Adding 'accept' configuration.\n");
#endif
                    // sleep(1);
                    // if head of item is the new start state,
                    //then set action[i, &] to accept
                    strcat(action[idx[i]][getIndex(terminals, "$")], "accept");
                }
                else
                {
// head of item is not start state
// some reduce operation to be added to the table.
#ifdef DEBUG_ACTION
                    printf("    Head [%s] is not the start state.\n", head1);
#endif
                    // sleep(1);
                    // get body of the item i.e. till dot
                    char bodyBeforeDot[MAX_UNIT_SIZE];
                    getBodyTillDot(bodyBeforeDot, sq.sets[i]->values[j]);
#ifdef DEBUG_ACTION
                    printf("    Body is [%s]\n", bodyBeforeDot);
#endif
                    for (int l = 0; l < getSize(p); l++)
                    {

                        // get body of the production being tried
                        char body[MAX_UNIT_SIZE];

                        getBody(body, p->values[l]);

                        // get head of the production being tried
                        char head[MAX_UNIT_SIZE];
                        getHead(head, p->values[l]);
                        // printf("      production being tested: [%s %s]\n", body, head);
                        // if the production matches the current item that
                        // has been consumed...
                        if (strcmp(head, head1) == 0 && strcmp(body, bodyBeforeDot) == 0)
                        {
#ifdef DEBUG_ACTION
                            printf("      Production the item matched to: [%s]\n", p->values[l]);
#endif
                            // sleep(1);
                            // get the lookahead of the item
                            char lookahead[MAX_UNIT_SIZE];
                            getLookahead(lookahead, sq.sets[i]->values[j]);
                            char stringToAdd[10];
                            stringToAdd[0] = 0;
                            char strFromInt[MAX_UNIT_SIZE];
                            sprintf(strFromInt, "%d", l + 1);
                            // add reduction rule to index i, indexOf(the lookahead found above)
                            strcat(stringToAdd, "r_");
                            strcat(stringToAdd, strFromInt);
#ifdef DEBUG_ACTION
                            printf("      Reduction number being added: [%s]\n", strFromInt);
#endif
                            // sleep(1);
                            if (strcmp(action[idx[i]][getIndex(terminals, lookahead)], stringToAdd) != 0)
                                strcat(action[idx[i]][getIndex(terminals, lookahead)], stringToAdd);
                        }
                    }
                }
            }
        }
        // sleep(1);
    }

    printf("\n\nACTION table: \n");
    printf("    ");
    for (int i = 0; i < getSize(terminals); i++)
    {
        printf("%s ", terminals->values[i]);
    }
    printf("\n");

    for (int i = 0; i < final.curr; i++)
    {
        printf("%d   ", i);
        for (int j = 0; j < getSize(terminals); j++)
        {
            if (strlen(action[i][j]) > 0)
                printf("%s, ", action[i][j]);
            else
                printf("NULL, ");
        }
        printf("\n");
    }

    // exit(1);
    // goto table
    int gotoTable[final.curr][getSize(nonTerminals)];
    for (int i = 0; i < final.curr; i++)
    {
        for (int j = 0; j < getSize(nonTerminals); j++)
        {
            gotoTable[i][j] = -1;
            set tmp = GOTO(*sq.sets[i], nonTerminals->values[j], p, nonTerminals, terminals);
            for (int k = 0; k < sq.curr; k++)
            {
                if (areIdentical(&tmp, sq.sets[k]) == true)
                {
                    gotoTable[idx[i]][j] = idx[k];
                }
            }
        }
    }

    printf("\n\nGOTO Table: \n");
    printf("    ");
    for (int i = 0; i < getSize(nonTerminals); i++)
    {
        printf("%s ", nonTerminals->values[i]);
    }

    printf("\n");
    for (int i = 0; i < final.curr; i++)
    {
        printf("%d   ", i);
        for (int j = 0; j < getSize(nonTerminals); j++)
        {
            if (gotoTable[i][j] > -1)
                printf("%d ", gotoTable[i][j]);
            else
                printf("NULL ");
        }
        printf("\n");
    }

    // parse using the table
    char inp[MAX_UNIT_SIZE];
    printf("Enter the input string: ");
    fscanf(stdin, "%[^\n]s", inp);
    getchar();

    strcat(inp, " $");

    stack *s = (stack *)malloc(sizeof(stack));
    s->curr = -1;
    push(0, s);
    int counter = 0; // position on the input string
    char a[MAX_UNIT_SIZE];
    getToken(inp, a, &counter);
    int top = -1;
    while (1)
    {
        top = pop(s);
        push(top, s);
        char tmp[MAX_UNIT_SIZE];
        strcpy(tmp, action[top][getIndex(terminals, a)]);
#ifdef DEBUG_PARSING
        printf("In action[%d, %d] we have: ", top, getIndex(terminals, a));
        printf("%s\n", tmp);
#endif
        if (strcmp(tmp, "accept") == 0)
        {
            printf("The string has been accepted.\n");
            exit(0);
        }
        else if (tmp[0] == 'r')
        {
            // some reduction
            char num[50];
            num[0] = 0;
            for (int i = 2; i < strlen(tmp); i++)
            {
                strcat(num, &tmp[i]);
            }
            num[2] = 0;
            int no = atoi(num) - 1; // machine handling
            // get the head of this production
            char head[MAX_UNIT_SIZE];
            getHead(head, p->values[no]);
            char body[MAX_UNIT_SIZE];
            getBody(body, p->values[no]);
            for (int k = 0; k < numSpaces(body) + 1; k++)
            {
                int smthg = pop(s);
#ifdef DEBUG_PARSING
                printf("Popped: %d\n", smthg);
#endif
            }
            int topnum = pop(s);
            push(topnum, s);
            push(gotoTable[topnum][getIndex(nonTerminals, head)], s);
#ifdef DEBUG_PARSING
            printf("Pushed: %d\n", gotoTable[topnum][getIndex(nonTerminals, head)]);
#endif
            printf("[%s]\n", p->values[no]);
        }
        else if (tmp[0] == 's')
        {
            // some shifting
            char num[50];
            num[0] = 0;
            for (int i = 2; i < strlen(tmp); i++)
            {
                strcat(num, &tmp[i]);
            }
            num[2] = 0;
            int no = atoi(num);
#ifdef DEBUG_PARSING
            printf("Shifting to %d\n", no);
#endif
            push(no, s);
            if (strcmp(a, "$") != 0)
                getToken(inp, a, &counter);
        }
        else
        {
            printf("String cannot be parsed by this grammar.\n");
            exit(4);
        }
    }
}
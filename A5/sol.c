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

void getRecursive(char **dst, set *s, int *count)
{
    int curr = 0;
    for (int i = 0; i < getSize(s); i++)
    {
        int counter = 0;
        char tmp1[MAX_UNIT_SIZE];
        char tmp2[MAX_UNIT_SIZE];
        getToken(s->values[i], tmp1, &counter);
        getToken(s->values[i], tmp2, &counter);
        if (strcmp(tmp1, tmp2) == 0)
        {
            // if the head is the same as the first
            // on the RHS
            strcpy(dst[curr], s->values[i]);
            curr += 1;
        }
    }
    *count = curr;
}

void getNonRecursive(char **dst, set *s, int *count)
{
    int curr = 0;
    for (int i = 0; i < getSize(s); i++)
    {
        int counter = 0;
        char tmp1[MAX_UNIT_SIZE];
        char tmp2[MAX_UNIT_SIZE];
        getToken(s->values[i], tmp1, &counter);
        getToken(s->values[i], tmp2, &counter);
        if (strcmp(tmp1, tmp2) != 0)
        {
            // if the head is NOT the same as the first
            // on the RHS
            strcpy(dst[curr], s->values[i]);
            curr += 1;
        }
    }
    *count = curr;
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
        strcat(dst, tmp);
        strcat(dst, " ");
    }
    if (strlen(dst) > 0)
    {
        dst[strlen(dst) - 1] = 0; // remove the extra space at the end
    }
    return;
}

set cleanImmLR(set *s)
{

    printf("[INFO] Asked to remove immediate LR from the productions:\n");
    printSet(s);
    // remove immediate left recursion from
    // the given set of productions and
    // return the new set of productions
    set s1;
    s1.max_size = 2 * s->max_size;
    s1.values = (char **)malloc(s1.max_size * sizeof(char *));
    for (int i = 0; i < s1.max_size; i++)
    {
        s1.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    }

    int counter = 0;
    char *head = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    getToken(s->values[0], head, &counter);
    printf("[INFO] Head of all the passed productions: [%s]\n", head);

    // separate out the recursive and non-recursive
    // productions for future ease.

    char **rec = (char **)malloc(s->max_size * sizeof(char *));
    char **nonRec = (char **)malloc(s->max_size * sizeof(char *));
    for (int i = 0; i < getSize(s); i++)
    {
        rec[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        nonRec[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        rec[i][0] = 0;
        nonRec[i][0] = 0;
    }
    int *rsize = (int *)malloc(sizeof(int));
    *rsize = 0;
    int *nrsize = (int *)malloc(sizeof(int));
    *nrsize = 0;

    getRecursive(rec, s, rsize);
    getNonRecursive(nonRec, s, nrsize);

    printf("[INFO] Set of recursive productions: %d\n", *rsize);
    for (int i = 0; i < *rsize; i++)
        printf("[INFO]   %s\n", rec[i]);
    printf("[INFO] Set of non-recursive productions: %d\n", *nrsize);
    for (int i = 0; i < *nrsize; i++)
        printf("[INFO]   %s\n", nonRec[i]);
    // get the head of the first production in the passed set.
    // note that the head should be same for all the productions
    // in the set passed to this function.

    if (*rsize == 0)
    {
        printf("[INFO] No immediate LR found.\n");
        addSet(&s1, *s);
        printf("[INFO] Set being returned:\n");
        printSet(&s1);
        return s1;
    }

    for (int i = 0; i < *rsize; i++)
    {
        char *tmp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        char *body = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        getBody(tmp, rec[i]);
        getBody(body, tmp);

        char *toBeAdded = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        strcat(toBeAdded, head);
        strcat(toBeAdded, "' ");
        if (strcmp(body, "?") == 0)
            ;
        else
        {
            strcat(toBeAdded, body);
            strcat(toBeAdded, " ");
        }
        strcat(toBeAdded, head);
        strcat(toBeAdded, "'");
        printf("[INFO] Production being added: [%s]\n", toBeAdded);
        addElement(&s1, toBeAdded);
        // free(body);
        // free(toBeAdded);
        // free(tmp);
    }

    printf("[INFO] Non-recursive case.\n");
    for (int i = 0; i < *nrsize; i++)
    {
        char *body = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        getBody(body, nonRec[i]);
        if (strcmp(body, "?") == 0)
            continue;

        char *toBeAdded = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        strcat(toBeAdded, head);
        strcat(toBeAdded, " ");
        if (strcmp(body, "?") == 0)
            ;
        else
        {
            strcat(toBeAdded, body);
            strcat(toBeAdded, " ");
        }
        strcat(toBeAdded, head);
        strcat(toBeAdded, "'");
        printf("[INFO] Production being added: [%s]\n", toBeAdded);
        addElement(&s1, toBeAdded);
        // free(body);
        // free(toBeAdded);
    }

    // add the epsilon production
    char *toBeAdded = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    strcat(toBeAdded, head);
    strcat(toBeAdded, "' ");
    strcat(toBeAdded, "?");
    addElement(&s1, toBeAdded);
    free(toBeAdded);

    return s1;
}

bool recDescent(char **p, int n, char *tok, set *nonTerminals, set *terminals, char *inp, int *curr, int depth)
{
    // p -> productions
    // n -> number of productions
    // tok -> the current non-terminal being talked about
    // tried -> whether a production has been tried or not.
    // nonTerminals -> set of non-terminals
    // inp -> input string
    // curr -> curr index of inpur string where we are at during the parsing
    bool result = true;
    for (int i = 0; i < n; i++)
    {   
        char *head = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        getHead(head, p[i]);
        if (strcmp(head, tok) == 0)
        {
            result = true;
            // the production is picked
            printf("[%d:%s] The picked production: [%s]\n", depth, tok, p[i]);
            sleep(3);
            char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            int counter = 0;
            getToken(p[i], token, &counter); // ignore the head of the production

            while (getToken(p[i], token, &counter) != -1)
            {
                printf("[%d:%s] The next token: [%s]\n", depth, tok, token);
                // if the token is a non-terminal
                if (isPresent(nonTerminals, token) == true)
                {
                    bool res = recDescent(p, n, token, nonTerminals, terminals, inp, curr, depth+1);
                    if(res==true){
                        printf("[%d:%s] Production [%s] worked\n", depth, tok, p[i]);
                    }
                    else {
                        printf("[%d:%s] Production [%s] did NOT work\n", depth, tok, p[i]);
                        result = false;
                        break;
                    }
                }
                else {
                    // if it's equal to the current input symbol
                    // get the current input stream's token
                    char *itoken = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));

                    int retVal = getToken(inp, itoken, curr);
                    printf("[%d:%s] Input string's token to be matched: [%s]\n", depth, tok, itoken);
                    if (strcmp(token, itoken) == 0)
                    {   
                        printf("[%d:%s] Input token [%s] was consumed. Moving to the next input token.\n", depth, tok, itoken);
                    }
                    else if (strcmp(itoken, "$")==0){
                        if (strcmp(token, "?")==0)
                            *curr -= 1;
                        else{
                            *curr -= 1;
                            result = false;
                            printf("[%d:%s] This didn't work, trying (if there's) another production.\n", depth, tok);
                            break;
                        }

                    }
                    else if (strcmp(token, "?")==0){
                        *curr -= 1;
                    }
                    else
                    {
                        // revert the curr variable.
                        *curr -= 1;
                        result = false;
                        printf("[%d:%s] This didn't work, trying (if there's) another production.\n", depth, tok);
                        break;
                    }
                }
            }
            if (result==true) break;
        }
    }
    return result;
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

    set nonTerminals;
    set terminals;

    nonTerminals.max_size = n;
    nonTerminals.values = (char **)malloc(nonTerminals.max_size * sizeof(char *));
    if (nonTerminals.values == NULL)
        printf("MALLOC failed.\n");

    terminals.max_size = MAX_NUM_TERMINALS;
    terminals.values = (char **)malloc(terminals.max_size * sizeof(char *));
    if (terminals.values == NULL)
        printf("MALLOC failed.\n");

    char **p = (char **)malloc(n * sizeof(char *));

    for (int i = 0; i < nonTerminals.max_size; i++)
    {
        nonTerminals.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        if (nonTerminals.values[i] == NULL)

            printf("MALLOC failed.\n");
        nonTerminals.values[i][0] = '\0';
        // printf(":%s:", nonTerminals.values[i][0]);
    }
    for (int i = 0; i < terminals.max_size; i++)
    {
        terminals.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        if (terminals.values[i] == NULL)

            printf("MALLOC failed.\n");
        terminals.values[i][0] = '\0';
    }

    printf("Enter the productions one by one.\n");
    for (int i = 0; i < n; i++)
    {
#ifdef DEBUG
        printf("%dth: ", i);
#endif
        p[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));

        fscanf(stdin, "%[^\n]s", p[i]);
        getchar();
        p[i][strlen(p[i])] = '\0';
    }

    for (int i = 0; i < n; i++)
    {
        // printf("{%d}", strlen(p[i]));
        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        int counter = 0;
        getToken(p[i], token, &counter);
            addElement(&nonTerminals, token);
    }

    for (int i = 0; i < n; i++)
    {
        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        int counter = 0;
        getToken(p[i], token, &counter);
        while (getToken(p[i], token, &counter) != -1)
        {
            if (isPresent(&terminals, token) == false && isPresent(&nonTerminals, token) == false)
            {
                addElement(&terminals, token);
            }
        }
    }

    printf("The set of non-terminals:\n");
    printSet(&nonTerminals);

    printf("The set of terminals:\n");
    printSet(&terminals);

    set prods1;
    prods1.max_size = 100;
    prods1.values = (char **)malloc(prods1.max_size * sizeof(char *)); // some large number
    for (int i = 0; i < prods1.max_size; i++)
    {
        prods1.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        if (prods1.values[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for new productions' set.\n");
            exit(3);
        }
        prods1.values[i][0] = '\0';
    }

    // removing left recursion
    int numTerminals = getSize(&terminals);
    int numNonTerminals = getSize(&nonTerminals);
    for (int i = 0; i < numNonTerminals; i++)
    {
        printf("Non-terminal being considered: [%s]\n", nonTerminals.values[i]);
        set tempSet;
        tempSet.max_size = 100;
        tempSet.values = (char **)malloc(tempSet.max_size * sizeof(char *)); // some large number
        for (int j = 0; j < tempSet.max_size; j++)
        {
            tempSet.values[j] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            if (tempSet.values[j] == NULL)
            {
                fprintf(stderr, "Failed to allocate memory for new productions' set.\n");
                exit(4);
            }
            tempSet.values[j][0] = '\0';
        }
        for (int j = 0; j < i; j++)
        {
            printf("  That non-terminal being considered which came before it: [%s]\n", nonTerminals.values[j]);
            for (int k = 0; k < n; k++)
            {
                printf("    The production being considered: [%s]\n", p[k]);
                // check if there is a production of the form
                // Ai -> Aj<gamma>
                int counter = 0;
                char *tmp1 = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                char *tmp2 = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                getToken(p[k], tmp1, &counter);
                getToken(p[k], tmp2, &counter);
                if (strcmp(tmp1, nonTerminals.values[i]) == 0 &&
                    strcmp(tmp2, nonTerminals.values[j]) == 0)
                {
                    printf("      It's of the required form.\n");
                    // get the remaining part of the production i.e. gamma
                    char *gamma = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                    char *tmp4 = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                    gamma[0] = 0;
                    while (getToken(p[k], tmp4, &counter) != -1)
                    {
                        if (counter > 3)
                            strcat(gamma, " ");
                        strcat(gamma, tmp4);
                    }
                    // free(tmp4);
                    printf("      It's extracted gamma: [%s]\n", gamma);
                    // get all Aj's productions
                    for (int l = 0; l < n; l++)
                    {
                        int counter2 = 0;
                        char *head1 = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                        getToken(p[l], head1, &counter2);
                        if (strcmp(head1, nonTerminals.values[j]) == 0)
                        {

                            printf("        [%s]'s production being considered, to add gamma to it: [%s]\n", nonTerminals.values[j], p[l]);
                            // append gamma at the end of the current production
                            char *toBeAdded = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                            strcpy(toBeAdded, nonTerminals.values[i]);
                            strcat(toBeAdded, " ");
                            printf("         After adding head: [%s]\n", toBeAdded);
                            char *body = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                            getBody(body, p[l]);
                            strcat(toBeAdded, body);
                            printf("         After adding body: [%s]\n", toBeAdded);
                            if (gamma[0] != 0)
                            { // if gamma is not empty add gamma
                                strcat(toBeAdded, " ");
                                strcat(toBeAdded, gamma);
                                printf("         After adding gamma: [%s]\n", toBeAdded);
                            }
                            printf("        Production being added to the temp set for the non-terminal [%s]: [%s]\n", nonTerminals.values[i], toBeAdded);
                            // add the new production to the new set.
                            addElement(&tempSet, toBeAdded);
                            // free(toBeAdded);
                        }
                        // free(head1);
                    }
                }
                else
                {
                    if (strcmp(tmp1, nonTerminals.values[i]) == 0)
                    {
                        printf("      It's not of the required form\n");
                        printf("      Directly adding it to the temp set for the non-terminal [%s]\n", nonTerminals.values[i]);
                        // if not of that form, just copy it.
                        addElement(&tempSet, p[k]);
                    }
                }
                // free(tmp1);
                // free(tmp2);
            }
        }

        if (i == 0)
        {
            // add all those productions which were there in Ai originally
            printf("  Adding its productions that were present originally, to its temp set\n");
            for (int j = 0; j < n; j++)
            {
                char *head = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                getHead(head, p[j]);
                if (strcmp(head, nonTerminals.values[i]) == 0)
                {
                    printf("    Adding [%s]\n", p[j]);
                    addElement(&tempSet, p[j]);
                }
                // free(head);
            }
        }

        // remove immediate left recursion among the Ai productions.
        // then add the final thing to the new set of productions.

        addSet(&prods1, cleanImmLR(&tempSet));

        // for (int i = 0; i < tempSet.max_size ; i++)
        //     free(tempSet.values[i]);

        // free(tempSet.values);
    }
    printf("Grammar after left recursion is removed: \n");
    printSet(&prods1);

    for (int i = 0; i < n; i++)
    {
        // printf("{%d}", strlen(p[i]));
        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        int counter = 0;
        getToken(prods1.values[i], token, &counter);
            addElement(&nonTerminals, token);
    }

    printf("The new set of non-terminals:\n");
    printSet(&nonTerminals);

    printf("The new set of terminals:\n");
    printSet(&terminals);
    // recursive descent parser
    // exit(1);
    int **tried = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        tried[i] = (int *)malloc(sizeof(int));
        *tried[i] = 0;
    }

    int depth = 0;

    int *curr = (int *)malloc(sizeof(int));
    *curr = 0;

    char *inp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    printf("Enter the input string: ");
    fscanf(stdin, "%[^\n]s", inp);
    getchar();
    int len = strlen(inp);
    inp[len] = ' ';
    inp[len+1] = '$';
    inp[len+2] = '\0';
    bool res = recDescent(prods1.values, getSize(&prods1), start, &nonTerminals, &terminals, inp, curr, depth);
    if (res==true)
    {
        printf("String [%s] successfully parsed.\n", inp);
    }
    else 
        printf("String [%s] could not be parsed using the grammar.\n", inp);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
        printf("{");
        for (int i = 0; i < s->max_size; i++)
        {
            if (strcmp(s->values[i], "") != 0)
            {
                printf(" [%s] ", s->values[i]);
            }
        }
        printf("}\n");
    }
    return;
}

void addSet(set *dst, set src){

}

set cleanImmLR(set *s){

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

bool isFirstDone(char *nTerm, int *pDone, char **p, int n)
{
    char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    for (int i = 0; i < n; i++)
    {
        int counter = 0;
        getToken(p[i], token, &counter);
        if (strcmp(token, nTerm) == 0)
        {
            if (pDone[i] != 1)
            {
                free(token);
                return false;
            }
        }
    }
    free(token);
    return true;
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
    for (int i = 0; i < prods1.max_size; i++) {
        prods1.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        if (prods1.values[i]==NULL)
        {
            fprintf(stderr, "Failed to allocate memory for new productions' set.\n");
            exit(3);
        }
        prods1.values[i][0] = '\0';
    }

    // removing left recursion
    int numTerminals = getSize(&terminals);
    int numNonTerminals = getSize(&nonTerminals);
    for (int i=0;i<numNonTerminals;i++){
        set tempSet;
        tempSet.max_size = 100;
        tempSet.values = (char **)malloc(tempSet.max_size * sizeof(char *)); // some large number
        for (int i = 0; i < tempSet.max_size ; i++) {
            tempSet.values[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            if (tempSet.values[i]==NULL)
            {
                fprintf(stderr, "Failed to allocate memory for new productions' set.\n");
                exit(4);
            }
            tempSet.values[i][0] = '\0';
        }
        for (int j=0;j<i;j++){
            for(int k=0;k<n;k++){
                // check if there is a production of the form 
                // Ai -> Aj<gamma>
                int counter = 0;
                char *tmp1 = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                char *tmp2 = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                getToken(p[k], tmp1, counter);
                getToken(p[k], tmp2, counter);
                if (strcmp(tmp1, nonTerminals.values[i])==0 &&
                    strcmp(tmp2, nonTerminals.values[j])==0){
                        
                        // get the remaining part of the production i.e. gamma
                        char* gamma = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                        char *tmp4 = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                        gamma[0] = 0;
                        while(getToken(p[k], tmp4, counter)!=-1){
                            strcat(gamma, " ");
                            strcat(gamma, tmp4);
                        }
                        free(tmp4);

                        // get all Aj's productions
                        for (int l=0;l<n;l++){
                            int counter2 = 0;
                            char *tmp3 = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                            getToken(p[l], tmp3, counter2);
                            if (strcmp(tmp3, nonTerminals.values[j])==0){
                                
                                // append gamma at the end of the current production
                                char *tmp5 = (char*)malloc(MAX_UNIT_SIZE*sizeof(char));
                                strcpy(tmp5, p[l]);
                                if (gamma[0]!=0) // if gamma is not empty add gamma
                                    strcat(tmp5, gamma);
                                
                                // add the new production to the new set.
                                addElement(&tempSet, tmp5);
                                free(tmp5);
                            }
                            free(tmp3);
                        }
                    }
                else {
                    // if not of that form, just copy it.
                    addElement(&tempSet, p[k]);
                }



                free(tmp1);
                free(tmp2);
            }
        }
        // remove immediate left recursion among the Ai productions.
        // then add the final thing to the new set of productions.

        addSet(&prods1, cleanImmLR(&tempSet));
    
        for (int i = 0; i < tempSet.max_size ; i++)
            free(tempSet.values[i]);

        free(tempSet.values);
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_UNIT_SIZE 100
#define MAX_NUM_TERMINALS 50

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
    }

    printf("[ERROR] No space in the set to enter %s.\n", el);
    exit(1);
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
    printf("{");
    for (int i = 0; i < s->max_size; i++)
    {
        if (strcmp(s->values[i], "") != 0)
        {
            printf(" [%s] ", s->values[i]);
        }
    }
    printf("}\n");
    return;
}
// !! IMPORTANT: source must be NULL-terminated. !!
int getToken(char *source, char *tok, int *loc)
{
    int currSpaceNum = 0;
    int count = 0;
    // printf("value of loc: %d", *loc);
    // for(int i=0;;i++){
    //     if (source[i]!='\0')
    //     printf(":%c:", source[i]);
    //     else
    //     break;
    // }
    // exit(1);
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
    printf("Enter the number of productions: ");
    scanf("%d", &n);
    getchar();
    printf("Enter the productions one by one.\n");
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

    for (int i = 0; i < n; i++)
    {
        printf("%dth: ", i);
        p[i] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));

        fscanf(stdin, "%[^\n]s", p[i]);
        getchar();
        p[i][strlen(p[i])] = '\0';
        // strcpy(p_copy[i], p[i]);
    }

    for (int i = 0; i < n; i++)
    {
        // printf("{%d}", strlen(p[i]));
        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        int counter = 0;
        getToken(p[i], token, &counter);
        // printf("{%d}", strlen(token));
        // token[strlen(token)] = '\0';
        if (isPresent(&nonTerminals, token) == false)
            addElement(&nonTerminals, token);
    }

    for (int i = 0; i < n; i++)
    {
        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        // token[strlen(token)] = '\0';
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
    for (int i = 0; i < nonTerminals.max_size; i++)
    {
        if (nonTerminals.values[i][0] != '\0')
            printf("%s\n", nonTerminals.values[i]);
    }

    printf("The set of terminals:\n");
    for (int i = 0; i < terminals.max_size; i++)
    {
        if (terminals.values[i][0] != '\0')
            printf("%s\n", terminals.values[i]);
    }

    // creating the FIRST set for all the terminals
    int fSize = terminals.max_size + nonTerminals.max_size;
    set FIRST[fSize];
    // FIRST = (set*)malloc((terminals.max_size+nonTerminals.max_size)*sizeof(set));

    for (int i = 0; i < terminals.max_size; i++)
    {
        if (strcmp(terminals.values[i], "") != 0)
        {
            printf("Terminal under consideration: %s.\n", terminals.values[i]);
            FIRST[i].max_size = MAX_NUM_TERMINALS;
            FIRST[i].values = (char **)malloc((FIRST[i].max_size) * sizeof(char *));
            if (FIRST[i].values == NULL)
                printf("MALLOC failed.\n");
            for (int j = 0; j < FIRST[i].max_size; j++)
            {
                FIRST[i].values[j] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                if (FIRST[i].values[j] == NULL)
                    printf("MALLOC failed.\n");
                FIRST[i].values[j][0] = '\0';
            }
            addElement(&FIRST[i], terminals.values[i]);
        }
    }

    printf("FIRST set for terminals:\n");
    for (int i = 0; i < terminals.max_size; i++)
    {
        if (strcmp(terminals.values[i], "") != 0)
        {
            printf("%s:", terminals.values[i]);
            printSet(&FIRST[i]);
        }
    }

    // if the production is handled
    int pDone[n];
    for (int i = 0; i < n; i++)
        pDone[i] = 0;

    // creating the FIRST set for all the non-terminals
    for (int i = terminals.max_size; i < fSize; i++)
    {
        int nonTermNum = i - terminals.max_size;
        if (strcmp(nonTerminals.values[nonTermNum], "") != 0)
        {
            printf("Non-terminal whose FIRST is being declared: %s.\n", nonTerminals.values[nonTermNum]);
            FIRST[i].max_size = MAX_NUM_TERMINALS;
            FIRST[i].values = (char **)malloc(FIRST[i].max_size * sizeof(char *));
            if (FIRST[i].values == NULL)
                printf("MALLOC failed.\n");
            for (int j = 0; j < FIRST[i].max_size; j++)
            {
                FIRST[i].values[j] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                if (FIRST[i].values[j] == NULL)
                    printf("MALLOC failed.\n");
                FIRST[i].values[j][0] = '\0';
            }
        }
    }

    int isPLeft = true;
    while (isPLeft)
    {
        printf("Starting FIRST calculation.\n");
        for (int i = terminals.max_size; i < fSize; i++)
        {
            int nonTermNum = i - terminals.max_size;
            for (int j = 0; j < n; j++)
            {
                if (pDone[j] == 0)
                {
                    char *temp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                    int counter = 0;
                    getToken(p[j], temp, &counter);
                    // check if the head is the same as the current non-terminal at hand
                    if (strcmp(temp, nonTerminals.values[nonTermNum]) == 0)
                    {
                        printf("Production {%s}'s RHS's head matches non-term %s.\n", p[j], nonTerminals.values[nonTermNum]);
                        // if it's the same, go into the RHS of the production
                        while (getToken(p[j], temp, &counter) != -1)
                        {
                            if (isPresent(&terminals, temp))
                            {
                                printf("Token {%s} is a terminal symbol.\n", temp);
                                // if it is, add it first of that terminal to the first of the current non-terminal
                                printf("added {%s} to FIRST of {%s}.\n", temp, nonTerminals.values[nonTermNum]);
                                addElement(&FIRST[i], temp);
                                pDone[j] = 1;
                                break;
                            }
                            else
                            {
                                printf("Token {%s} is a non-terminal symbol.\n", temp);
                                if (isFirstDone(temp, pDone, p, n) == true)
                                {
                                    // FIRST is ready, so handle this.
                                    printf("Its FIRST creation is complete. Elements in its FIRST: \n");
                                    // if it is not, then find out it's FIRST set.
                                    int idx = getIndex(&nonTerminals, temp);
                                    int epsilonWasFound = false;
                                    printSet(&FIRST[terminals.max_size + idx]);
                                    for (int k = 0; k < MAX_NUM_TERMINALS; k++)
                                    {
                                        if (strcmp(FIRST[terminals.max_size + idx].values[k], "") != 0)
                                        {
                                            if (strcmp(FIRST[terminals.max_size + idx].values[k], "?") == 0)
                                            {
                                                // printf("epsilon condition.\n");
                                                epsilonWasFound = true;
                                                // check if this is the last symbol of the current production. Add epsilon only then.
                                                int check = getToken(p[j], temp, &counter);
                                                if (check == -1)
                                                {
                                                    printf("adding epsilon(?) to FIRST of {%s}.\n", nonTerminals.values[nonTermNum]);
                                                    addElement(&FIRST[i], FIRST[terminals.max_size + idx].values[k]);
                                                }
                                                else
                                                {
                                                    counter -= 2;                   // checking caused a change to the counter.
                                                    getToken(p[j], temp, &counter); // to revert temp
                                                }
                                            }
                                            else
                                            {
                                                printf("adding {%s} to FIRST of {%s}.\n", FIRST[terminals.max_size + idx].values[k], nonTerminals.values[nonTermNum]);
                                                addElement(&FIRST[i], FIRST[terminals.max_size + idx].values[k]);
                                            }
                                        }
                                    }
                                    // printf("I'm out.\n");
                                    if (epsilonWasFound == false)
                                    {
                                        // epsilon wasn't there in the FIRST of the current non-terminal
                                        // this means that we don't need to go further and we can mark
                                        // the current production as done.
                                        printf("Epsilon wasn't found in the FIRST of {%s}. So production {%s} is done.\n",
                                               temp, p[j]);
                                        pDone[j] = 1;
                                        break;
                                    }
                                    else if (epsilonWasFound == true)
                                    {
                                        int check = getToken(p[j], temp, &counter);
                                        // printf("current symbol: {%s} check value: %d.\n", temp, check);
                                        if (check == -1)
                                        {
                                            printf("Epsilon was found in the FIRST of {%s}, but its the last symbol in production {%s}, so the production is done.\n",
                                                   temp, p[j]);
                                            // this is the last symbol in the production
                                            pDone[j] = 1;
                                            break;
                                        }
                                        else
                                        {
                                            counter -= 2;                   // checking caused a change to the counter.
                                            getToken(p[j], temp, &counter); // to revert temp
                                            printf("Epsilon was found in the FIRST of {%s}, but its not the last symbol in production {%s}.\n",
                                                   temp, p[j]);
                                        }
                                    }
                                }
                                else
                                {
                                    printf("FIRST of {%s} is not ready. Moving on.\n", temp);
                                    // cannot do anything so move on, as the FIRST is not ready,
                                    // so break out of this production
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        // check if any production is left to be Done.
        isPLeft = false;
        for (int i = 0; i < n; i++)
        {
            if (pDone[i] == 0)
            {
                isPLeft = true;
                break;
            }
        }
    }

    // // creating the FIRST set for all the non-terminals
    // for (int i = terminals.max_size; i < fSize; i++)
    // {
    //     int nonTermNum = i - terminals.max_size;
    //     if (strcmp(nonTerminals.values[nonTermNum], "") != 0)
    //     {
    //         printf("Non-terminal under consideration: %s.\n", nonTerminals.values[nonTermNum]);
    //         FIRST[i].max_size = MAX_NUM_TERMINALS;
    //         FIRST[i].values = (char **)malloc(FIRST[i].max_size * sizeof(char *));
    //         if (FIRST[i].values == NULL)
    //             printf("MALLOC failed.\n");
    //         for (int j = 0; j < FIRST[i].max_size; j++)
    //         {
    //             FIRST[i].values[j] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    //             if (FIRST[i].values[j] == NULL)
    //                 printf("MALLOC failed.\n");
    //             FIRST[i].values[j][0] = '\0';
    //         }
    //         // iterate over all the non-terminal's productions
    //         // and keep adding to its FIRST set accordingly
    //         for (int j = 0; j < n; j++)
    //         {
    //             // strtok the production
    //             char *temp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    //             int counter = 0;
    //             getToken(p[j], temp, &counter);
    //             // check if the head is the same as the current non-terminal at hand
    //             if (strcmp(temp, nonTerminals.values[nonTermNum]) == 0)
    //             {
    //                 printf("Production {%s}'s head matches non-term %s.\n", p[j], nonTerminals.values[nonTermNum]);
    //                 // if it's the same, go into the RHS of the production
    //                 while (getToken(p[j], temp, &counter) != -1)
    //                 {

    //                     // For every token on the RHS, check if it's in terminals.
    //                     if (isPresent(&terminals, temp))
    //                     {
    //                         printf("Token {%s} is in terminals\n", temp);
    //                         // if it is, add it first of that terminal to the first of the current non-terminal
    //                         addElement(&FIRST[i], temp);
    //                     }
    //                     else {
    //                         printf("Token {%s} is in Non-terminals. Will handle later.\n", temp);
    //                         nSkipped[j] = 1;
    //                     }
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }
    // bool allDone = false;
    // int attempt=1;
    // while(allDone == false){
    //     printf("Pass num: %d\n", attempt);
    //     for (int i = terminals.max_size; i < fSize; i++)
    //     {
    //         int nonTermNum = i - terminals.max_size;
    //         if (strcmp(nonTerminals.values[nonTermNum], "") != 0)
    //         {
    //             printf("Non-terminal under consideration: %s.\n", nonTerminals.values[nonTermNum]);
    //             // iterate over all the non-terminal's productions
    //             // and keep adding to its FIRST set accordingly
    //             if (isEmpty(&FIRST[i])==false)
    //             {
    //                 // FIRST set not empty i.e. was done in phase one
    //                 printf("FIRST set is already populated.\n");
    //             }
    //             else {
    //                 // FIRST[i] is empty, so fill now.
    //                 for (int j = 0; j < n; j++)
    //                 {
    //                     if (nSkipped[j]>0) // if the production wasn't handled
    //                     {
    //                         // strtok the production
    //                         char *temp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
    //                         int counter = 0;
    //                         getToken(p[j], temp, &counter);
    //                         // check if the head is the same as the current non-terminal at hand
    //                         if (strcmp(temp, nonTerminals.values[nonTermNum]) == 0)
    //                         {
    //                             printf("Production {%s}'s head matches non-term %s.\n", p[j], nonTerminals.values[nonTermNum]);
    //                             // if it's the same, go into the RHS of the production
    //                             while (getToken(p[j], temp, &counter) != -1)
    //                             {
    //                                 // For every token on the RHS, check if it's in terminals.
    //                                 if (isPresent(&terminals, temp))
    //                                 {
    //                                     printf("Token {%s} is in terminals\n", temp);
    //                                     // if it is, add it first of that terminal to the first of the current non-terminal
    //                                     addElement(&FIRST[i], temp);
    //                                     fDone[nonTermNum] = 1;
    //                                     break;
    //                                 }
    //                                 else
    //                                 {
    //                                     printf("Token {%s} is in Non-terminals\n", temp);
    //                                     // if it is not, then find out it's FIRST set.
    //                                     int idx = getIndex(&nonTerminals, temp);
    //                                     if (isEmpty(&FIRST[terminals.max_size+idx])==false){
    //                                         printf("Its first set has some element(s). Adding those.\n");
    //                                         // add every element of this non-terminal to the FIRST.
    //                                         int isEpsilonPresent = false;
    //                                         for(int k=0;k<MAX_NUM_TERMINALS;k++){
    //                                             if(strcmp(FIRST[terminals.max_size+idx].values[k], "")!=0){
    //                                                 addElement(&FIRST[i],FIRST[terminals.max_size+idx].values[k]);
    //                                                 if (strcmp(FIRST[terminals.max_size+idx].values[k], "?")==0){
    //                                                     // if epsilon is present, go on to the next non-terminal
    //                                                     printf("Epsilon was added. Going to the next symbol.\n");
    //                                                 }
    //                                                 else
    //                                                     // no epsilon, so break
    //                                                     break;
    //                                             }
    //                                         }
    //                                         fDone[nonTermNum] = 1;

    //                                     }
    //                                     else {
    //                                         // if that non-terminal's set is empty. do nothing.
    //                                         printf("Its FIRST set is empty. Leaving for now.\n");
    //                                         break;
    //                                     }

    //                                 }
    //                             }
    //                         }
    //                     }
    //                     // if it's not the same ignore
    //                     // while writing this, a thought - first find the FIRST of those non-terminals,
    //                 } // that have a terminal at the beginning of the RHS of their productions.
    //             }
    //         }
    //     }
    //     allDone = true;
    //     for(int i=0;i<nonTerminals.max_size;i++){
    //         if (fDone[i]==0) {
    //             allDone = false;
    //             break;
    //         }
    //     }
    // }

    printf("FIRST set for non-terminals:\n");
    for (int i = terminals.max_size; i < fSize; i++)
    {
        if (strcmp(nonTerminals.values[i - terminals.max_size], "") != 0)
        {
            printf("%s:", nonTerminals.values[i - terminals.max_size]);
            printSet(&FIRST[i]);
        }
    }
}
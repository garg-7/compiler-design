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
    printSet(&nonTerminals);

    printf("The set of terminals:\n");
    printSet(&terminals);

    // creating the FIRST set for all the terminals
    int fSize = terminals.max_size + nonTerminals.max_size;
    set FIRST[fSize];
    // FIRST = (set*)malloc((terminals.max_size+nonTerminals.max_size)*sizeof(set));

    for (int i = 0; i < terminals.max_size; i++)
    {
        if (strcmp(terminals.values[i], "") != 0)
        {
            #ifdef DEBUG
                printf("Terminal under consideration: %s.\n", terminals.values[i]);
            #endif
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
            #ifdef DEBUG
                printf("Non-terminal whose FIRST is being declared: %s.\n", nonTerminals.values[nonTermNum]);
            #endif
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
        #ifdef DEBUG
            printf("Starting FIRST calculation.\n");
        #endif
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
                        #ifdef DEBUG
                            printf("Production {%s}'s RHS's head matches non-term %s.\n", p[j], nonTerminals.values[nonTermNum]);
                        #endif
                        // if it's the same, go into the RHS of the production
                        while (getToken(p[j], temp, &counter) != -1)
                        {
                            if (isPresent(&terminals, temp))
                            {
                                #ifdef DEBUG
                                    printf("Token {%s} is a terminal symbol.\n", temp);
                                    // if it is, add it first of that terminal to the first of the current non-terminal
                                    printf("added {%s} to FIRST of {%s}.\n", temp, nonTerminals.values[nonTermNum]);
                                #endif
                                addElement(&FIRST[i], temp);
                                pDone[j] = 1;
                                break;
                            }
                            else
                            {
                                #ifdef DEBUG
                                    printf("Token {%s} is a non-terminal symbol.\n", temp);
                                #endif
                                if (isFirstDone(temp, pDone, p, n) == true)
                                {
                                    // FIRST is ready, so handle this.
                                    int idx = getIndex(&nonTerminals, temp);
                                    int epsilonWasFound = false;
                                    #ifdef DEBUG
                                        printf("Its FIRST creation is complete. Elements in its FIRST: \n");
                                        printSet(&FIRST[terminals.max_size + idx]);
                                    #endif
                                    // if it is not, then find out it's FIRST set.
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
                                                    #ifdef DEBUG
                                                        printf("adding epsilon(?) to FIRST of {%s}.\n", nonTerminals.values[nonTermNum]);
                                                    #endif
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
                                                #ifdef DEBUG
                                                    printf("adding {%s} to FIRST of {%s}.\n", FIRST[terminals.max_size + idx].values[k], nonTerminals.values[nonTermNum]);
                                                #endif
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
                                        #ifdef DEBUG
                                            printf("Epsilon wasn't found in the FIRST of {%s}. So production {%s} is done.\n",
                                               temp, p[j]);
                                        #endif
                                        pDone[j] = 1;
                                        break;
                                    }
                                    else if (epsilonWasFound == true)
                                    {
                                        int check = getToken(p[j], temp, &counter);
                                        // printf("current symbol: {%s} check value: %d.\n", temp, check);
                                        if (check == -1)
                                        {
                                            #ifdef DEBUG
                                                printf("Epsilon was found in the FIRST of {%s}, but its the last symbol in production {%s}, so the production is done.\n",
                                                   temp, p[j]);
                                            #endif
                                            // this is the last symbol in the production
                                            pDone[j] = 1;
                                            break;
                                        }
                                        else
                                        {
                                            counter -= 2;                   // checking caused a change to the counter.
                                            getToken(p[j], temp, &counter); // to revert temp
                                            #ifdef DEBUG
                                                printf("Epsilon was found in the FIRST of {%s}, but its not the last symbol in production {%s}.\n",
                                                      temp, p[j]);
                                            #endif
                                        }
                                    }
                                }
                                else
                                {
                                    #ifdef DEBUG
                                        printf("FIRST of {%s} is not ready. Moving on.\n", temp);
                                    #endif
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

    printf("FIRST set for non-terminals:\n");
    for (int i = terminals.max_size; i < fSize; i++)
    {
        if (strcmp(nonTerminals.values[i - terminals.max_size], "") != 0)
        {
            printf("%s:", nonTerminals.values[i - terminals.max_size]);
            printSet(&FIRST[i]);
        }
    }

    // FOLLOW() for terminals and non-terminals.
    set FOLLOW[getSize(&nonTerminals)];
    int foSize = getSize(&nonTerminals);

    int folDone[getSize(&nonTerminals)];
    int folProd[getSize(&nonTerminals)][n];

    for (int i = 0; i < foSize; i++)
    {
        folDone[i] = 0;
        for (int j = 0; j < n; j++)
        {
            folProd[i][j] = 0;
            char *temp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            int counter = 0;
            getToken(p[j], temp, &counter);
            while (getToken(p[j], temp, &counter) != -1)
            {
                // entering the RHS of the production.
                if (strcmp(temp, nonTerminals.values[i]) == 0)
                {
                    // Found on the RHS of jth production
                    folProd[i][j] = 1;
                    break;
                }
            }
        }
    }
    #ifdef DEBUG
        for (int i = 0; i < foSize; i++)
        {
            printf("%s:", nonTerminals.values[i]);
            for (int j = 0; j < n; j++)
            {
                printf(" %d", folProd[i][j]);
            }
            printf("\n");
        }
    #endif

    // creating the FOLLOW set for all the non-terminals
    for (int i = 0; i < foSize; i++)
    {
        if (strcmp(nonTerminals.values[i], "") != 0)
        {
            #ifdef DEBUG
                printf("Non-terminal whose FOLLOW is being declared: %s.\n", nonTerminals.values[i]);
            #endif
            FOLLOW[i].max_size = MAX_NUM_TERMINALS;
            FOLLOW[i].values = (char **)malloc(FOLLOW[i].max_size * sizeof(char *));
            if (FOLLOW[i].values == NULL)
                printf("MALLOC failed.\n");
            for (int j = 0; j < FOLLOW[i].max_size; j++)
            {
                FOLLOW[i].values[j] = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                if (FOLLOW[i].values[j] == NULL)
                    printf("MALLOC failed.\n");
                FOLLOW[i].values[j][0] = '\0';
            }
        }
    }

    // add input endmarker to the FOLLOW of the start state.
    int startIdx = getIndex(&nonTerminals, start);
    addElement(&FOLLOW[startIdx], "$");

    #ifdef DEBUG
        printf("Starting 1st pass.\n");
    #endif
    for (int i = 0; i < foSize; i++)
    {
        bool dependencyLeft = false;
        int found = 0;
        #ifdef DEBUG
            printf("Non-terminal: {%s}.\n", nonTerminals.values[i]);
        #endif
        for (int j = 0; j < n; j++)
        {
            int counter = 0;
            // head of the current production
            char *head = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            getToken(p[j], head, &counter);
            int headIdx = getIndex(&nonTerminals, head);
            #ifdef DEBUG
                printf("Production: {%s}.\n", p[j]);
                printf("Head: {%s}.\n", head);
            #endif
            char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
            while (getToken(p[j], token, &counter) != -1)
            {
                if (strcmp(token, nonTerminals.values[i]) == 0)
                {
                    // found in RHS of jth production
                    found = 1;
                    // check if its the last symbol
                    int check = getToken(p[j], token, &counter);
                    if (check == -1)
                    {
                        if (strcmp(token, head)!=0)
                        {
                            #ifdef DEBUG
                                printf("{%s} is at the end of the production, will have to wait.\n", token);
                            #endif
                            // add everything from FOLLOW(head) to FOLLOW(token)
                            dependencyLeft = true;
                        }
                        else
                        {
                            #ifdef DEBUG
                                printf("{%s} is at the end of the production, but also at the head, so production is done.\n", token);
                            #endif
                            folProd[i][j]--;
                        }
                    }
                    else // it's not the last symbol
                    {
                        // revert the changes made due to the check
                        counter -= 2;                                 // checking caused a change to the counter.
                        getToken(p[j], token, &counter);              // to revert temp
                        while (getToken(p[j], token, &counter) != -1) // get the next token
                        {
                            int idx = -1;
                            if (isPresent(&terminals, token))
                            {
                                idx = getIndex(&terminals, token);
                                #ifdef DEBUG
                                    printf("Terminal {%s}'s index (in FIRST): %d.\n", token, idx);
                                #endif
                            }
                            else
                            {
                                idx = terminals.max_size + getIndex(&nonTerminals, token);
                                #ifdef DEBUG
                                    printf("Non-terminal {%s}'s index (in FIRST): %d.\n", token, idx);
                                #endif
                            }
                            // next is a terminal
                            bool isEpsilonPresent = false;
                            #ifdef DEBUG
                                printf("Adding FIRST of it to the FOLLOW.\n");
                            #endif
                            for (int k = 0; k < MAX_NUM_TERMINALS; k++)
                            {
                                if (strcmp(FIRST[idx].values[k], "") != 0)
                                {
                                    if (strcmp(FIRST[idx].values[k], "?") == 0)
                                    {
                                        isEpsilonPresent = true;
                                    }
                                    else
                                    {
                                        #ifdef DEBUG
                                            printf("Adding {%s} to the FOLLOW of {%s}.\n", FIRST[idx].values[k], nonTerminals.values[i]);
                                        #endif
                                        addElement(&FOLLOW[i], FIRST[idx].values[k]);
                                    }
                                }
                            }
                            if (isEpsilonPresent == false)
                            {
                                #ifdef DEBUG
                                    printf("{%s} does not have epsilon in its FIRST set.\n", token);
                                #endif
                                folProd[i][j]--;
                                counter = 10000;
                                break;
                            }
                            else if (isEpsilonPresent == true)
                            {
                                int check = getToken(p[j], token, &counter);
                                if (check == -1)
                                {
                                    // it is the last token and there was an epsilon, so
                                    // FOLLOW dependency on head.
                                    dependencyLeft = true;
                                    #ifdef DEBUG
                                        printf("{%s} has epsilon in its FIRST set and is at the end, so FOLLOW dependency on head {%s}.\n", token, head);
                                    #endif
                                }
                                else
                                {
                                    // revert the changes made due to the check
                                    counter -= 2;                    // checking caused a change to the counter.
                                    getToken(p[j], token, &counter); // to revert temp
                                    #ifdef DEBUG
                                        printf("{%s} has epsilon in its FIRST set but is NOT at the end.\n", token);
                                    #endif
                                }
                            }
                        }
                    }
                }
            }
        }
        if (found == 0)
        {
            // the nonterminal wasn't found anywhere in the RHS.
            // its FOLLOW is done.
            folDone[i] = true;
        }
        if (dependencyLeft == false)
            folDone[i] = true;
    }
    #ifdef DEBUG
        printf("1st pass for FOLLOW sets done.\n");
        for (int i = 0; i < foSize; i++)
        {
            printf("%s:", nonTerminals.values[i]);
            for (int j = 0; j < n; j++)
            {
                printf(" %d", folProd[i][j]);
            }
            printf("\n");
        }
    #endif

    bool isFolLeft = false;
    // exit(1);
    for (int i = 0; i < foSize; i++)
    {
        if (folDone[i] == 0)
        {
            isFolLeft = true;
            break;
        }
    }

    bool progressMade = true;
    while (isFolLeft && progressMade)
    {
        progressMade = false;
        // loop over every non-terminal and try to find it in the RHS of
        // some production
        for (int i = 0; i < foSize; i++)
        {
            if (folDone[i] == false)
            {
                #ifdef DEBUG
                    printf("Non-terminal: {%s}.\n", nonTerminals.values[i]);
                #endif
                // FOLLOW set for ith non terminal is not done.
                for (int j = 0; j < n; j++)
                {
                    if (folProd[i][j] == 1)
                    {
                        // jth production has to be consulted for ith non terminal's FOLLOW creation.
                        int counter = 0;
                        // head of the current production
                        char *head = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                        getToken(p[j], head, &counter);
                        int headIdx = getIndex(&nonTerminals, head);
                        
                        #ifdef DEBUG
                            printf("Considering production: %s.\n", p[j]);
                            printf("Head: {%s}.\n", head);
                        #endif

                        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                        while (getToken(p[j], token, &counter) != -1)
                        {
                            if (strcmp(token, nonTerminals.values[i]) == 0)
                            {
                                // check if its the last symbol
                                int check = getToken(p[j], token, &counter);
                                if (check == -1)
                                {
                                    if(strcmp(token, head)!=0)
                                    {
                                        // head is different from the current non-terminal
                                        // add everything from FOLLOW(head) to FOLLOW(token)
                                        if (folDone[headIdx] == 0)
                                        {
                                            // will come to this production again.
                                            #ifdef DEBUG
                                                printf("FOLLOW of head {%s} is not ready. Will visit again.\n", head);
                                                printf("For now, adding the elements in FOLLOW of head till now:\n");
                                                printSet(&FOLLOW[headIdx]);
                                            #endif
                                            if (isEmpty(&FOLLOW[headIdx])==false)
                                            {
                                                for (int k = 0; k < MAX_NUM_TERMINALS; k++)
                                                {
                                                    if (strcmp(FOLLOW[headIdx].values[k], "") != 0)
                                                        addElement(&FOLLOW[i], FOLLOW[headIdx].values[k]);
                                                }
                                            }
                                        }
                                        else
                                        {
                                            #ifdef DEBUG
                                                printf("FOLLOW of head {%s} is complete and has the following elements: \n", head);
                                                printSet(&FOLLOW[headIdx]);
                                                printf("Adding these to the FOLLOW of {%s}.\n", nonTerminals.values[i]);
                                            #endif
                                            // simply add all the FOLLOW elements
                                            for (int k = 0; k < MAX_NUM_TERMINALS; k++)
                                            {
                                                if (strcmp(FOLLOW[headIdx].values[k], "") != 0)
                                                    addElement(&FOLLOW[i], FOLLOW[headIdx].values[k]);
                                            }
                                            folProd[i][j]--;
                                            progressMade = true;
                                            #ifdef DEBUG
                                                printf("Production %d is done for nonterminal {%s}.\n", j, nonTerminals.values[i]);
                                            #endif
                                        }
                                    }
                                    else
                                    {
                                        #ifdef DEBUG
                                            printf("Head is the same as the current nonterminal {%s} under consideration. So this production is done.\n", head);
                                        #endif
                                        folProd[i][j]--;
                                        progressMade = true;
                                    }

                                }
                                else // it's not the last symbol
                                {
                                    // revert the changes made due to the check
                                    counter -= 2;                                 // checking caused a change to the counter.
                                    getToken(p[j], token, &counter);              // to revert temp
                                    while (getToken(p[j], token, &counter) != -1) // get the next token
                                    {
                                        int idx = -1;
                                        if (isPresent(&terminals, token))
                                        {
                                            idx = getIndex(&terminals, token);
                                            #ifdef DEBUG
                                                printf("Terminal {%s}'s index (in FIRST): %d.\n", token, idx);
                                            #endif
                                        }
                                        else
                                        {
                                            idx = terminals.max_size + getIndex(&nonTerminals, token);
                                            #ifdef DEBUG
                                                printf("Non-terminal {%s}'s index (in FIRST): %d.\n", token, idx);
                                            #endif
                                        }
                                        bool isEpsilonPresent = false;
                                        #ifdef DEBUG
                                            printf("Adding FIRST of it to the FOLLOW.\n");
                                        #endif
                                        for (int k = 0; k < MAX_NUM_TERMINALS; k++)
                                        {
                                            if (strcmp(FIRST[idx].values[k], "") != 0)
                                            {
                                                if (strcmp(FIRST[idx].values[k], "?") == 0)
                                                {
                                                    isEpsilonPresent = true;
                                                }
                                                else
                                                {
                                                    #ifdef DEBUG
                                                        printf("Adding {%s} to the FOLLOW of {%s}.\n", FIRST[idx].values[k], nonTerminals.values[i]);
                                                    #endif
                                                    addElement(&FOLLOW[i], FIRST[idx].values[k]);
                                                }
                                            }
                                        }
                                        if (isEpsilonPresent == false)
                                        {
                                            #ifdef DEBUG
                                                printf("{%s} does not have epsilon in its FIRST set.\n", token);
                                                printf("Production %d is done for nonterminal {%s}.\n", j, nonTerminals.values[i]);
                                            #endif
                                            folProd[i][j]--;
                                            progressMade = true;
                                            counter = 10000;
                                            break;
                                        }
                                        else if (isEpsilonPresent == true)
                                        {
                                            int check = getToken(p[j], token, &counter);
                                            if (check == -1)
                                            {
                                                // it is the last token and there was an epsilon, so
                                                // FOLLOW dependency on head.
                                                #ifdef DEBUG
                                                    printf("{%s} has epsilon in its FIRST set and is at the end, so FOLLOW dependency on head {%s}.\n", token, head);
                                                #endif
                                                // add everything from FOLLOW(head) to FOLLOW(token)
                                                if (folDone[headIdx] == 0)
                                                {
                                                    // will come to this production again.
                                                    #ifdef DEBUG
                                                    printf("FOLLOW of head {%s} is not ready. Will visit again.\n", head);
                                                    #endif
                                                }
                                                else
                                                {
                                                    #ifdef DEBUG
                                                        printf("FOLLOW of head {%s} is complete and has the following elements: \n", head);
                                                        printSet(&FOLLOW[headIdx]);
                                                        printf("Adding these to the FOLLOW of {%s}.\n", nonTerminals.values[i]);
                                                    #endif
                                                    // simply add all the FOLLOW elements
                                                    for (int k = 0; k < MAX_NUM_TERMINALS; k++)
                                                    {
                                                        if (strcmp(FOLLOW[headIdx].values[k], "") != 0)
                                                            addElement(&FOLLOW[i], FOLLOW[headIdx].values[k]);
                                                    }
                                                    folProd[i][j]--;
                                                    progressMade = true;
                                                    #ifdef DEBUG
                                                        printf("Production %d is done for nonterminal {%s}.\n", j, nonTerminals.values[i]);
                                                    #endif
                                                }
                                            }
                                            else
                                            {
                                                // revert the changes made due to the check
                                                counter -= 2;                    // checking caused a change to the counter.
                                                getToken(p[j], token, &counter); // to revert temp
                                                #ifdef DEBUG
                                                    printf("{%s} has epsilon in its FIRST set but is NOT at the end.\n", token);
                                                #endif
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            for (int j = 0; j < n; j++)
            {
                if (folProd[i][j] == 1)
                {
                    // if even a single production from which FOLLOW
                    // of ith nonterminal needs to be calculated is left to be consulted,
                    // break and say that the nonterminal's FOLLOW is not done.
                    folDone[i] = false;
                    break;
                }
                folDone[i] = true;
            }
            #ifdef DEBUG
                if(folDone[i]==true)
                {
                    printf("FOLLOW set of {%s} is done.\n", nonTerminals.values[i]);
                }
                else
                {
                    printf("FOLLOW set of {%s} is unfinished.\n", nonTerminals.values[i]);
                }
            #endif
        }

        isFolLeft = false;
        for (int i = 0; i < foSize; i++)
        {
            if (folDone[i] == false)
            {
                // if even a single nonterminal's FOLLOW is not done.
                // do another pass.
                isFolLeft = true;
                break;
            }
        }
    }

    printf("FOLLOW set for non-terminals:\n");
    for (int i = 0; i < foSize; i++)
    {
        if (strcmp(nonTerminals.values[i], "") != 0)
        {
            printf("%s:", nonTerminals.values[i]);
            printSet(&FOLLOW[i]);
        }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_UNIT_SIZE 100

typedef struct set
{
    char **values;
    int max_size;
} set;

int checkPresence(set *s, char *el)
{
    printf("Checking for %s.\n", el);
    for (int i = 0; i < (s->max_size); i++)
    {
        if (strcmp(s->values[i], el) == 0)
        {
            return true;
        }
    }
    return false;
}

void addElement(set *s, char *el)
{
    // printf("Adding %s.\n", el);
    // printf("%d\n", s->max_size);
    for (int i = 0; i < (s->max_size); i++)
    {
        if (s->values[i][0]=='\0')
        {
            strcpy(s->values[i], el);
            printf("%s added.\n", el);
            return;
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

    terminals.max_size = MAX_UNIT_SIZE;
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
    }

    for(int i=0;i<n;i++){
        // printf("{%d}", strlen(p[i]));
        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        token = strtok(p[i], "->");
        // printf("{%d}", strlen(token));
        token[strlen(token)] = '\0';
        if (checkPresence(&nonTerminals, token) == false)
            addElement(&nonTerminals, token);
    }
    printf("FINEc.\n");
    strtok("sdcd ew ecwe",              " ");
    for(int i=0;i<n;i++){
        int count = 0;
        char *token = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
        printf("FINE.\n");
        token = strtok(p[i], "->");
        printf("{%d}", strlen(token));
        token[strlen(token)] = '\0';
        while (token != NULL)
        {
            token = strtok(NULL, " ");
            if (count == 0)
            {
                char *temp = (char *)malloc(MAX_UNIT_SIZE * sizeof(char));
                // printf("%ld: \n", strlen(token));
                for (int k = 1; k < strlen(token); k++)
                    temp[k - 1] = token[k];
                temp[strlen(token) - 1] = '\0';
                printf("Corrected first: %s\n", temp);
                if (checkPresence(&terminals, temp) == false && checkPresence(&nonTerminals, temp)==false)
                {
                    addElement(&terminals, temp);
                }
                count += 1;
            }
            else
            {
                if (token != NULL)
                {
                    if (checkPresence(&terminals, token) == false && checkPresence(&nonTerminals, token)==false)
                    {
                        addElement(&terminals, token);
                    }
                }
            }
            // printf(":%s:", token);
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
    free(nonTerminals.values);
    free(terminals.values);

    // calculating the FIRST set for all the non-terminals
}
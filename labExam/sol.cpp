#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

#include <set>
#include <algorithm>

#define ENTRY 7
#define EXIT 8
// #define DEBUG

using namespace std;

typedef struct basicBlock
{
    char **lines;
    int *succ;
} BB;

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

int main()
{
    // printf("Enter the num of BB: ");
    int b = 7 + 2; // 2 for entry and exit

    printf("Number of basic blocks: %d\n", b);
    BB *blocks = (BB *)malloc(b * sizeof(BB));
    for (int i = 0; i < b; i++)
    {
        blocks[i].lines = (char **)malloc(10 * sizeof(char *));
        blocks[i].succ = (int *)malloc(10 * sizeof(int));
        for (int j = 0; j < 10; j++)
        {
            blocks[i].succ[j] = -1;
            blocks[i].lines[j] = (char *)malloc(50 * sizeof(char));
            blocks[i].lines[j][0] = 0;
        }
    }

    strcpy(blocks[0].lines[0], "i = 0");
    strcpy(blocks[0].lines[1], "b = 0");
    strcpy(blocks[0].lines[2], "x = p");
    blocks[0].succ[0] = 1;

    strcpy(blocks[1].lines[0], "if ( i < p ) GOTO L1");
    blocks[1].succ[0] = 2;
    blocks[1].succ[1] = 3;

    strcpy(blocks[2].lines[0], "b = b + 1");
    strcpy(blocks[2].lines[1], "x = 0");
    blocks[2].succ[0] = 8; // exit

    strcpy(blocks[3].lines[0], "t = a + 1");
    strcpy(blocks[3].lines[1], "b = t");
    strcpy(blocks[3].lines[2], "if ( a == b ) GOTO L2");
    blocks[3].succ[0] = 4;
    blocks[3].succ[1] = 5;

    strcpy(blocks[4].lines[0], "a = x + 1");
    blocks[4].succ[0] = 6;

    strcpy(blocks[5].lines[0], "a = x - 1");
    blocks[5].succ[0] = 6;

    strcpy(blocks[6].lines[0], "i = i + 1");
    strcpy(blocks[6].lines[1], "GOTO L3");
    blocks[6].succ[0] = 3;

    // note that blocks[7]:=ENTRY and blocks[8]:=EXIT
    blocks[7].succ = 0;

    // for (int i=0;i<b;i++)
    // {
    //     printf("BB: %d\n", i);
    //     for (int j=0;j<10;j++)
    //     {
    //         char tmp[100];
    //         strcpy(tmp, blocks[i].lines[j]);
    //         if (tmp[0]!=0)
    //             printf("%s\n", tmp);
    //         int tmp1 = -1;
    //         tmp1 = blocks[i].succ[j];
    //         if (tmp1!=-1)
    //             printf("%d\n", tmp1);
    //     }
    //     printf("\n");
    // }

    set<char> def[b];
    set<char> use[b];
    for (int i = 0; i < b; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (blocks[i].lines[j][0] == 0)
                break;

            int counter = 0;
            char tmp[50];

            getToken(blocks[i].lines[j], tmp, &counter);

            if (strcmp(tmp, "if") == 0)
            {
                while (getToken(blocks[i].lines[j], tmp, &counter) != -1)
                {
                    if (strlen(tmp) == 1 && isalpha(tmp[0]) != 0)
                    {
                        if (def[i].find(tmp[0]) != def[i].end() == true)
                            ;
                        else
                        {
                            use[i].insert(tmp[0]);
                        }
                    }
                }
            }
            else
            {
                if (strlen(tmp) == 1 && isalpha(tmp[0]))
                {
                    def[i].insert(tmp[0]);
                    char firstChar = tmp[0];
                    while (getToken(blocks[i].lines[j], tmp, &counter) != -1)
                    {
                        if (strlen(tmp) == 1 && isalpha(tmp[0]) != 0)
                        {
                            if (def[i].find(tmp[0]) != def[i].end() == true)
                            {
                                if (tmp[0] == firstChar)
                                {
                                    // if also the first character then
                                    // insert in use too
                                    use[i].insert(tmp[0]);
                                }
                            }
                            else
                            {
                                // not previously defined in this block
                                // so add to use
                                use[i].insert(tmp[0]);
                            }
                        }
                    }
                }

                else
                {
                    // some GOTO kind of a statement
                    // so skipping i.e. nothing to do
                }
            }
        }
    }

    for (int i = 0; i < b - 2; i++)
    {
        printf("Basic block %d: \n", i);
        set<char>::iterator t;
        printf("  The use set: ");
        for (t = use[i].begin(); t != use[i].end(); ++t)
        {
            printf("%c ", *t);
        }
        printf("\n");

        printf("  The def set: ");
        for (t = def[i].begin(); t != def[i].end(); ++t)
        {
            printf("%c ", *t);
        }
        printf("\n\n");
    }

    set<char> in[b];
    set<char> out[b];

    // the liveness analysis algorithm to find out the
    // IN and OUT for all the basic blocks

    bool changeHappened = true;
    int iteration = 1;
    while (changeHappened == true)
    {
#ifdef DEBUG
                printf("----------Iteration: %d------------\n", iteration++);
#endif
        changeHappened = false;
        for (int i = 0; i < b; i++)
        {
            if (i != EXIT && i != ENTRY)
            {

#ifdef DEBUG
                printf("block %d\n", i);
#endif

                // OUT calculation

                // iterate over all the successors
                set<char> tmp;
                for (int j = 0; j < 10; j++)
                {
                    if (blocks[i].succ[j] == -1)
                        break;

                    // add the successor's IN elements to the out of
                    // the current basic block
                    set<char>::iterator t;
                    for (t = in [blocks[i].succ[j]].begin(); t != in[blocks[i].succ[j]].end(); ++t)
                    {
                        tmp.insert(*t);
                    }
                }

                out[i] = tmp;

                set<char>::iterator t;
#ifdef DEBUG
                printf("  OUT: \n");
                for (t = out[i].begin(); t != out[i].end(); ++t)
                {
                    printf("    %c ", *t);
                }
                printf("\n");
#endif
                // IN calculation
                set<char> prevIN = in[i];

#ifdef DEBUG
                printf("  IN set before\n");
                for (t = in[i].begin(); t != in[i].end(); ++t)
                {
                    printf("    %c ", *t);
                }
                printf("\n");

#endif
                in[i] = use[i];
                // difference between OUT and def for the basic blocks
                set<char> diff;
                set_difference(out[i].begin(), out[i].end(), def[i].begin(), def[i].end(), inserter(diff, diff.end()));

                // set<char>::iterator t;
                for (t = diff.begin(); t != diff.end(); ++t)
                {
                    in[i].insert(*t);
                }
#ifdef DEBUG
                printf("  IN set after\n");

                for (t = in[i].begin(); t != in[i].end(); ++t)
                {
                    printf("    %c ", *t);
                }
                printf("\n");

#endif
                if (prevIN.size() < in[i].size())
                    changeHappened = true;
            }
        }
    }

    for (int i = 0; i < b - 2; i++)
    {
        printf("Basic block %d:\n", i);
        set<char>::iterator t;
        printf("  The IN set: ");
        for (t = in[i].begin(); t != in[i].end(); ++t)
        {
            printf("%c ", *t);
        }
        printf("\n");

        printf("  The OUT set: ");
        for (t = out[i].begin(); t != out[i].end(); ++t)
        {
            printf("%c ", *t);
        }
        printf("\n\n");
    }
}
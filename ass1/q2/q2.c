#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"

#define MAX_LINE_LENGTH 1024 /* Max number of characters per line.*/
#define YES 1
#define NO 0

/* Returns the first non-space character's index starting from `start` */
int getNonSpace(char *buffer, int start)
{
    for (int i = start; i < strlen(buffer); i++)
    {
        if (buffer[i] != ' ')
            return i;
    }

    // rest of the line is all spaces, return -1.
    return -1;
}

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        printf("Please pass the code file as an argument.\n");
        exit(1);
    }

    else if (argc > 2)
    {
        printf("Too many arguments. I just need a single C source code file.\n");
        exit(2);
    }

    else
    {
        FILE *f = fopen(argv[1], "r");
        if (f < 0)
        {
            printf("Unable to open passed file.\n");
            exit(3);
        }

        FILE *fout = fopen("noComm.txt", "w");
        int *isComment = (int *)malloc(sizeof(int));
        *isComment = NO;
        while (1)
        {
            char *singleLine = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
            char *check;
            check = fgets(singleLine, MAX_LINE_LENGTH, f);
            // note: fgets reads in the newline and adds a NULL at the end
            if (check == NULL)
            {
                // EOF reached.
                break;
            }

            // count return includes \n but not \0
            // printf("%ld\n", strlen(singleLine));
            // exit(0);
            // printf("lp: [%s]\n", singleLine);
            if (*isComment == NO)
            {
                // no pending */ to look for, start processing line from the beginning.
                processLine(singleLine, 0, fout, isComment);
            }
            else
            {
                // look for pending */
                int idx = getCommentEnd(singleLine, 0);
                if (idx == -1)
                {
                    // the entire line is a comment
                    // read the next line
                    *isComment = YES; // next line also begins as a comment.
                }
                else
                {
                    // start processing from where the comment ends
                    *isComment = NO; // next line not a comment.
                    processLine(singleLine, idx, fout, isComment);
                }
            }
        }

        // comments have been removed.
        // now read the intermediate file and remove the extra spaces.
        fclose(f);
        fclose(fout);
        f = fopen("noComm.txt", "r");
        fout = fopen("output.txt", "w");

        int *isWithinString = (int *)malloc(sizeof(int));
        *isWithinString = NO;
        while (1)
        {
            char *singleLine = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
            char *check;
            check = fgets(singleLine, MAX_LINE_LENGTH, f);
            // note: fgets reads in the newline and adds a NULL at the end
            if (check == NULL)
            {
                // EOF reached.
                break;
            }

            // count return includes \n but not \0
            // printf("%ld\n", strlen(singleLine));
            // exit(0);
            printf("lp: [%s]\n", singleLine);

            for (int i = 0; i < strlen(singleLine); i++)
            {
                if (singleLine[i] == ' ')
                {
                    // i-th character is a space
                    if ((i + 1) < strlen(singleLine))
                    {
                        if (singleLine[i + 1] == ' ')
                        {
                            // if the the next character is also
                            // a space, get the first non-space character.
                            int idx = getNonSpace(singleLine, i + 2);
                            if (idx == -1)
                            {
                                // rest of the line is all spaces.
                                // put one space and read the next line.
                                fputc(' ', fout);
                                break;
                            }
                            else
                            {
                                fputc(' ', fout);
                                // first non space character is at idx
                                i = idx - 1;
                            }
                        }
                        else
                        {
                            // it was an isolated space
                            fputc(' ', fout);
                        }
                    }
                    else
                    {
                        fputc(' ', fout);
                    }
                }
                else
                {
                    // non-space characters printed as usual.
                    fputc(singleLine[i], fout);
                }
            }
        }

        fclose(f);
        fclose(fout);
    }
}

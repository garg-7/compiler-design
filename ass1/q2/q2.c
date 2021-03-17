#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"

#define MAX_LINE_LENGTH 1024 /* Max number of characters per line.*/
#define YES 1
#define NO 0

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

        FILE *fout = fopen("temp1.txt", "w");
        int *isComment = (int *)malloc(sizeof(int));
        *isComment = NO;
        char *singleLine = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        char *check;
        while (1)
        {

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
                processComments(singleLine, 0, fout, isComment);
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
                    processComments(singleLine, idx, fout, isComment);
                }
            }
        }

        // comments have been removed.
        // now read the intermediate file and remove the extra spaces.
        fclose(f);
        fclose(fout);
        f = fopen("temp1.txt", "r");
        fout = fopen("temp2.txt", "w");

        int *isWithinQuotes = (int *)malloc(sizeof(int));
        *isWithinQuotes = NO;

        while (1)
        {

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
            if (*isWithinQuotes == NO)
            {
                processRedunSpaces(singleLine, 0, fout, isWithinQuotes);
            }
            else
            {
                int idx = getQuotesEnd(singleLine, 0);
                if (idx == -1)
                {
                    *isWithinQuotes = YES; // next line starts within quotes
                    for (int i = 0; i < strlen(singleLine); i++)
                        fputc(singleLine[i], fout);
                }
                else
                {
                    // some idx was returned, process from there.
                    *isWithinQuotes = NO; // next line doesn't start within quotes
                    for (int i = 0; i <= idx; i++)
                        fputc(singleLine[i], fout);
                    processRedunSpaces(singleLine, idx + 1, fout, isWithinQuotes);
                }
            }
        }

        fclose(f);
        fclose(fout);

        // extra consecutive spaces have been removed.
        // now remove trailing spaces and empty lines.

        f = fopen("temp2.txt", "r");
        fout = fopen("output.txt", "w");

        *isWithinQuotes = NO;
        while (1)
        {
            check = fgets(singleLine, MAX_LINE_LENGTH, f);
            // note: fgets reads in the newline and adds a NULL at the end
            if (check == NULL)
            {
                // EOF reached.
                break;
            }

            if (*isWithinQuotes == NO)
            {
                cleanupLine(singleLine, fout, isWithinQuotes);
            }
            else
            {
                int idx = getQuotesEnd(singleLine, 0);
                if (idx == -1)
                {
                    *isWithinQuotes = YES; // next line starts within quotes
                    for (int i = 0; i < strlen(singleLine); i++)
                        fputc(singleLine[i], fout);
                }
                else
                {
                    // some idx was returned, process from there.
                    *isWithinQuotes = NO; // next line doesn't start within quotes
                    for (int i = 0; i <= idx; i++)
                        fputc(singleLine[i], fout);

                    char *newBuffer = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
                    int lenBuf = 0;

                    for (int i = idx + 1; i < strlen(singleLine); i++)
                        newBuffer[lenBuf++] = singleLine[i];

                    cleanupLeftoverLine(newBuffer, fout, isWithinQuotes);
                    free(newBuffer);
                }
            }
        }

        fclose(f);
        fclose(fout);
        free(check);
        free(singleLine);
    }
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YES 1
#define NO 0

/* Returns if and where the comment ends*/
int getCommentEnd(char *buffer, int start)
{

    for (int i = start; i < strlen(buffer); i++)
    {
        if (buffer[i] == '*')
        {
            if ((i + 1) < strlen(buffer))
            {
                if (buffer[i + 1] == '/')
                    // comment end found, return the next index.
                    return i + 2;
            }
        }
    }

    // no comment end (i.e. */) found, so return -1.
    return -1;
}

/* Processes passed line (buffer) from specified index (start).
Writes to FILE* fout. */
void processComments(char *buffer, int start, FILE *fout, int *isComment)
{
    for (int i = start; i < strlen(buffer); i++)
    {
        if (buffer[i] == '/')
        {
            if ((i + 1) < strlen(buffer))
            {
                // next character is before the NULL
                if (buffer[i + 1] == '/')
                {
                    // ignore the rest of the line
                    fputs("\n", fout);
                    break;
                }
                else if (buffer[i + 1] == '*')
                {
                    // possible closure of comment on the same line.
                    // call getCommentEnd()
                    int idx = getCommentEnd(buffer, i + 2);
                    if (idx == -1)
                    {
                        // the rest of the current line is a comment.
                        fputc('\n', fout);
                        *isComment = YES; // look for */ in the next line.
                        break;
                    }
                    else
                    {
                        // comment was closed, resume processing from i = index.
                        // -1 because it will go to i++ after reaching end of loop body.
                        *isComment = NO; // next line not to be treated as a comment.
                        i = idx - 1;
                    }
                }
                else
                {
                    // false alarm, not a comment, simply print the character.
                    fputc(buffer[i], fout);
                }
            }
            else
            {
                // this will be reached only if the last line of the code
                // has a / at the end becasue other lines will always be
                // terminated by \n.
                fputc('/', fout);
            }
        }
        else if (buffer[i] != ' ')
        {
            fputc(buffer[i], fout);
        }
        else
        {
            fputc(buffer[i], fout);
            // space condition. start looping from next position onwards
            // till a non-space is detected.
        }
    }
    return;
}

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

/* Remove redundant spaces from the passed line and 
write a clean line to FILE* fout. */
void processRedunSpaces(char *buffer, FILE *fout)
{
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == ' ')
        {
            // i-th character is a space
            if ((i + 1) < strlen(buffer))
            {
                if (buffer[i + 1] == ' ')
                {
                    // if the the next character is also
                    // a space, get the first non-space character.
                    int idx = getNonSpace(buffer, i + 2);
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
            fputc(buffer[i], fout);
        }
    }
    return;
}

/* Fills start and end indices of non space characters for the passed buffer */
void getLineIndices(char *buffer, int *indices, int *appendNewLine)
{
    indices[0] = 0;
    indices[1] = strlen(buffer) - 1;
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == ' ' || buffer[i] == '\t')
            ;
        else
        {
            indices[0] = i;
            break;
        }
    }
    if (buffer[strlen(buffer) - 1] == '\n')
    {
        for (int i = strlen(buffer) - 1; i >= 0; i--)
        {
            if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n')
                ;
            else
            {
                indices[1] = i;
                *appendNewLine = YES;
                break;
            }
        }
    }
    else
    {
        for (int i = strlen(buffer) - 1; i >= 0; i--)
        {
            if (buffer[i] == ' ' || buffer[i] == '\t')
                ;
            else
            {
                indices[1] = i;
                break;
            }
        }
    }

    return;
}

/* Ignores empty lines and cleans up leading and trailing spaces.*/
void cleanupLine(char *buffer, FILE *fout)
{
    printf("[%ld] ", strlen(buffer));
    int *indices = (int *)malloc(2 * sizeof(int));
    int *appendNewLine = (int *)malloc(sizeof(int));
    *appendNewLine = NO;

    getLineIndices(buffer, indices, appendNewLine);
   
    if (indices[1] == indices[0])
    {
        // if the line contains only a newline character.
        // don't output anything for that line.
        if (buffer[indices[0]] == '\n')
        {
            free(appendNewLine);
            free(indices);
            return;
        }
    }
    for (int i = indices[0]; i <= indices[1]; i++)
    {
        fputc(buffer[i], fout);
    }
    if (*appendNewLine == YES)
        fputc('\n', fout);
    free(appendNewLine);
    free(indices);
    return;
}
#include <stdio.h>
#include <string.h>
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
int processLine(char *buffer, int start, FILE *fout, int *isComment)
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
}

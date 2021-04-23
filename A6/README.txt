---------------------------------------------------------------
ASSIGNMENT - 6
CS335 - Compiler Design
B18CSE011
---------------------------------------------------------------

FILES:

- lalr.c -> the solution file.
- t1.txt -> file containing the grammar in the required format
- valid_string.png -> result for a string that can be parsed by 
                        the given grammar
- invalid_string.png -> result for a string that CANNOT be parsed
                        by the given grammar

---------------------------------------------------------------

EXECUTION:

gcc lalr.c
./a.out

This will ask the user for the start state. (Here, E)
Then it will ask for the number of productions. (Here, 6)
Finally it will ask the user to input the productions
one by one.
For that:
1. Open t1.txt
2. Ctrl+a then ctrl+c, to copy all the content
3. Ctrl+Shift+v to paste on the command line. 

The parser's action and goto table will be displayed.
Then the user will be asked for an input string to parse.
Enter the string, it displays the results accordingly.
Note that the string should be entered in the same format
as the grammar's productions i.e. every token of the input
should be space-separated.

NOTE: The ouput is minimal initially, however, to get the 
complete output from all the functions, simply uncomment the
corresponding
#define DEBUG_XXX
statement in the code.

---------------------------------------------------------------
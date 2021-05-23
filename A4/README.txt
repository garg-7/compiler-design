----------------------------------------------------------
ASSIGNMENT-4
CS335 Compiler Design
----------------------------------------------------------

FILES:

1. sol.c : File containing the solution
2. i1.txt : test input file 1
3. i2.txt : test input file 2
4. i3.txt : test input file 3
5. i4.txt : test input file 4

----------------------------------------------------------

EXECUTION:

gcc sol.c
./a.out

This would first ask the user to enter the start state.
(In this enter the first state like S or prog in the 
given examples).
Then enter the number of productions.
Note that '|' cannot be used to enter multiple productions.
Need to count every individual production.
Then the user is supposed to enter all the productions in the
following format:

HEAD RHS1 RHS2 RHS3 and so on.
So the head of the production is the first token.
Then space separated, all the tokens on the RHS of the
production.

Note that there should not be any extra spaces anywhere.
For reference, 3 input files have been added.

For testing, using an input file:
Enter its start state and number of productions first.
Then simply Ctrl+A & Ctrl+C, the content of the input file
and Ctrl+Shift+V the content onto the terminal when asked
for the productions.

----------------------------------------------------------

VERBOSE OUTPUT:

By default, only the final results are displayed i.e.
the FIRST, FOLLOW sets and the parsing table.

To display information about every step of the process, 
un-comment line 9 (#define DEBUG) from sol.c and repeat the
compilation and execution in the way mentioned above.
This would list all the steps of the process.

------------------------END-------------------------------

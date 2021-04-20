---------------------------------------------------------------------------------
Assignment - 5 
CS335
B18CSE011
---------------------------------------------------------------------------------

Files:
- B18CSE011.c -> solution file
- t1.txt -> test file 1
- t2.txt -> test file 2
- Results/a-1.png -> Left recursion removed from Grammar 1.
- Results/a-2.png -> Left recursion removed from Grammar 2.
- Results/b-1.png -> input string (id+id)*id parsed using LR removed Grammar 1.
                        (read output in bottom up manner)
- Results/b-2.png -> input string )id+id*id not parsed using LR removed Grammar 1.

---------------------------------------------------------------------------------

Exectution:

gcc B18CSE011.c
./a.out

[Code has been written and tested on Ubuntu 20.04]

Note that the file runs with minimal output to the user. To get the full
detailed output of the execution, simply un-comment line 10 such that
DEBUG macro is defined. Compile and run and now detailed output will be shown.
---------------------------------------------------------------------------------

Guidelines:

1. The program first asks for the start state. 

2. Then it asks for the number of productions.

3. Then it asks to input the productions one by one. 
   Note that the productions need to be entered with one space between 
   every pair of tokens. 
   E.g. the production S->aAf, is entered as:
   S a A f
   No extra spaces should be present anywhere to prevent unexpected behaviour.

   To aid testing the rules in the question are entered in the test files 
   t1.txt and t2.txt. Simply do Ctrl+a (select all) & Ctrl+c (copy) an
   intput file and then Ctrl+Shift+v (paste) into the terminal when the program
   asks for the productions. All will be entered in one go. 

4. Finally the program asks for an input string. Simply press enter if
   no input string needs to be parsed. Otherwise enter the input string to parse.
   Again, the format is with added space i.e. the string (id+id)*id needs to
   be input as: ( id + id ) * id

---------------------------------------------------------------------------------
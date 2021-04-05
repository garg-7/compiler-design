--------------------------------------------------------------------------
Assignment - 3
CS335
B18CSE011
--------------------------------------------------------------------------

COMPILATION REQUIREMENTS: 

* Besides -ll linking, parts 'm.lex' and 'o.lex' require
linking the math library using -lm.

* All the programs have been tested on Ubuntu 20.04 and work fine.

--------------------------------------------------------------------------

GENERAL NOTE:

In the parts where some number or string or email etc. is 
to be checked, put it in a file and pass that file using
inpu redirection.
--------------------------------------------------------------------------

CODE SPECIFIC ASSUMPTIONS AND PECULIARITIES:

b - Capitalizes the first character after end of every line
    indicated by ., ?, !

c - The text is delimited only using whitespaces ' ', '\t', 
    '\n'. Everything within this is considered a token.

e - Every character other than a-z and A-Z is used as a delimiter
    to obtain number of words.

f - Same word-delimiter condition as in (e)

g - Pass whatever string is in question into a file. And then 
    pass this file using input redirection.

h - Pass the word to be searched as the first argument
    after the executable. Then use input redirection to pass
    the file to search for it in. Same word-delimiter condition
    as in (e). Note: word lookup is case-sensitive.

i - Same word-delimiter condition as in (e)

j - Same instructions as (h).

k - ASSUMPTIONS:- negative numbers do not have a space
                between the minus sign and the number.
                - "positive and negative numbers" includes
                whole as well as fractional numbers
                - no space between the dot and numbers on 
                either side in case of decimal numbers

l - Same assumptions as in (k).

m - Enter the number to be checked into a file and pass
    the file using input redirection. 
    LINK MATH LIBRARY USING -lm WHILE COMPILING.

n - Enter the number to be checked into a file and pass
    the file using input redirection. 

o - Enter the number to be checked into a file and pass
    the file using input redirection. Prime checking
    limitation based on the size of long long int.
    LINK MATH LIBRARY USING -lm WHILE COMPILING.

p - ASSUMPTIONS: - A valid number is that which 
                has 10 digits with no other character
                in between.
    Enter the number to be checked into a file and pass
    the file using input redirection.

q -  ASSUMPTIONS: - The local part can contain only a-z, A-Z, 0-9, /, _, .
                - The local part cannot begin/end with a dot.
                - There cannot be consecutive dots anywhere.
                - The domain part cannot begin/end with a dot.
                - The domain part can contain only a-z, A-Z, 0-9, -, .
                - No whitespaces are allowed anywhere in the address.
    Enter the email address to be checked into a file
    and pass the file using input redirection.

r - only an IPv4 address is assumed to make up the
    expected input.

----------------------------END-------------------------------------------
CIS*2520: Data Structures - Assignment #3

NOTE: assumed submission should be in a ".tar" file like last time for Assignment 1

To compile, just type "make" to compile everything.

f.dat file format:
NOTE: assumed that "f.dat" is a text file containing proper data of a 2D array of integers for a 20x10 size of numbers < 100
NOTE: if integers are larger than 99, then 2D array table printing will be skewed

Q1: q1.c    - Usage: ./q1 "<input: String expression_fully_paranthesized_with_no_spaces>"
NOTE: followed assignment description for rules about the expression such as operators, variables, and number formats
--> if this is not followed, it will stop and display an error msg (see program comments for more detailed information on error checking)
NOTE: assumption that numbers inserted as part of the expression must be positive
NOTE: assumption that each variable name in the expression is unique
/**
 * - Takes input as a fully parenthesized, arithmetic expression
 *   of binary operators +, −, ∗, /, and converts the expression into a binary expression tree.
 * - Your program should take input from the command line, so the expression is surrounded by "".
 * - The entire expression should be in a haracter string without any space in it.
 * - An input string only includes floating numbers in the format of Y.YY, that is, one digit to
 *   the left of the decimal point and two digits to the right of the decimal point, and variables of
 *   the form of x1, x2, ....
 * - Your program shall allow for the leaves in the expression tree not only to store floating
 *   values but also to store variables of the form x1, x2, x3, ..., which are initially 0.0 and can be
 *   updated interactively by the user.
 * - menu options:
 * --> 1: display - prints a visual representation of the binary arithmetic tree
 * --> 2: preorder - displays the prefix notation of the expression
 * --> 3: inorder - displays the fully parenthesized expression in proper order
 * --> 4: postorder - displays the postfix notation of the expression
 * --> 5: update - prompts user for the variable name, then it prompts the user for that variable's new value and changes it within the expression (binary arithmetic tree structure)
 * --> 6: calculate - calculates and displays the result of the expression (checks for divide by zero errors)
 * --> 7: exit - will only end program if '7' is entered
 *
 * NOTE: assumption that numbers inserted as part of the expression must be positive
 * NOTE: assumption that each variable name in the expression is unique
 *
 * NOTE: https://youtu.be/9bzZjNiPUWo used in conjunction with lecture slides to create traversal print functions
 */

E.g.
Input:  ./q1 "((((1.00+(8.00-5.12))/((x1+3.70)-(6.00*x2)))*x3)+2.00)"
NOTE: displays appropriate error msg upon bad input for the expression, and then terminates
    --> not fully parenthesized
    --> not the proportionate number of operands vs operators
    --> variable is max 10 characters or less, starts with 'x' followed by digits
    --> number of form 'Y.YY'
    --> only operators +, -, *, and /
- has a menu loop that prompts the user for what they want to do and their 7 options
--> 1: displays a visual representation of the binary arithmetic tree (horizontally)
--> 2: prints the expression in prefix notation
--> 3: prints the expression in proper ordered notation with parentheses
--> 4: prints the expression in postfix notation
--> 5: updates a varaible's stored value (initially 0); prompts user for variable name, then prompts user for new value if a match for that variable name was found
    --> displays error msg if input is bad for either of these prompts
--> 6: calculates/evaluates the result of the binary arithmetic expression
    --> if a divide by zero occurs anywhere, the program gives a result of "Answer = undefined --> cannot dive by zero"
--> 7: ends program

Q2: q2.c    - Usage: ./q2
/**
 * - Reads 200 2-digit integers from text file "f.dat" and stores the integers in an array of 20 rows and 10 columns.
 * - The program treats each row of the array as an object,
 *   with the sum of the first three integers being the key,
 *   and the other seven integers being the information content.
 * - The program then creates a heap with a node containing an object.
 * - You are required to use an array representation of heap, and apply the parental node downheap algorithm in the array representation.
 * - The program finally displays the heap as a 20 × 10 array, a row for an object.
 *
 * NOTE: assumption that the file being used is a text file named "f.dat" is 20 rows with 10 numbers in each row separated by a space
 * NOTE: assume that the file has good data
 * NOTE: I left some commented print blocks that can be used to check some some things if you would like to
 */

E.g.
Input:  the example file given to us
Output:

25 12 17 82 52 53 05 03 68 64
16 14 27 99 29 99 79 45 59 19
50 09 06 44 59 87 32 36 36 46
12 00 68 68 49 68 74 32 09 30
81 30 16 65 40 39 44 49 32 60
25 41 11 88 15 39 88 69 31 49
44 27 68 96 70 98 85 13 25 47
03 60 76 23 14 81 82 00 38 04
43 27 20 59 04 84 10 42 43 02
41 82 53 40 29 84 24 61 87 20
99 37 50 93 57 06 29 91 92 39
81 14 42 21 09 69 05 30 20 91
03 37 97 53 36 54 69 68 38 43
83 82 43 11 21 04 08 97 93 14
51 90 37 86 42 44 48 64 41 01
96 89 75 00 63 24 54 20 20 24
88 56 59 88 39 30 44 59 49 72
49 17 44 32 33 54 60 33 51 87
09 83 22 90 74 56 64 75 67 55
41 62 82 46 69 42 74 68 79 28

NOTE: assumption that the file being used is a text file named "f.dat" is 20 rows with 10 numbers in each row separated by a space
NOTE: assume that the file has good data and is appropriate for this question
NOTE: assumed that "f.dat" is a text file containing proper data of a 2D array of integers for a 20x10 size of numbers < 100
NOTE: if integers are larger than 99, then 2D array table printing will be skewed

NOTE: any URLs in code are referencing learning resources used to assist with things we have not done yet and/or not really talked about in detail

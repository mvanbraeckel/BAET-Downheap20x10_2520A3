/*
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @version 03/11/2018
 * CIS2520 Assignment 3
 * q1.c
    - Takes input as a fully parenthesized, arithmetic expression
        of binary operators +, −, ∗, /, and converts the expression into a binary expression tree.
    - Your program should take input from the command line, so the expression is surrounded by "".
    - The entire expression should be in a haracter string without any space in it.
    - An input string only includes floating numbers in the format of Y.YY, that is, one digit to
        the left of the decimal point and two digits to the right of the decimal point, and variables of
        the form of x1, x2, ....
    - Your program shall allow for the leaves in the expression tree not only to store floating
        values but also to store variables of the form x1, x2, x3, ..., which are initially 0.0 and can be
        updated interactively by the user.
    - menu options:
    --> 1: display - prints a visual representation of the binary arithmetic tree
    --> 2: preorder - displays the prefix notation of the expression
    --> 3: inorder - displays the fully parenthesized expression in proper order
    --> 4: postorder - displays the postfix notation of the expression
    --> 5: update - prompts user for the variable name, then it prompts the user for that variable's new value and changes it within the expression (binary arithmetic tree structure)
    --> 6: calculate - calculates and displays the result of the expression (checks for divide by zero errors)
    --> 7: exit - will only end program if '7' is entered

    NOTE: assumption that numbers inserted as part of the expression must be positive
    NOTE: assumption that each variable name in the expression is unique

    NOTE: https://youtu.be/9bzZjNiPUWo used in conjunction with lecture slides to create traversal print functions
 */

// =========================== INCLUDED LIBRARIES ===========================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ================================= DEFINES ================================
#define TREE_SPACING 6 // define spacing of each column when printing the tree

// =========================== STRUCT DECLARATIONS ==========================
typedef struct binary_tree_struct {
    struct binary_tree_struct *left;
    struct binary_tree_struct *right;
    char* id;
    double value;
} Node;

// =========================== FUNCTION PROTOTYPES ==========================
void create_tree(Node** root, char* expression);

void preorder(Node** root);
void inorder(Node** root);
void postorder(Node** root);

void free_tree(Node **root);
void find_variable(Node** root, char* var_name, int *var_found);
double evaluate_tree(Node** root, int *error_occurred);

void flush_input(char *input);
int isDouble(char *input);
int isWhitespace(char *input);
int max(int num1, int num2);

void display_tree2(Node** root, int depth, char* path, int right);
void display_tree(Node** root);
int get_tree_height(Node** root);

int main(int argc, char **argv) {
    // checks that the correct #of command line arguments have been entered
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <input: String expression_fully_paranthesized_with_no_spaces>\n", argv[0]);
        exit(-1);
    }

    // check expression is encapsulated by parentheses
    if(argv[1][0] != '(' || argv[1][strlen(argv[1])-1] != ')') {
        // display error msg because expression was not fully paranthesized properly
        fprintf(stderr, "Invalid expression <%s>: entire expression must be contained by parentheses: '(' at the start, and ')' at the end\n", argv[1]);
        exit(-1);
    }

    // declare variables
    char* string = argv[1];
    Node* root = NULL;

    char curr_char;
    char curr_string[11]; //max 10 characters (max should only be 4)
    int num_operands = 0;
    int num_operators = 0;
    int left_brackets = 0;
    int right_brackets = 0;

    // loops through inputted expression, retrieves each significant part of the expression and constructs the binary arithmetic tree
    for(int i = 0; i < strlen(string); i++) {
        curr_char = string[i];
        // gets single curr char
        strcpy(curr_string, &curr_char);
        curr_string[1] = '\0';

        // checks for valid expression
        if(curr_char == '(') {          // counts number of '('
            left_brackets++;

        } else if(curr_char == ')') {   // counts number of ')'
            right_brackets++;

        } else if(curr_char >= '0' && curr_char <= '9') { // it's a double
            num_operands++;
            // gets number of format Y.YY
            strncpy(curr_string, string +i, 4);
            curr_string[4] = '\0';

            // checks validity
            if(strlen(curr_string) != 4 || !isdigit(curr_string[0]) || curr_string[1] != '.' || !isdigit(curr_string[2]) || !isdigit(curr_string[3])) {
                // display error msg because number is invalid
                fprintf(stderr, "Invalid number input <%s>: numbers must be of format '0.00'\n", curr_string);
                exit(-1);
            }
            i += 3; //compensate

        } else if( curr_char == 'x' ) { // it's a variable
            num_operands++;
            int j = 1;

            // get variable of form x0#
            while(isdigit(string[i+j])) {
                if(j == 10) {
                    // display error msg because variable is invalid
                    fprintf(stderr, "Invalid variable input <%s>: variable name of format 'x0#' is too long\n", curr_string);
                    exit(-1);
                }
                strncat(curr_string, string +i +j, 1);
                j++;
            }
            curr_string[j] = '\0';

            // checks validity
            if(strlen(curr_string) < 2) {
                // display error msg because variable is invalid
                fprintf(stderr, "Invalid variable input <%s>: variables must be of format 'x0#'\n", curr_string);
                exit(-1);
            }
            i += (j-1); //compensate

        } else if( curr_char == '+' || curr_char == '-' || curr_char == '*' || curr_char == '/' ) {
            num_operators++;            // it's an operator

        } else {
            // display error msg because character is invalid
            fprintf(stderr, "Invalid input character <%c>: expression must only contain numbers of format '0.00', " \
                            "variables of form 'x0#', and operators: +, -, *, or / and parentheses: ( or )\n", curr_char);
            exit(-1);
        }
    } // end for loop

    // continue final checks for valid expression
    if(left_brackets != right_brackets) {
        // display error msg because expression was not fully paranthesized properly
        fprintf(stderr, "Invalid expression <%s>: expression must contain an equal number of left and right parentheses\n", string);
        exit(-1);
    }
    if(num_operands - num_operators != 1) {
        // display error msg because expression was not fully paranthesized properly
        fprintf(stderr, "Invalid expression <%s>: expression must contain one more operator than operand\n", string);
        exit(-1);
    }

    /*// if it reaches here, it's a valid expression
        printf("\n\tIt's a good expression!\n\n"); */

    // *******************************************************************************************************************************************************************************************

    // build the tree now
    create_tree(&root, string);

    // if this happens, messed up growing the tree -- shouldn't print
    if(root == NULL) {
        fprintf(stderr, "\t...still NULL here -I am root\n");
    }

    // *******************************************************************************************************************************************************************************************

    // declare variables
    char menu_input[3];
    char variable_name[12]; //input -max 10 characters long
    int var_found = 0; //false
    int good_input = 0;//false
    int error_occurred = 0;//false
    double result = 0.0;

    // infinite menu loop, only ending program with '7' as input
    while(1) {
        printf("What would you like to do?\n(Please enter the number of the option you would like to do)\n\n" \
                "\t1. Display\n" \
                "\t2. Preorder\n" \
                "\t3. Inorder\n" \
                "\t4. Postorder\n" \
                "\t5. Update\n" \
                "\t6. Calculate\n" \
                "\t7. Exit.\n\n" \
                "Select option: ");
        fgets(menu_input, 3, stdin);    //only need the first character of input for menu option
        flush_input(menu_input);

        // checks user input
        if(strlen(menu_input) > 1) {
            // display error msg inputting more than just the number option
            printf("\nError: invald input - You must choose one of the menu options by number: [1,7]\nPlease try again\n\n");

        }
        else if(menu_input[0] == '1') {         // ================================= 1 =================================
            display_tree(&root);
            printf("\n");

        }
        else if(menu_input[0] == '2') {         // ================================= 2 =================================
            printf("\nPreorder:");
            preorder(&root); // prints tree in prefix notation
            printf("\n\n");

        }
        else if(menu_input[0] == '3') {         // ================================= 3 =================================
            printf("\nInorder: ");
            inorder(&root); // prints tree in proper, ordered notation (as it was inputted)
            printf("\n\n");

            //printf("Inorder\t%s\n\n", string);

        }
        else if(menu_input[0] == '4') {         // ================================= 4 =================================
            printf("\nPostorder:");
            postorder(&root); // prints tree in postfix notation
            printf("\n\n");

        }
        else if(menu_input[0] == '5') {         // ================================= 5 =================================
            // prompt user for variable name
            good_input = 0;
            while(!good_input) {
                printf("\nEnter the name of variable to be updated: ");
                fgets(variable_name, 12, stdin);
                flush_input(variable_name);

                // checks input
                if(variable_name[0] != 'x') {
                    printf("\nError: Invalid variable name - must be of form 'x0#'\n\n");   //display err msg -doesn't start with 'x'
                } else if(strlen(variable_name) > 10) {
                    printf("\nError: Invalid variable name - must be 10 characters or less\n\n");   //display err msg -too long
                } else if(isWhitespace(variable_name)) {
                    printf("\nError: Invalid variable name - please try again\n\n");   //display err msg -only whitespace chars
                } else {
                    good_input = 1;
                }
            }
            // get it correct size
            strncpy(variable_name, variable_name, 10);
            variable_name[strlen(variable_name)] = '\0';

            // find matching variable name, then prompt user for new value, and set it
            var_found = 0; //reset
            find_variable(&root, variable_name, &var_found);
            // display err msg if variable not found
            if(!var_found) {
                printf("\nSorry, that variable name is not in the expression - please try again\n\n");
            }

        }
        else if(menu_input[0] == '6') {         // ================================= 6 =================================
            error_occurred = 0;
            result = evaluate_tree(&root, &error_occurred);

            // check for divide-by-zero error
            if(error_occurred) {
                printf("\nAnswer = undefined --> cannot divide by zero\n\n");

            } else {
                printf("\nAnswer = %.2lf\n\n", result);

            }

        }
        else if(menu_input[0] == '7') {         // ================================= 7 =================================
            printf("\nGood bye!\n");    // display closing msg
            free_tree(&root);           // free before ending
            return 0;
        }
        else {                                  // =============================== ELSE ================================
            // display error msg for bad input
            printf("\nError: invald input - You must choose one of the menu options by number: [1,7]\nPlease try again\n\n");
        }
    } // end menu loop
} // end main

// =============================== FUNCTIONS ================================

/**
 * Recursively reates the binary arithmetic tree starting at the root, working its way to each leaf
 * @param root -the tree node being created
 * @param expression -the expression being used to create the tree
 */
void create_tree(Node** root, char* expression) {
    // declare variables
    int left_brackets = 0;
    int right_brackets = 0;
    char *left_half, *right_half, curr_char;
    char *ptr;

    // check base case (substring of just a variable or number was passed as the expression)
    if(expression[0] != '(') {
        // create the node
        *root = malloc(sizeof(Node));
        // id is a variable or number
        (*root)->id = malloc(strlen(expression)+1);
        (*root)->id = expression;

        // check if variable or number, set value accordingly
        if(expression[0] == 'x') { //variable
            (*root)->value = 0.00;     //init
        } else { //number
            (*root)->value = strtod(expression, &ptr);
        }

        // pass the half subtrings recursively
        (*root)->left = NULL;
        (*root)->right = NULL;
        return;
    }

    // loop through expression to find root operator, then recursively do so for each left and right substring
    for(int i = 0; i < strlen(expression); i++) {
        curr_char = expression[i];

        // it's the root operator (leaves of the tree) if there's 1 more left bracket than right brackets

        if(curr_char == '(') {
            left_brackets++;

        } else if(curr_char == ')') {
            right_brackets++;

        } else if( left_brackets - right_brackets == 1 && (curr_char == '+' || curr_char == '-' || curr_char == '*' || curr_char == '/') ) {
            // construct left substring excluding starting '(' and operator
            left_half = malloc(i);
            strncpy(left_half, expression +1, i-1);
            left_half[strlen(left_half)] = '\0';

            // construct right substring excluding operator and ending ')'
            right_half = malloc(strlen(expression) -i-1);
            strncpy(right_half, expression +i+1, strlen(expression) -i-2);
            right_half[strlen(right_half)] = '\0';

            // create the node, and init attributes
            *root = malloc(sizeof(Node));
            // id is an operator
            (*root)->id = malloc(2);
            (*root)->id[0] = curr_char;
            (*root)->id[1] = '\0';
            (*root)->value = 0.0;

            // pass each half subtring to get nodes constructed recursively
            (*root)->left = NULL;
            create_tree( &((*root)->left), left_half );
            (*root)->right = NULL;
            create_tree( &((*root)->right), right_half );
        }
    }
}

/**
 * Recursively prints the binary arithmetic tree in prefix notation
 * @param root -the tree node being printed during preorder traversal
 */
void preorder(Node** root) {
    if(*root == NULL) {      //recursive base case
        return;
    }
    // print before traversing left and right subtrees (pre)
    printf(" %s", (*root)->id);
    preorder( &((*root)->left) );
    preorder( &((*root)->right) );
}

/**
 * Recursively prints the binary arithmetic tree in ordered notation with parentheses
 * @param root -the tree node being printed during inorder traversal
 */
void inorder(Node** root) {
    if(*root == NULL) {      //recursive base case
        return;
    }
    // print in order (after left subtree, but before right subtree)
    if( (*root)->left != NULL ) { // prints an open bracket as long it's not a leaf
        printf("(");
    }
    inorder( &((*root)->left) );
    printf("%s", (*root)->id);
    inorder( &((*root)->right) );
    if( (*root)->right != NULL ) { // prints a closed bracket as long it's not a leaf
        printf(")");
    }
}

/**
 * Recursively prints the binary arithmetic tree in postorder notation
 * @param root -the tree node being printed during postorder traversal
 */
void postorder(Node** root) {
    if(*root == NULL) {      //recursive base case
        return;
    }
    // print after traversing both left and right subtrees (post)
    postorder( &((*root)->left) );
    postorder( &((*root)->right) );
    printf(" %s", (*root)->id);
}

/**
 * Frees the entire tree (all nodes in the tree) by postorder traverse
 * @param root -the tree node to be freed
 */
void free_tree(Node **root) {
    if(*root == NULL) {      //recursive base case
        return;
    }
    // print after traversing both left and right subtrees (post)
    free_tree( &((*root)->left) );
    free_tree( &((*root)->right) );
    // actually free the node and it's id
    free( (*root)->id );
    free( *root );
}

/**
 * Searches for a given variable name within the tree and prompts the user to change it
 * @param root -the tree node being searched
 * @param var_name -the variable being searched for
 * @param var_found -set to 1 if a match is found, otherwise remains 0
 */
void find_variable(Node** root, char* var_name, int *var_found) {
    if(*root == NULL) {      //reached a leaf's left or right NULL child
        return;
    }
    // check if it's a match (if it's a leaf, both children = NULL)
    if( (*root)->left == NULL && (*root)->right == NULL && strcmp(var_name, (*root)->id) == 0 ) {
        char new_value[12]; //input -max 10 digits
        char* ptr; //placeholder

        *var_found = 1; //found match, set true

        // prompt user for new value
        int good_input = 0;
        while(!good_input) {
            printf("Enter the new value for '%s': ", var_name);
            fgets(new_value, 12, stdin);
            flush_input(new_value);

            // checks input
            if(strlen(new_value) > 10) {
                printf("\nError: Invalid value - max of 10 digits\n\n");   //display err msg -string too long
            } else {
                strncpy(new_value, new_value, 10);  //gets it to correct size of string
                new_value[strlen(new_value)] = '\0';

                if(isWhitespace(new_value)) {
                    printf("\nError: Invalid value - please try again\n\n");   //display err msg -only whitespace chars
                } else if(isDouble(new_value)) {
                    good_input = 1;
                } else {
                    printf("\nError: Invalid value - must enter a valid decimal number\n\n");   //display err msg -contained non-digits
                }
            }
        }
        // store value in the node
        (*root)->value = strtod(new_value, &ptr);

        return; // helps to speed up the process a little bit
    }
    // continue checking the left and right subtrees
    find_variable( &((*root)->left), var_name, var_found );
    find_variable( &((*root)->right), var_name, var_found );
}

/**
 * Evaluates the result of the arithmetic binary expression contained by the tree
 * @param root -the tree node being evaluated
 * @param error_occurred -set to 1 if divide-by-zero error occurs, otherwise remains 0
 * @return -the value of the node on a base case, the value of two nodes with the operation otherwise
 */
double evaluate_tree(Node** root, int *error_occurred) {
    // check if the tree exists
    if( (*root) == NULL ) {
        return 0;
    }
    // if it's a leaf, get the value
    if( (*root)->left == NULL && (*root)->right == NULL ) {
        return (*root)->value;

    } else if( *error_occurred ) {
        return 0; // quickly go back up recursive call chain, result doesn't matter anymore

    } else {
        double a = evaluate_tree( &((*root)->left), error_occurred );
        double b = evaluate_tree( &((*root)->right), error_occurred );
        // check operator and return left <operator> right, check for divide by zero error
        if( (*root)->id[0] == '+' ) {
            return a + b;

        } else if( (*root)->id[0] == '-' ) {
            return a - b;

        } else if( (*root)->id[0] == '*' ) {
            return a * b;

        } else { //(*root)->id == '/' )
            // checks for divide by zero error
            if(b == 0.0) {
                *error_occurred = 1;
                return 0; // quickly go back up recursive call chain, result doesn't matter anymore

            } else {
                return a / b;
            }
        }
    }
}

// ============================== MY FUNCTIONS ==============================

/**
 * Flushes all leftover data in the stream
 * @param char *input -the string that was just read from stdin
 */
void flush_input(char* input) {
    /* if the '\n' is NOT found in the word itself, flush the stream */
    if(strchr(input, '\n') == NULL) {
        while ((getchar()) != '\n');
        input[strlen(input)] = '\0';
    } else {
        input[strlen(input)-1] = '\0';
    }
}

/**
 * Checks if the string is a double
 * @param char *input -the string to be checked
 * @return 1 if the string is a double
 */
int isDouble(char* input) {
    // checks that it's only digits and '.'
    for(int i = 0; i < strlen(input); i++) {
        if(!isdigit(input[i]) && input[i] != '.') {
            return 0; //contained a non-digit character (also not a period)
        }
    }

    // accounts for case of only digits, but multiple '.'
    char* ptr;
    double input_as_double = strtod(input, &ptr);
    input_as_double += 0.0; // doesn't matter

    if(ptr == NULL || strcmp(ptr, "") == 0) {
        return 1; //good conversion to double
    } else {
        return 0; //something leftover from original input, so not good
    }
}

/**
 * Checks if the string is pure whitespace
 * @param char *input -the string to be checked
 * @return 1 if the string only contains whitespace characters
 */
int isWhitespace(char *input) {
    for(int i = 0; i < strlen(input); i++) {
        if(!isspace(input[i])) {
            return 0; //contained a non-whitespace character
        }
    }
    return 1;
}

/**
 * Determines the larger of two integers
 * @param num1 -the first number being compared
 * @param num2 -the second number being compared
 * @return -the greater of the two numbers
 */
int max(int num1, int num2) {
    int result;
    // check which is greater
    if(num1 > num2) {
        result = num1;
    } else {
        result = num2;
    }
    return result;
}

// ============================ HELPER FUNCTIONS ============================

/**
 * Secondary function to display the binary arithmetic tree horizontally (it the helper function to primary to help print)
 * @param root -the biggest head root of the tree structures
 * @param depth -the depth of the current tree node
 * @param path -the string that helps print the visual line connections of the tree by acting as a placeholder for pathfinding
 * @param right -set to 1 if it's a right subtree, otherwise 0 for a left subtree
 * Used https://stackoverflow.com/questions/41091382/how-to-print-in-console-a-tree-horizontally-with-links-using-c for inspiration
 * NOTE: a '1' means to print the tree path, otherwise don't (default '0')
 */
void display_tree2(Node** root, int depth, char* path, int right) {
    // base case to stop
    if( *root == NULL ) {
        return;
    }
    // increase spacing
    depth++;
    // start with right child
    display_tree2( &((*root)->right), depth, path, 1 );

    // set | to draw map
    path[depth-2] = '0';

    if(right) {
        path[depth-2] = '1';
    }
    if( (*root)->left ) {
        path[depth-1] = '1';
    }

    // print root after spacing
    printf("\n");
    // print verical lines
    for(int i = 0; i < depth-1; i++) {
        if(i == depth-2) {
            printf(" ");
        } else if(path[i] == '1') {
            printf("┆");
        } else {
            printf(" ");
        }
        // print horizontal lines
        for(int j = 1; j < TREE_SPACING; j++){
            if(i < depth-2) {
                printf(" ");
            } else {
                printf("╌");
            }
        }
    }

    // print the root tree node's identifier and end the line
    printf("%s\n", (*root)->id);
    // vertical spacers below, and print more vertical lines
    for(int i = 0; i < depth; i++) {
        if(path[i] == '1') {
            printf("┆");
        } else {
            printf(" ");
        }
        for(int j = 1; j < TREE_SPACING; j++) {
            printf(" ");
        }
    }

    // go to left node
    display_tree2( &((*root)->left), depth, path, 0 );
}

/**
 * Primary function to display the binary arithmetic tree horizontally (uses a helper function to print)
 * @param root -the biggest head root of the tree structures
 * Used https://stackoverflow.com/questions/41091382/how-to-print-in-console-a-tree-horizontally-with-links-using-c for inspiration
 */
void display_tree(Node** root) {
    int tree_height = get_tree_height( &(*root) );
    // create string for pathfinding to place vertical and horizontal lines to draw the tree (init everything to '0') based on the height of the tree
    char* path = malloc( TREE_SPACING * (tree_height+1) );
    path[ TREE_SPACING * (tree_height+1) - 1] = '\0';
    for(int i = 0; i < TREE_SPACING * (tree_height+1) - 1; i++) {
        path[i] = '0';
    }
    // initial depth is 0
    display_tree2( &(*root), 0, path, 0 );
}

/**
 * Retrieves the height of the binary arithmetic tree
 * @param root -the tree being looked at
 * @return -the height of the tree
 * http://www.techiedelight.com/calculate-height-binary-tree-iterative-recursive/ used here
 */
int get_tree_height(Node** root) {
    // if the tree is empty, it's a height of 0 (base case)
    if( *root == NULL ) {
        return 0;
    }
    // recurse for left and right subtree, while considering which is deepest
    return max( get_tree_height(&((*root)->left)), get_tree_height(&((*root)->right)) ) + 1;
}

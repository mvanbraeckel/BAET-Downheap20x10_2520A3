/*
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @version 03/11/2018
 * CIS2520 Assignment 3
 * q2.c
    - Reads 200 2-digit integers from text file "f.dat" and stores the integers in an array of 20 rows and 10 columns.
    - The program treats each row of the array as an object,
        with the sum of the first three integers being the key,
        and the other seven integers being the information content.
    - The program then creates a heap with a node containing an object.
    - You are required to use an array representation of heap, and apply the parental node downheap algorithm in the array representation.
    - The program finally displays the heap as a 20 × 10 array, a row for an object.

    NOTE: assumption that the file being used is a text file named "f.dat" is 20 rows with 10 numbers in each row separated by a space
    NOTE: assume that the file has good data
    NOTE: I left some commented print blocks that can be used to check some some things if you would like to
 */

// =========================== INCLUDED LIBRARIES ===========================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =========================== STRUCT DECLARATIONS ==========================
typedef struct heap_struct {
    int key;
    int array[10];
} Node;

// =========================== FUNCTION PROTOTYPES ==========================
int** read_file(char *filename);
int** create_array2D(int m, int n);
void free_array2D(void** arr);
void downheap(Node* heap_array, int i);

int main(int argc, char **argv) {
    // declare variables
    int** array2D = read_file("f.dat");

    /*// display the original read 2D array
    printf("\n\tBefore:\n");
    for(int i = 0; i < 20; i++) {
        printf("\n");
        for(int j = 0; j < 10; j++) {
            printf("%02d ", array2D[i][j]);
        }
    }
    printf("\n\n");*/

    // step 1 -> create heap
    Node heap_array[21]; // 0th index will be unused
    Node temp;

    // and then load the heap as it appears (calculate key, copy array2D row into the node, add it to the heap)
    for(int i = 1; i < 21; i++) {
        temp.key = array2D[i-1][0] + array2D[i-1][1] + array2D[i-1][2];
        for(int j = 0; j < 10; j++) {
            temp.array[j] = array2D[i-1][j];
        }
        heap_array[i] = temp;
    }

    /*// display original key order
    printf("\n\tAfter heap creation - try keys:\n");
    for(int i = 1; i < 21; i++) {
        printf("Key %02d = %d\n", i, heap_array[i].key);
    }*/

    // step 2 -> apply downheap to every internal (parental) node starting with the last, ending with the root,
    // so loop backwards through the heap array only looking at the parental nodes
    for(int i = 21/2; i > 0; i--) {
        downheap(heap_array, i);
    }

    /*// display downheaped key order
    printf("\n\tAfter downheap - try keys:\n");
    for(int i = 1; i < 21; i++) {
        printf("Key %02d = %d\n", i, heap_array[i].key);
    }*/

    // display the heap as a 2D array after downheaping
    for(int i = 1; i < 21; i++) {
        printf("\n");
        for(int j = 0; j < 10; j++) {
            printf("%02d ", heap_array[i].array[j]);
        }
    }
    printf("\n\n");

    free_array2D((void**)array2D);
    return 0;
}

// ================================ FUNCTIONS ===============================

/**
 * Reads the data for a 20x10 2D array of integers, storing it in a 2D array
 * @param filename -the name of the text file being read
 * @return -the read 2D array of integers
 * NOTE: format for file has each integer on the same line separated by a space
 */
int** read_file(char *filename) {
    // create and init 2D array
    //int array2D[20][10];
    int** array2D = create_array2D(20,10);

    FILE *fp = fopen(filename, "r");
    // checks if fopen messed up
    if(fp == NULL) {
        fprintf(stderr, "ERROR: File could not be opened\n");

    } else {
        char buffer[100];
        // read until the end of the file -- if something is read, will return non-zero
        for(int i = 0; i < 20; i++) {
            if(fgets(buffer, 100, fp)) {
                buffer[strlen(buffer)-1] = '\0';
                // store each integer in the 2D array
                array2D[i][0] = atoi(strtok(buffer, " ")); //reads first integer
                for(int j = 1; j < 10; j++) {
                    array2D[i][j] = atoi(strtok(NULL, " ")); //reads other 9 integers
                }
            }
        }
    }
    fclose(fp);
    return array2D;
}

/**
 * Creates a 2D array of integers given the number of rows and columns
 * @param m -the number of rows
 * @param n -the number of columns
 * @return -the created 2D array of integers
 * https://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-to-a-function-in-c used here
 */
int** create_array2D(int m, int n) {
    // create memory for the array
    int* values = calloc(m * n, sizeof(int)); // initialize all values as 0
    int** rows = malloc(m * sizeof(int*));
    for(int i = 0; i < m; i++) {
        rows[i] = values + i*n;
    }
    return rows;
}

/**
 * Frees a 2D array
 * @param arr -the 2D array to be freed
 */
void free_array2D(void** arr) {
    free(*arr);
    free(arr);
}

/**
 * Applies parental node downheap algorithm on the heap_array representation
 * @param heap_array -the heap array structure being downheaped
 * @param i -the index of the current cell in the heap array
 * --> internal (parental) nodes are in the first floor(size/2) array cells
 * --> the children of parent node array cell 'i' (s.t. 1 <= i <= floor(size/2)) are at array cells '2i' and '2i+1'
 * --> the parent of the node at array cell 'i' (s.t. 2 <= i <= n) is in array cell 'floor(i/2)'
 * NOTE: integer division automatically does floor, so using a floor method is not necessary
 */
void downheap(Node* heap_array, int i) {
    Node temp;
    int leftSmaller = 1; // true -left child is smaller by default

    // check parental node
    if(1 <= i && i <= 21/2) {

        // if internal, must have at least one child (which would be a left child)
        // check right child exists
        if(2*i +1 < 21) {
            // check which is smaller
            if(heap_array[2*i].key > heap_array[2*i +1].key) {
                leftSmaller = 0;
            }
        }

        // check left child (must be in range)
        if(2*i < 21 && heap_array[i].key > heap_array[2*i].key && leftSmaller) {
            // swap nodes
            temp = heap_array[i];
            heap_array[i] = heap_array[2*i];
            heap_array[2*i] = temp;
            // continue downheaping until its children are >= to it (or reaches a leaf)
            downheap(heap_array, 2*i);
        }
        // check right child (must be in range)
        if(2*i +1 < 21 && heap_array[i].key > heap_array[2*i +1].key && !leftSmaller) {
            // swap nodes
            temp = heap_array[i];
            heap_array[i] = heap_array[2*i +1];
            heap_array[2*i +1] = temp;
            // continue downheaping until its children are >= to it (or reaches a leaf)
            downheap(heap_array, 2*i +1);
        }
    }
}

/* This is a code written by Hannah Ohunene Ajoge (ASU ID: 1217581417) for CSE 543: Information Assurance and Security Fuzz Them All Project*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

// A function to swap integers 
void swap (int *a, int *b) 
{ 
    int temp = *a; 
    *a = *b; 
    *b = temp; 
} 
  
// A function to generate a random permutation of an array
void randomize ( int arr[], int n, int x) { 
    // Seed given so different but reproduceable value is gotten each time
    srand(x);
    for (int i = n-1; i > 0; i--) 
    { 
        // Pick a random index from 0 to i 
        int j = rand() % (i+1); 
  
        // Swap arr[i] with the element at random index 
        swap(&arr[i], &arr[j]); 
    }
} 

// A function to read content of file to string
char *readFile(char *filename) {
    FILE *f = fopen(filename, "rt");
    assert(f);
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = (char *) malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, f);
    fclose(f);
    return buffer;
}

// A function to generate random numbers between 0 and 255, given an integer seed
int genrandomchar(int x) {
    srand(x);
    int y = rand() % 256;
    return y;
}

int main(int argc, char *argv[]) {
    // Pass in integer through command line
    char *a = argv[1];
    char *b = argv[2];
    int fuzzer_seed = atoi(a);
    int fuzzer_iteration = atoi(b);

    // Ensure arguments are greater than or less than 1
    if (fuzzer_seed < 1 || fuzzer_iteration < 1){
        printf("Your arguments must be greater than or equal to 1");
        exit(1);
    } 

    // Read content of seed file to string
    char *content = readFile("./seed");

    // Get length of seed
    content[strcspn(content, "\n")] = '\0'; // Ensure length of content is not increased by 1 because of presence of newline 
    int seedlength, num_of_muts;
 
    // Set your seed to `prng_seed`
    srand(fuzzer_seed);

    // Generate seed for array shuffling
    int seed_randomiz = 500;

    // While loop to execute fuzzing
    int iteration_counter = 0;
    while (iteration_counter < fuzzer_iteration){
        // In this block generate random non-repeatative but reproducible index of content, that corresponds to 13% of content's length
        iteration_counter = iteration_counter + 1;
        seedlength = strlen(content);
        if (seedlength == 0){ // Before continuing, take care of short seed file content - i.e. even seed file with as low as 0 byte can be used to fuzz. 
        // This will is to help take care of an unexpected empty seed file, which could be one of the 10 test cases.  
            content[0] = 'h';
            seedlength = strlen(content);
        }
        // Get number of bytes to mutate
        num_of_muts = round(0.13 * (float)seedlength);
        // Next take care of situation whereby 3 or less bytes are in seed and 13% can't result in having a mutant character. 
        if (num_of_muts == 0){
            num_of_muts = 1;
        }
        // Generate an array of index. The size of the array should be the length of content
        int array[seedlength];
        for (int i = 0; i < seedlength; i++) { 
        array[i] = i;
        }
        // Shuffle the content of the array of index
        randomize (array, seedlength, seed_randomiz);
        seed_randomiz = seed_randomiz - 1;
        // Generate a new array that consist of the first 13% of the shuffled array - this will be used to identify bytes/characters to mutate
        int newarray[num_of_muts];
        for (int i = 0; i < num_of_muts; i++) { 
        newarray[i] = array[i];
        }


        /*In this block using final array generated in the above block 
        (i.e array of index that correspond to 13% of indexes content) to mutate content - remember content is the name of the content of the seed file*/
        int seed_here1 = seedlength * iteration_counter; // Get seed to pass to genrandomchar function - here the seedlength that will be decreased by 1 at each iteration to get repeatable result from genrandomchar function is initiated
        int mut_int;
        int string_index1;
        for (int z = 0; z < num_of_muts; z++){
            mut_int = genrandomchar(seed_here1); // Get integer to convert to character for mutation
            seed_here1 = seed_here1 - 1; // Decrease seed value to have random but repeatable result from genrandomchar function
            string_index1 = newarray[z]; // For identifing which content's character (index) to mutate
            // Execute mutation of content
            char mut_char = mut_int;
            content[string_index1] = mut_char;
        }

        //In this block, check if this iteration is a multiple of 500, if it is add 10 bytes of random but reproducible characters to content
        if  (iteration_counter % 500 == 0){
            int newseedlength = seedlength + 10; // Get seed to pass to genrandomchar function
            char append_content[10];
            // Add the 10 bytes of random but reproducible characters to content
            for (int m = 0; m < 10; m++){
                char char_toappend = genrandomchar(newseedlength);
                append_content[m] = char_toappend;
                newseedlength = newseedlength - 1;
            }
            strcat(content, append_content);
            seed_randomiz = 500;
        }
        //In this block print value of content
        printf("%s\n", content);   
    }
    // Exit the application gracefully
    exit(0);
}
# Fuzzer
CSE 543: Information Assurance and Security Fuzz Them All Project
Name: Hannah Ohunene Ajoge 
ASU ID: 1217581417

My fuzzer is and executable from a c source code. I wrote the c source code and built the executeable using the command:
gcc fuzzer.c -o fuzzer -lm
I needed the -lm flag because I included <math.h>
All the libraries I included are <stdio.h>, <stdlib.h>,  <assert.h>,  <string.h> and <math.h>
I executed all my coding, compiling and fuzzing of the test programs using Visual Studio Code. All my work was done in Ubuntu 20.04.

My Algotithm:
Before I started coding I decided that my code will accomplish the following.
1. Input: 
a. My code should be able to take an input see file, read the content and assign the content to a variable named content. But the code must not modify the seed file.
b. My code should accept two arguments which are both int32. These integers should not be less than or equal to 0.
2. Iterations:
a. At each iteration, my code should calculate the length of the content variable, take a rounded up 13% of the length and randomly but reproducibly mutate 13% of the bytes.
b. Still within each iteration check if that iteration is a multiple of 500 (of course not zero as the while loop ensures that at the begining). If it is, it should add 10 bytes which are randomly but reproducibly generated.
3. My code whould ensure at the end of each iteration the content is printed to stdout.
4. Finally my code should ensure that it graceously exit with exit(0).

Strategy:
I have documented my execution process within my source code by richly using comments and also using sensible and easily predictable function and variable names.
Below is a succinct and comprehensive overview.
1. Input: 
a. I wrote a function named readFile to assertively read a file of a specified filename to a variable, close the file and return the variable. 
Within the main function, I call this function pass "./seed" and assign the returned string to a variable named content
b. Within the main function, I accept two argument, pass the first argument to a variable named fuzzer_seed as int32 and the second argument to a second variable - fuzzer_iteration.
I then added and if statement which checks if any of the arguments have a value less than or equal to 0. It then prints out "Your arguments must be greater than or equal to 1" and exits with indication of failure.
2. Iterations:
a. First I wrote a function named swap to swap integers and then another function (randomize) which calls swap to generate a random permutation of an array.
randomize takes in three arguments, an array and two integers. The first integer is the length of the array and the second integer is the value to be passed to srand() so that a reproducible but random shuffling of the array can occur.
I also wrote another function named genrandomchar which takes an integer as argument. It then assign that integer to srand() so that a reproducible but random integer within 0 and 255 can be generated.
Then within main function I do the following to calculate the length of content, take a rounded up 13% of the length and randomly but reproducibly mutate 13% of content.
- I initiate int variables named seedlength and num_of_muts. seedlength will be holding the length of content at each itearation as needed. num_of_muts will hold the number of bytes to be mutated.
- I then set my seed (srand()) to the first argument (`prng_seed`) to my application (fuzzer)
- Aftwerards, I intiate another variable seed_randomiz which I set to 500. seed_randomiz will be used to be used passed to the function randomize as the third argument and the input for srand().
- Then I initiated a while loop that will ensure each iteration is carried out as following:
--> Just before entering the while loop I initiate a counter variable which is named iteration_counter and set to 0. This will be used to indicate the interation the program is in; and called as needed.
--> The first part of the while loop generate random non-repeatative but reproducible index of content, that corresponds to 13% of content's length as following:
--> --> I increase the iteration counter iteration_counter  by 1
--> --> I get the legnth of content and assign it to seed variable. 
Since I do not know what the "ten or bonus" programs the grader will test my application against, I introduced an if statement to be able to use an empty seed file.
The statement assign a byte to content and the assign the new length to seedlength.
--> --> Then I get the number of bytes to mutate by calculating 13% of seed rounded to whole number. To take care of situation, where content has 3 or less charcters in which 13% will result in 0, I introduced an if statement.
The if statement assign 1 to num_of_muts. The idea is that in such case, in each iteration, one byte will be executed until the 10byte addition at iteration 500.
--> -->  Next I generate an array of index. The size of the array is the length of content.
--> -->  Then I shuffle the array of index to get a random but reproducible redistribution of the index. The shuffling is done by randomize, which takes the array, the length of the array and seed_randomiz as input.
The idea is that by passing seed_randomiz which is decreased at each iteration as seed to randomize, the shuffling will be random but reproducible as the seed is known.
--> -->  Finally in this part of the while loop generate a sub array known as newarray that consist of the first 13% of the shuffled array.
The idea is to use the indices which are randomly but reproducibly generated to identify bytes to mutate
--> In the second part of the while loop, I use newarray to mutate content within a for loop statement. 
--> -->  First I get the seed to to pass to genrandomchar function. This seed (integer value) is stored in seed_here1 and it is initialized to length of content multiplied by the iteration count at the moment. 
The seed_here1 is decreased at each iteration of the for loop.
--> -->  Next initiate two variables, mut_int and string_index1. The randomly generated integer from genrandomchar is assigned to mut_int; and string_index1 holds index to be mutated at the iteration of the for loop.
--> -->  Finally, I execute a for loop statement that strategically mutate a randomly sellected byte.
b. The final part of the while loop execute an important task, which is to to check if that iteration is a multiple of 500 and add 10 bytes.
This is executed using a for loop within an if loop statement. Also at the end of the if statement seed_randomiz is reinitiated to 500. 
The idea is that with a different length of array that will result from the addition of 10 bytes, repeating seed for randomize will still generate a different set of shuffled array.
3. At the end of each each iteration (while loop) I print content to stdout.
4. Finally at the end of all the iterations I exit the application graceously.

Fuzzing:
Within the folders prog0. prog1 and prog2 respectively I executed the following commands from the terminal:
counter=0; while read -r line; counter=$((counter+1)); do echo $line | ./prog_0; if [[ $? -eq 139 ]]; then echo -e "Iteration: $counter" '\n' $line   > crashinput; break; else echo -e "Iteration: $counter" '\n'  $line  >> noncrashinput; fi  done < <(./fuzzer 1 100000)
counter=0; while read -r line; counter=$((counter+1)); do echo $line | ./prog_1; if [[ $? -eq 139 ]]; then echo -e "Iteration: $counter" '\n' $line   > crashinput; break; else echo -e "Iteration: $counter" '\n'  $line  >> noncrashinput; fi  done < <(./fuzzer 1 100000)
counter=0; while read -r line; counter=$((counter+1)); do echo $line | ./prog_2; if [[ $? -eq 139 ]]; then echo -e "Iteration: $counter" '\n' $line   > crashinput; break; else echo -e "Iteration: $counter" '\n'  $line  >> noncrashinput; fi  done < <(./fuzzer 1 100000)
These read each print out and trys to crash the programs p_0, p_1 and p_2 respectively. A counter is also initiated to know which iteration that each input to the program is generated.
The commands will send iteration count and the bytes to a file called crashinput and stop execution once they get the error code 139 - Segmentation fault. 
Else if no error is encountered, the iteration count and bytes are sent to the file known as noncrashinput.
The idea is to crash the program and know which iteration the crash took place.

Result:
The three programs p_0, p_1 and p_2 were crashed at iterations 755, 1412 and 3842 respectively.

Helios Stress Tester is a utility designed to stress test two Programming Olympiad programs, usually one of which works correctly but slowly, and the second quickly but incorrectly. Helios will help generate random tests of the type that the user specifies. For example, the number n and the array of numbers are long n, for each of them you can set the generation range as you like.

There are 5 functions in the current version. Function parameters are separated by ","

count,number of tests - Sets the number of tests to generate

tested ,"program" - Specifies the program to be tested

testing ,"program" - Specifies the testing program

save_tests,"path" - Saves the input and output data for each test along the back path, if you write "1" instead of the path, the Tests folder will be created in the current directory and all the necessary files will be written there.

v,name,int,min,max,eol - creates a random number from minimum to maximum inclusive, if eol = 1, puts a line break.
v,name,vector _int,length,minimum,maximum,eol - creates an array of random numbers from minimum to maximum inclusive, if eol = 1, puts a line break, the length can be set by a number, or can use the number generated earlier, for this write variable name.

v,name,permutation,length,eol - creates a random permutation of numbers from 1 to length, if eol = 1, puts a line break, the length can be set by a number, or can use a number generated earlier, for this write the name of the variable.

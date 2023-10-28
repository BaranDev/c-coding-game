#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//SIMILARITY CHECKER

int min(int a, int b, int c) {
    int min = a;
    if (b < min) {
        min = b;
    }
    if (c < min) {
        min = c;
    }
    return min;
}

int calculateLevenshteinDistance(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    int matrix[len1 + 1][len2 + 1];
    int i,j;
    for (i = 0; i <= len1; i++) {
        for (j = 0; j <= len2; j++) {
            if (i == 0) {
                matrix[i][j] = j;
            } else if (j == 0) {
                matrix[i][j] = i;
            } else if (s1[i - 1] == s2[j - 1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
            } else {
                matrix[i][j] = 1 + min(matrix[i - 1][j], matrix[i][j - 1], matrix[i - 1][j - 1]);
            }
        }
    }

    return matrix[len1][len2];
}

double calculateSimilarityRate(const char *s1, const char *s2) {
    int distance = calculateLevenshteinDistance(s1, s2);
    int maxLen = strlen(s1) > strlen(s2) ? strlen(s1) : strlen(s2);
    double similarity = 100.0 - ((double)distance / maxLen) * 100.0;
    return similarity < 0.0 ? 0.0 : similarity;
}

int calculateSimilarityRateBetweenStrings(const char *str1, const char *str2) {
    return (int)calculateSimilarityRate(str1, str2);
}

// SIMILARITY CHECKER ENDED

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char *giveLoopQuestion(int *questionNum);
char *giveArrayQuestion(int *questionNum);


void displayMenu() {
    printf("Choose a subject:\n");
    printf("1. Loops\n");
    printf("2. Arrays\n");
    printf("3. Quit\n");
}

int isFileEmpty(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Return -1 to indicate an error
    }

    // Move the file pointer to the end of the file
    fseek(file, 0, SEEK_END);

    // Get the current position of the file pointer
    long size = ftell(file);

    fclose(file); // Close the file

    if (size == 0) {
        return 1; // File is empty
    } else {
        return 0; // File is not empty
    }
}



void checkFile_firstInit() {
    const char *filename = "answer.c";

    if (isFileEmpty(filename)) {
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            perror("Error opening file for writing");
            return;
        }

        const char *code = "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\nint main() {\n//START\n    \n";
        fprintf(file, "%s", code);
        fclose(file);
        //printf("File 'answer.c' was empty and has been initialized with the code.\n");
    }
}

void changeRound() {
    const char *filename = "answer.c";
    const char *filename_lastAnswer = "last_answer.c";

    // Create 'last_answer.c'
    FILE *file_lastAnswer = fopen(filename_lastAnswer, "w");
    if (file_lastAnswer == NULL) {
        perror("Error opening 'last_answer.c' for writing");
        return;
    }

    fclose(file_lastAnswer);

    if (!isFileEmpty(filename)) {
        // Copy the content of 'answer.c' to 'last_answer.c'
        FILE *file = fopen(filename, "r");
        file_lastAnswer = fopen(filename_lastAnswer, "w");

        if (file == NULL || file_lastAnswer == NULL) {
            perror("Error opening files");
            return;
        }

        char c;
        while ((c = fgetc(file)) != EOF) {
            fputc(c, file_lastAnswer);
        }

        fclose(file);
        fclose(file_lastAnswer);

        // Clear 'answer.c' and write the starting code
        file = fopen(filename, "w");
        if (file == NULL) {
            perror("Error opening 'answer.c' for writing");
            return;
        }

        const char *code = "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\nint main() {\n//START\n    \n";
        fprintf(file, "%s", code);
        fclose(file);
        //printf("File 'answer.c' has been cleared and initialized with the code.\n");
    } else {
        // 'answer.c' is empty, so initialize it with the starting code
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            perror("Error opening 'answer.c' for writing");
            return;
        }

        const char *code = "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\nint main() {\n//START\n    \n";
        fprintf(file, "%s", code);
        fclose(file);
        //printf("File 'answer.c' was empty and has been initialized with the code.\n");
    }
}

void writeUserCode() {
    const char *filename = "answer.c";
    int lineCounter = 0; // Counter to track the line number
    int startWriting = 0; // Flag to indicate when to start writing the code

    if (isFileEmpty(filename)) {
        printf("File 'answer.c' is empty. Please provide your C code after the 6th line.\n");
        return;
    }

    FILE *file = fopen(filename, "a"); // Open the file for appending

    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    char userCode[500];
    printf("\n\t------------------------------------------------------------------------");
    printf("\n\t\tEnter your C code (type 'EOF' on a new line to finish)\n");
    printf("\n\t\t    Instead of breaking lines(\\n) leave a space\n");
     printf("\n\t\t     And consider the main function returns 0\n");
    printf("\n\t\tDon't forget you can't delete the lines! Good luck!\n");
    printf("\t------------------------------------------------------------------------\n\n");
    printf("#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\nint main() {\n    \n");
    

    char line[500];
    while (1) {
        lineCounter=6;
        startWriting = 1; // Start writing after the 6th line
        fgets(line, sizeof(line), stdin);
        
        if (strcmp(line, "EOF\n") == 0) {
            system("cls");
            fprintf(file, "%s","return 0;\n}\n");
            break;
        } else if (startWriting && line != NULL) {
            fprintf(file, "%s", line);
        }
    }

    fclose(file);
}




void compileAndCaptureOutput() {
    const char *sourceFile = "answer.c";
    const char *outputFile = "output.txt";
    char compileCommand[200];
    char runCommand[100];

    // Compile the source code using GCC and redirect errors to the output file
    sprintf(compileCommand, "gcc %s -o output 2>%s", sourceFile, outputFile);
    int compileStatus = system(compileCommand);

    if (compileStatus == 0) {
        // Run the compiled program and capture the output in the output file
        sprintf(runCommand, "output > %s", outputFile);
        int runStatus = system(runCommand);

        if (runStatus == 0) {
            printf("Code has been compiled, and output captured in 'output.txt'.\n");
        } else {
            printf("Error running the program.\n");
        }
    } else {
        printf("Error compiling the code. Check 'output.txt' for compilation errors.\n");
    }
}

void roundStart()
{
     
   // Get the player's code
    writeUserCode();
   // Compile and get output
   compileAndCaptureOutput();
     
}



//LOOPS

int questionNum;
void printLoopQuestion() {
    system("cls");
    printf(ANSI_COLOR_RED "\t\t\tYou chose 'Loops'. Here's a C code challenge:\n" ANSI_COLOR_RESET);
    
    
    char *randomQuestion = giveLoopQuestion(&questionNum); //NUMBER OF THE QUESTION, 0 INDEXED

    if (randomQuestion != NULL) {
        printf(ANSI_COLOR_GREEN "\tQuestion: %s\n" ANSI_COLOR_RESET, randomQuestion);
    } else {
        printf("Error: Failed to retrieve a random question.\n");
    }
}


char* giveLoopQuestion(int *questionNum) {
    char *questions[] = {
        "Print numbers from 1 to 10 using a for loop",
        "Print the even numbers from 2 to 20 using a for loop",
        "Print the odd numbers from 1 to 19 using a for loop",
        "Print the first 10 natural numbers using a while loop",
        "Print the squares of numbers from 1 to 10 using a for loop",
        "Print the first 10 Fibonacci numbers using a for loop",
        "Print the numbers in reverse order from 10 to 1 using a for loop",
        "Print the factorial of 12 using a while loop",
        "Print the multiplication table of 5 using a for loop (e.g. 2 4 6 8 10 12 14 16 18 20)",
        "Print the numbers in descending order from 10 to 1 using a while loop",
        "Print the sum of numbers from 1 to 100 using a for loop",
        "Print the powers of 2 from 1 to 10 using a while loop",
        "Print the reverse of the string 'I love programming in C!' using while loop",
        "Print 2 arrow that look at each other that each has 50 '-' in it using for loop (e.g. -----><-----)",
        "Print the ASCII values of lowercase letters from 'a' to 'z' using a for loop",
        "Print the ASCII values of uppercase letters from 'A' to 'Z' using a for loop",
        "Print numbers from 97 to 500 using a while loop",
        "Print 'Hello, World!' 10 times using a for loop (e.g. 'Hello, World! Hello, World! Hello, World!...')",
        "Print the characters of 'Niels Henrik David Bohr' with a space in between them using a for loop (e.g. N i e l s...)",
        "Calculate and print the sum of digits of 8849522839452457783 using a while loop (e.g. 232: 7)"
    };
    
    srand(time(NULL));
    
    int numQuestions = sizeof(questions) / sizeof(questions[0]);
    int randomIndex = rand() % numQuestions;
    
    if (questionNum != NULL) {
        *questionNum = randomIndex;
    }

    return questions[randomIndex];
}

void checkLoopAnswer(int questionNum) {
    char *desiredOutputs[] = {
        "1 2 3 4 5 6 7 8 9 10",
        "2 4 6 8 10 12 14 16 18 20",
        "1 3 5 7 9 11 13 15 17 19",
        "1 2 3 4 5 6 7 8 9 10",
        "1 4 9 16 25 36 49 64 81 100",
        "0 1 1 2 3 5 8 13 21 34",
        "10 9 8 7 6 5 4 3 2 1",
        "479001600",
        "5 10 15 20 25 30 35 40 45 50",
        "10 9 8 7 6 5 4 3 2 1",
        "5050",
        "1 2 4 8 16 32 64 128 256 512",
        "!C ni gnimmargorp evol I",
        "--------------------------------------------------><--------------------------------------------------",
        "97 98 99 100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 266 267 268 269 270 271 272 273 274 275 276 277 278 279 280 281 282 283 284 285 286 287 288 289 290 291 292 293 294 295 296 297 298 299 300 301 302 303 304 305 306 307 308 309 310 311 312 313 314 315 316 317 318 319 320 321 322 323 324 325 326 327 328 329 330 331 332 333 334 335 336 337 338 339 340 341 342 343 344 345 346 347 348 349 350 351 352 353 354 355 356 357 358 359 360 361 362 363 364 365 366 367 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394 395 396 397 398 399 400 401 402 403 404 405 406 407 408 409 410 411 412 413 414 415 416 417 418 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433 434 435 436 437 438 439 440 441 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486 487 488 489 490 491 492 493 494 495 496 497 498 499 500",
        "Hello, World! Hello, World! Hello, World! Hello, World! Hello, World! Hello, World! Hello, World! Hello, World! Hello, World! Hello, World!",
        "N i e l s   H e n r i k   D a v i d   B o hr",
        "103"
    };

    if (questionNum < 0 || questionNum >= sizeof(desiredOutputs) / sizeof(desiredOutputs[0])) {
        printf("Invalid question number.\n");
        return;
    }

    FILE *file = fopen("output.txt", "r");
    if (file == NULL) {
        printf("Error opening output file.\n");
        return;
    }

    char outputBuffer[500];
    char *desiredOutput = desiredOutputs[questionNum];
    int linesMatched = 0;
    int totalLines = 0;

    while (fgets(outputBuffer, sizeof(outputBuffer), file) != NULL) {
        int similarityRate = calculateSimilarityRateBetweenStrings(outputBuffer, desiredOutput);
        if (similarityRate >= 90) {
            linesMatched++;
        }
        totalLines++;
    }

    if (totalLines > 0) {
        double successRate = ((double)linesMatched / totalLines) * 100.0;
        printf("\t\t  Your output's success rate: %.2lf%%\n", successRate);
    } else {
        printf("No output found in the file.\n");
    }

    fclose(file);
}
     
void checkLoopCodes(int questionNum) {
    char *desiredCodes[] = {
    "for(int i = 1; i <= 10; i++) printf(%d, i);",
    "for(int i = 2; i <= 20; i += 2) printf(%d, i);",
    "for(int i = 1; i <= 19; i += 2) printf(%d, i);",
    "int i = 1; while(i <= 10) { printf(%d, i); i++; }",
    "for(int i = 1; i <= 10; i++) printf(%d, i * i);",
    "int a = 0, b = 1, c, n = 10; for(int i = 0; i < n; i++) { printf(%d, a); c = a + b; a = b; b = c; }",
    "for(int i = 10; i >= 1; i--) printf(%d, i);",
    "int fact = 1, num = 12; while(num > 0) { fact *= num; num--; } printf(%d, fact);",
    "for(int i = 1; i <= 10; i++) printf(%d, 5 * i);",
    "int j = 10; while(j >= 1) { printf(%d, j); j--; }",
    "int sum = 0; for(int i = 1; i <= 100; i++) sum += i; printf(%d, sum);",
    "int power = 1; for(int i = 1; i <= 10; i++) { printf(%d, power); power *= 2; }",
    "char str[] = I love programming in C!; int len = strlen(str); for(int i = len - 1; i >= 0; i--) printf(%c, str[i]);",
    "for(int i = 0; i < 50; i++) printf(-); printf(>><<); for(int i = 0; i < 50; i++) printf(-);",
    "for(char c = 'a'; c <= 'z'; c++) printf(%d, c);",
    "for(char c = 'A'; c <= 'Z'; c++) printf(%d, c);",
    "for(int i = 97; i <= 500; i++) printf(%d, i);",
    "for(int i = 0; i < 10; i++) printf(Hello, World! );",
    "char name[] = Niels Henrik David Bohr; for(int i = 0; name[i] != '\0'; i++) { if(name[i] == ' ') printf( ); else printf(%c, name[i]); }",
    "long long num = 8849522839452457783; int sum = 0; while(num != 0) { sum += num % 10; num /= 10; } printf(Sum: %d, sum);"
};


    if (questionNum < 0 || questionNum >= sizeof(desiredCodes) / sizeof(desiredCodes[0])) {
        printf("Invalid question number.\n");
        return;
    }

    FILE *file = fopen("last_answer.c", "r");
    if (file == NULL) {
        printf("Error opening last_answer.c for reading.\n");
        return;
    }

    char codeBuffer[10000];
    size_t codeLength = 0;

    while (fgets(codeBuffer + codeLength, sizeof(codeBuffer) - codeLength, file) != NULL) {
        codeLength += strlen(codeBuffer + codeLength);
    }

    fclose(file);

    char *desiredCode = desiredCodes[questionNum];
    int similarityRate = calculateSimilarityRateBetweenStrings(codeBuffer, desiredCode);

    printf("\t\t  Your code's success rate is: %d%%\n", similarityRate);
    if(similarityRate>50)
    printf(ANSI_COLOR_GREEN "\n\t\tYou have passed!!\n" ANSI_COLOR_RESET);
    else if(similarityRate<=50)
    printf(ANSI_COLOR_RED "\t\tYou have failed!! Please try again!\n\n" ANSI_COLOR_RESET);
}   
//ARRAYS

void printArrayQuestion() {
    system("cls");
    printf(ANSI_COLOR_RED "\t\t\tYou chose 'Arrays'. Here's a C code challenge:\n" ANSI_COLOR_RESET);
    
    
    char *randomQuestion = giveArrayQuestion(&questionNum); //NUMBER OF THE QUESTION, 0 INDEXED

    if (randomQuestion != NULL) {
        printf(ANSI_COLOR_GREEN "\tQuestion: %s\n" ANSI_COLOR_RESET, randomQuestion);
    } else {
        printf("Error: Failed to retrieve a random question.\n");
    }
}

char* giveArrayQuestion(int *questionNum) {
    char *questions[] = {
    "Create an array of 5 integers and initialize it with values 1, 2, 3, 4, and 5. Print the array.",
    "Initialize an array of 4 floating-point numbers with values 3.14, 2.71, 1.618, and 0.577. Print the array.",
    "Given an array of integers: 8, 12, 5, 27, 19, 3, 15, 22. Find and print the maximum value in the array.",
    "Given an array of integers: 8, 12, 5, 27, 19, 3, 15, 22. Find and print the minimum value in the array.",
    "Create an array of 6 strings, each representing a month of the year. Print the array.",
    "Given an array of characters: 'H', 'e', 'l', 'l', 'o'. Print the characters as a string.",
    "Initialize an array with 6 integers: 4, 7, 2, 9, 5, 8. Calculate and print their sum.",
    "Given an array of integers: 6, 18, 3, 12, 9, 7. Print the even numbers in the array.",
    "Given an array of integers: 6, 18, 3, 12, 9, 7. Print the odd numbers in the array.",
    "Create an array of 5 names (strings) and print the names in reverse order.",
    "Given an array of integers: 10, 20, 30, 40, 50. Calculate and print the average value of the numbers.",
    "Create an array of 4 doubles: 1.5, 2.0, 0.5, 3.0. Calculate and print the product of the elements.",
    "Given an array of characters: 'A', 'e', 'i', 'o', 'U'. Count and print the number of vowels in the array.",
    "Given an array of integers: 4, 5, 6, 8, 10. Calculate and print the product of all even numbers.",
    "Given an array of integers: 2, 3, 5, 7, 8. Calculate and print the sum of prime numbers.",
    "Create an array of 3 words: 'Apple', 'Banana', 'Cherry'. Print the words in alphabetical order.",
    "Given an array of characters: 'A', 'b', 'C', 'd', 'E'. Count and print the number of uppercase letters.",
    "Given an array of integers: 8, 14, 22, 5, 10. Find and print the index of the first occurrence of number 22.",
    "Given an array of integers: 17, 9, 23, 6, 12. Print the array in descending order.",
    "Given an array of integers: 3, 7, 1, 5, 9. Find and print the sum of numbers at even indices.",
    "Create an array of 3 sentences: 'Hello, World!', 'Coding is fun.', 'Programming is cool.' Print the sentence with the most words."
};
    
    srand(time(NULL));
    
    int numQuestions = sizeof(questions) / sizeof(questions[0]);
    int randomIndex = rand() % numQuestions;
    
    if (questionNum != NULL) {
        *questionNum = randomIndex;
    }

    return questions[randomIndex];
}

void checkArrayAnswer(int questionNum) {
    char *desiredOutputs[] = {
        "1 2 3 4 5",
        "3.14 2.71 1.618 0.577",
        "27",
        "3",
        "January February March April May June",
        "Hello",
        "35",
        "6 18 12",
        "3 9 7",
        "May June April March February January",
        "30",
        "9.0",
        "4",
        "48",
        "17",
        "Apple Banana Cherry",
        "ACE",
        "2",
        "23 17 12 9 6",
        "5",
        "Hello, World! Coding is fun. Programming is cool."
    };

    if (questionNum < 0 || questionNum >= sizeof(desiredOutputs) / sizeof(desiredOutputs[0])) {
        printf("Invalid question number.\n");
        return;
    }

    FILE *file = fopen("output.txt", "r");
    if (file == NULL) {
        printf("Error opening output file.\n");
        return;
    }

    char outputBuffer[500];
    char *desiredOutput = desiredOutputs[questionNum];
    int linesMatched = 0;
    int totalLines = 0;

    while (fgets(outputBuffer, sizeof(outputBuffer), file) != NULL) {
        int similarityRate = calculateSimilarityRateBetweenStrings(outputBuffer, desiredOutput);
        if (similarityRate >= 90) {
            linesMatched++;
        }
        totalLines++;
    }

    if (totalLines > 0) {
        double successRate = ((double)linesMatched / totalLines) * 100.0;
        printf("\t\t  Your output's success rate: %.2lf%%\n", successRate);
    } else {
        printf("No output found in the file.\n");
    }

    fclose(file);
}

void checkArrayCodes(int questionNum) {
   char *desiredCodes[] = {
    "int arr[5] = {1, 2, 3, 4, 5}; for (int i = 0; i < 5; i++) printf(%d , arr[i]);",
    "float arr[4] = {3.14, 2.71, 1.618, 0.577}; for (int i = 0; i < 4; i++) printf(%.2f , arr[i]);",
    "int arr[] = {8, 12, 5, 27, 19, 3, 15, 22}; int max = arr[0]; for (int i = 1; i < 8; i++) if (arr[i] > max) max = arr[i]; printf(%d, max);",
    "int arr[] = {8, 12, 5, 27, 19, 3, 15, 22}; int min = arr[0]; for (int i = 1; i < 8; i++) if (arr[i] < min) min = arr[i]; printf(%d, min);",
    "char *months[] = {January, February, March, April, May, June}; for (int i = 0; i < 6; i++) printf(%s , months[i]);",
    "char chars[] = {'H', 'e', 'l', 'l', 'o'}; printf(%s, chars);",
    "int arr[] = {4, 7, 2, 9, 5, 8}; int sum = 0; for (int i = 0; i < 6; i++) sum += arr[i]; printf(%d, sum);",
    "int arr[] = {6, 18, 3, 12, 9, 7}; for (int i = 0; i < 6; i++) if (arr[i] % 2 == 0) printf(%d , arr[i]);",
    "int arr[] = {6, 18, 3, 12, 9, 7}; for (int i = 0; i < 6; i++) if (arr[i] % 2 != 0) printf(%d , arr[i]);",
    "char *names[] = {Name1, Name2, Name3, Name4, Name5}; for (int i = 4; i >= 0; i--) printf(%s , names[i]);",
    "int arr[] = {10, 20, 30, 40, 50}; int sum = 0; for (int i = 0; i < 5; i++) sum += arr[i]; printf(%.2f , (float)sum / 5);",
    "double arr[] = {1.5, 2.0, 0.5, 3.0}; double product = 1.0; for (int i = 0; i < 4; i++) product *= arr[i]; printf(%.2f , product);",
    "char chars[] = {'A', 'e', 'i', 'o', 'U'}; int count = 0; for (int i = 0; i < 5; i++) if (chars[i] == 'A' || chars[i] == 'E' || chars[i] == 'I' || chars[i] == 'O' || chars[i] == 'U') count++; printf(%d, count);",
    "int arr[] = {4, 5, 6, 8, 10}; int product = 1; for (int i = 0; i < 5; i++) if (arr[i] % 2 == 0) product *= arr[i]; printf(%d, product);",
    "int arr[] = {2, 3, 5, 7, 8}; int sum = 0; for (int i = 0; i < 5; i++) { int prime = 1; for (int j = 2; j < arr[i]; j++) if (arr[i] % j == 0) prime = 0; if (prime == 1) sum += arr[i]; } printf(%d, sum);",
    "char *words[] = {Apple, Banana, Cherry}; for (int i = 0; i < 3; i++) printf(%s , words[i]);",
    "char chars[] = {'A', 'b', 'C', 'd', 'E'}; int count = 0; for (int i = 0; i < 5; i++) if (chars[i] >= 'A' && chars[i] <= 'Z') count++; printf(%d, count);",
    "int arr[] = {8, 14, 22, 5, 10}; int index = -1; for (int i = 0; i < 5; i++) if (arr[i] == 22) { index = i; break; } printf(%d, index);",
    "int arr[] = {17, 9, 23, 6, 12}; for (int i = 4; i >= 0; i--) printf(%d , arr[i]);",
    "int arr[] = {3, 7, 1, 5, 9}; int sum = 0; for (int i = 0; i < 5; i += 2) sum += arr[i]; printf(%d, sum);",
    "char *sentences[] = {Hello, World!, Coding is fun., Programming is cool.}; int maxWords = 0, index = -1; for (int i = 0; i < 3; i++) { int words = 1; for (int j = 0; j < strlen(sentences[i]); j++) if (sentences[i][j] == ' ') words++; if (words > maxWords) { maxWords = words; index = i; } } printf(%s, sentences[index]);"
};


    if (questionNum < 0 || questionNum >= sizeof(desiredCodes) / sizeof(desiredCodes[0])) {
        printf("Invalid question number.\n");
        return;
    }

    FILE *file = fopen("last_answer.c", "r");
    if (file == NULL) {
        printf("Error opening last_answer.c for reading.\n");
        return;
    }

    char codeBuffer[10000];
    size_t codeLength = 0;

    while (fgets(codeBuffer + codeLength, sizeof(codeBuffer) - codeLength, file) != NULL) {
        codeLength += strlen(codeBuffer + codeLength);
    }

    fclose(file);

    char *desiredCode = desiredCodes[questionNum];
    int similarityRate = calculateSimilarityRateBetweenStrings(codeBuffer, desiredCode);

    printf("Your code's success rate is: %d%%\n", similarityRate);
}


int main() {
    FILE *file;
    FILE *file_read;
    
    
    file = fopen("answer.c", "w");
    if (file == NULL) {
        printf("File could not be opened.\n");
        return 1;
    }
    
    checkFile_firstInit(); //Check for first initialization
    
    int choice;
    char code[500];

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (choice == 1) {
            printLoopQuestion();
            roundStart();
            checkLoopAnswer(questionNum);
            checkLoopCodes(questionNum);
            changeRound();
            
        } else if (choice == 2) {
            
            printArrayQuestion();
            roundStart();
            checkArrayAnswer(questionNum);
            checkArrayCodes(questionNum);
            changeRound();
            
            
        } else if (choice == 3) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid choice. Please select a valid option.\n");
        }
    }

    return 0;
}

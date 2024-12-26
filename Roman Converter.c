#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to convert a Roman numeral string to its decimal equivalent
int romanToDecimal(char roman[]) {
    int total = 0, i = 0;
    int values[256] = {0}; // ASCII table mapping
    values['I'] = 1; values['V'] = 5; values['X'] = 10;
    values['L'] = 50; values['C'] = 100; values['D'] = 500; values['M'] = 1000;

    while (roman[i] != '\0') {
        if (values[roman[i]] == 0) { // Check for invalid characters
            return -1; // Return -1 for invalid characters
        }
        if (values[roman[i]] < values[roman[i + 1]]) {
            total -= values[roman[i]];
        } else {
            total += values[roman[i]];
        }
        i++;
    }
    return total;
}

// Function to convert a number to its word equivalent
void writeInWords(int num, FILE* outputFile) {
    char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million", "Billion"};

    if (num < 0) {
        fprintf(outputFile, "Negative ");
        num = -num;
    }

    if (num == 0) {
        fprintf(outputFile, "Zero");
        return;
    }

    char result[1000] = "";
    int chunkCount = 0;

    while (num > 0) {
        int chunk = num % 1000; // Process last three digits
        if (chunk > 0) {
            char chunkWords[256] = "";

            // Handle hundreds
            if (chunk / 100 > 0) {
                sprintf(chunkWords, "%s Hundred ", ones[chunk / 100]);
            }

            chunk %= 100; // Remaining two digits

            // Handle tens and ones
            if (chunk >= 10 && chunk < 20) {
                strcat(chunkWords, teens[chunk - 10]);
            } else {
                if (chunk / 10 > 0) {
                    strcat(chunkWords, tens[chunk / 10]);
                    if (chunk % 10 > 0) strcat(chunkWords, " ");
                }
                if (chunk % 10 > 0) {
                    strcat(chunkWords, ones[chunk % 10]);
                }
            }

            // Add thousands/millions/etc. suffix
            if (chunkCount > 0) {
                char temp[256];
                sprintf(temp, " %s ", thousands[chunkCount]);
                strcat(chunkWords, temp);
            }

            // Prepend to result
            char temp[1000];
            sprintf(temp, "%s %s", chunkWords, result);
            strcpy(result, temp);
        }

        num /= 1000; // Move to the next three digits
        chunkCount++;
    }

    fprintf(outputFile, "%s\n", result);
}

// Function to perform arithmetic operations
int performOperation(int num1, int num2, char operation) {
    switch (operation) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        default:
            return 0; // Return 0 for invalid operation
    }
}

// Function to process each line from the input file
void processLine(char* line, FILE* outputFile) {
    char roman1[100], roman2[100], operation;
    int result, num1, num2;

    // Parse the line for two Roman numerals and an operation
    if (sscanf(line, "%s %c %s", roman1, &operation, roman2) == 3) {
        num1 = romanToDecimal(roman1);
        num2 = romanToDecimal(roman2);

        if (num1 == -1 || num2 == -1) {
            fprintf(outputFile, "Error in input: Invalid Roman numeral in '%s'.\n", line);
            return; // Skip processing this line
        }

        result = performOperation(num1, num2, operation);
        fprintf(outputFile, "%s (%d) %c %s (%d) = %d\n", roman1, num1, operation, roman2, num2, result);
        fprintf(outputFile, "Result in words: ");
        writeInWords(result, outputFile); // Write result in words
        fprintf(outputFile, "\n\n"); // Add spacing for readability
    } else {
        fprintf(outputFile, "Error in input: Invalid format in '%s'.\n", line);
    }
}

// Main function
int main() {
    FILE* inputFile = fopen("Input.txt", "r");
    FILE* outputFile = fopen("Output.txt", "w");
    char line[256];

    if (inputFile == NULL) {
        printf("Error: Could not open Input.txt.\n");
        return 1;
    }
    if (outputFile == NULL) {
        printf("Error: Could not open Output.txt.\n");
        fclose(inputFile);
        return 1;
    }

    // Process each line from the input file
    while (fgets(line, sizeof(line), inputFile)) {
        processLine(line, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("Processing complete. Check Output.txt for results.\n");
    return 0;
}

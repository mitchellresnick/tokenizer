/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
        char* userInput; //the actual string to be processed
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
        if (strlen(ts) == 0) { //check if the input string is empty
                return NULL; //return null as per instructions
        } else { //the input string is not empty
                TokenizerT *tokenP = malloc(sizeof(TokenizerT)); //create an instance of the struct
                // tokenP->userInput = malloc(4000); //create a dynamic size of memory to hold the input string
                tokenP->userInput = malloc(strlen(ts)+1); //create a dynamic size of memory to hold the input string
                strcpy(tokenP->userInput, ts); //copy the ts var into the userInput
                return tokenP;
        }

}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
        free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {
        if (strlen(tk->userInput) == 0) { //if the token is empty
                return 0; //as per instructions
        }
        char * temp = malloc(strlen(tk->userInput)); //create a temp char var that will be set as the user input
        strcpy(temp, tk->userInput); //copy the value over to temp
        while (isspace(temp[0])) { //trims whitespace from the beginning of the token
                if (temp[0] == '\0') { //if the first char is the null byte, indicating an empty string
                        return 0;
                } else {
                        temp++; // pointer arithmatic, now points to the next character in the string
                        tk->userInput++; // pointer arithmatic, now points to the next character in the string
                }
        }
        char * nextToken = malloc(2); //allcated space for a single char that is to be returned
        int i; //counter for following loop
        for (i = 0; !isspace(temp[i]); i++) { //loop over the length of the temp string until a space is hit
                if (temp[i] == '\0') { //if there is a null byte, exit the loop
                        break;
                }
                nextToken = realloc(nextToken, (2*(i+1))); //realloc to make room for one more character
                nextToken[i] = temp [i]; //set the current position as the new character from input string
                nextToken[i+1] = '\0'; //set the last spot of the string as the null byte
                tk->userInput++; // pointer arithmatic, now points to the next character in the string
        }
        if (strlen(nextToken) == 0) { //final check to see if after removing all whitespace, the string is empty
                return 0;
        }
        return nextToken;
}

int isHex(char c){ //detects if a number is a hex number
        if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ||
            c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' ||
            c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F') {
                return 1;
        } else {
                return -1;
        }
}

int isOctal(char c){ //detects if a number is an octal number
        if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7') {
                return 1;
        } else {
                return -1;
        }
}

int isDigit(char c){ //detects if a number is a digit
        if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
                return 1;
        } else {
                return -1;
        }
}

int isEscape(char c){
        if (isspace(c) || isDigit(c) || isOctal(c) || isHex(c) || c == '.' || c=='x' || c=='X' || c=='e' || c=='E') {
                return -1;
        } else {
                return 1;
        }
}

void reportType(char* token, char type){ //function to report the result of the token type detection
        switch(type) {
        case 'z': //z for zero
                printf("%s %s\n", "zero", token); break;
        case 'd': //d for decimal
                printf("%s %s\n", "decimal", token); break;
        case 'o': //o for octal
                printf("%s %s\n", "octal", token); break;
        case 'h': //h for hexadecimal
                printf("%s %s\n", "hexadecimal", token); break;
        case 'f': //f for float
                printf("%s %s\n", "float", token); break;
        case 'i': //i for integer
                printf("%s %s\n", "decimal integer", token); break;
        case 'e': //escape character
                printf("%s %s\n", "escape character", token); break;
        }
}

char* splitToken (char* token, char c){
        char * temp = malloc(strlen(token)); //create a temp char var that will be set as the user input
        strcpy(temp, token); //copy the value over to temp
        char * nextToken = malloc(2); //allcated space for a single char that is to be returned
        int i; //counter for following loop
        if (isEscape(token[0]) > 0) {
                nextToken[0] = token[0];
                token++;
                return nextToken;
        } else {
                for (i = 0; isEscape(temp[i]) < 0; i++) { //loop over the length of the temp string until an escape is hit
                        if (temp[i] == '\0') { //if there is a null byte, exit the loop
                                break;
                        }
                        nextToken = realloc(nextToken, (2*(i+1))); //realloc to make room for one more character
                        nextToken[i] = temp [i]; //set the current position as the new character from input string
                        nextToken[i+1] = '\0'; //set the last spot of the string as the null byte
                        token++; // pointer arithmatic, now points to the next character in the string
                }
                if (strlen(nextToken) == 0) { //final check to see if after removing all whitespace, the string is empty
                        return 0;
                }
                return nextToken;
        }
}

void reportError(char* token, char c){ //function to report an error, basically just malformed token
        printf("malformed: %s, error on: [0x%x]\n", token, c);
}

void reportEscape(char* token, int position){
        char* newTokenEscape = malloc(position);
        int i;
        for (i = 0; i != position; i++) {
                newTokenEscape[i] = token[i];
        }
        newTokenEscape[position] = '\0';
        state_initial(newTokenEscape, 0);
        free(newTokenEscape);

        char* reportEscapeChar = malloc(2);
        reportEscapeChar[0] = token[position];
        reportEscapeChar[1] = '\0';
        reportType(reportEscapeChar, 'e');
        free(reportEscapeChar);

        char* newTokenEscapeTwo = malloc(strlen(token) - position);
        i = position+1;
        int j = 0;
        while (i != strlen(token)) {
                newTokenEscapeTwo[j] = token[i];
                j++;
                i++;
        }
        state_initial(newTokenEscapeTwo, 0);
        free(newTokenEscapeTwo);
}

int state_initial(char* token, int position){ //initial setting of the FSM
        if (token[position] == '0') {
                processZero_one(token, (position + 1));
        } else if (token[position] == '1' || token[position] == '2' || token[position] == '3'
                   || token[position] == '4' || token[position] == '5' || token[position] == '6'
                   || token[position] == '7' || token[position] == '8' || token[position] == '9') {
                processOnetoNine_seven(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

/* the following functions are all part of the FSM
 * processZero functiosn relate to processing tokens that start with 0
 * processOnetoNine functions relate to processing tokens that start with 1-9
 * the number after the _ is the number that is labeled in the FSM diagram in readme.pdf
 */
int processZero_one(char* token, int position){
        if (token[position] == '\0') {
                reportType(token, 'z');
        } else if (token[position] == 'x' || token[position] == 'X') {
                processZero_three(token, (position + 1));
        } else if (token[position] == '.') {
                processZero_five(token, (position + 1));
        } else if (isOctal(token[position]) > 0) {
                processZero_two(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processZero_two(char* token, int position){
        if (token[position] == '\0') {
                reportType(token, 'o');
        } else if (isOctal(token[position]) > 0) {
                processZero_two(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processZero_three(char* token, int position){
        if (token[position] == '\0') {
                reportError(token, token[position]);
        } else if (isHex(token[position]) > 0) {
                processZero_four(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processZero_four(char* token, int position){
        if (token[position] == '\0') {
                reportType(token, 'h');
        } else if (isHex(token[position] > 0)) {
                processZero_four(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processZero_five(char* token, int position){
        if (token[position] == '\0') {
                reportError(token, token[position]);
        } else if (isDigit(token[position]) > 0) {
                processZero_six(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processZero_six(char* token, int position){
        if (token[position] == '\0') {
                reportType(token, 'f');
        } else if (isDigit(token[position]) > 0) {
                processZero_six(token, (position + 1));
        } else if(token[position] == 'E' || token[position] == 'e') {
                processOnetoNine_ten(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processOnetoNine_seven(char* token, int position){
        if (token[position] == '\0') {
                reportType(token, 'i');
        } else if (isDigit(token[position]) > 0) {
                processOnetoNine_seven(token, (position + 1));
        } else if(token[position] == '.') {
                processOnetoNine_eight(token, (position + 1));
        } else if(token[position] == 'E' || token[position] == 'e') {
                processOnetoNine_ten(token, (position + 1));
        } else{
                reportEscape(token, position);
        }
}

int processOnetoNine_eight(char* token, int position){
        if (token[position] == '\0') {
                reportError(token, token[position]);
        } else if (isDigit(token[position]) > 0) {
                processOnetoNine_nine(token, (position + 1));
        } else{
                reportEscape(token, position);
        }
}

int processOnetoNine_nine(char* token, int position){
        if (token[position] == '\0') {
                reportType(token, 'f');
        } else if (token[position] == 'E' || token[position] == 'e') {
                processOnetoNine_ten(token, (position + 1));
        } else if (isDigit(token[position]) > 0) {
                processOnetoNine_nine(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processOnetoNine_ten(char* token, int position){
        if (token[position] == '\0') {
                reportError(token, token[position]);
        } else if (token[position] == '+' || token[position] == '-') {
                processOnetoNine_eleven(token, (position + 1));
        } else if (isDigit(token[position]) > 0) {
                processOnetoNine_twelve(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processOnetoNine_eleven(char* token, int position){
        if (token[position] == '\0') {
                reportError(token, token[position]);
        } else if (isDigit(token[position]) > 0) {
                processOnetoNine_twelve(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

int processOnetoNine_twelve(char* token, int position){
        if (token[position] == '\0') {
                reportType(token, 'f');
        } else if (isDigit(token[position]) > 0) {
                processOnetoNine_twelve(token, (position + 1));
        } else {
                reportEscape(token, position);
        }
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
        TokenizerT *inputP; //pointer for the program
        char * token;
        inputP = TKCreate(argv[1]);//TKCreate
        if (inputP == NULL) {
                printf("Error, TKCreate failed (returned NULL)");
                return 0;
        }
        token = TKGetNextToken(inputP);
        while (token != 0) {
                state_initial(token, 0);
                token = TKGetNextToken(inputP);
        }
        TKDestroy(inputP);
        return 0;
}

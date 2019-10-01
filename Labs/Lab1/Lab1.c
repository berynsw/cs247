/*
  Course: CSCI 247 - Spring 2019
  Beryn Staub-Waldenberg: Lab1.c
*/

/*
  preprocessor directives: tells preprocessor to include other necessary files such as stdio.h

  forward declarations: these tell the compiler what arguments and return to expect from functions

  macros: often used like a constant variable, can be used on a C keyword in which case this keyword will be hidden from the compiler

  functions: separate functions outside of the main are more easily reusable and testable

  escape character(\): quotes generally signify the start or end of a string, but using the escape character beforehand, \" , enables you to put quotes in a string

  operators: are evaluated during compilation. Functions however are evaluated at runtime. Macros are evaluated by the preprocessor.

  keywords: predefined words that have a special meaning to the compiler. Some in this file: include, define, return

  significance of display function for printf: the display represents standard output, stdout

*/

#include <stdio.h>
#include <string.h>

#define DOUBLE_DATA_TYPE "double"

void DataTypeSizes(void);

char* GetStringFromStdin(char* str);

void DisplayString(const char* str);

int main(int argc, char* argv[]){

  printf("number of args being passed: %d\r\n", argc);
  printf("the args are: ");

  for(int i = 0; i < argc; i++) //found out I don't need brackets for loops!
    printf("%s ", argv[i]);

  printf("\n");
  printf("input a string to be displayed from stdout or just hit enter: ");

  char str[100];

  GetStringFromStdin(str);

  DisplayString(str);

  DataTypeSizes();

  return(0);
}

void DataTypeSizes(void){
  printf("\"%s\" is a standard C datatype. Size of a \"%s\" data type is = %ld\r\n", DOUBLE_DATA_TYPE, DOUBLE_DATA_TYPE, sizeof(double));
}

char* GetStringFromStdin(char* str){
    fgets(&str[0], 100, stdin);
    return &str[0];
}

void DisplayString(const char* str){
    fputs(str, stdout);
}

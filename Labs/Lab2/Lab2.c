#include <stdio.h>
#include <string.h>


//function for converting int digits to chars
char* itoa(int num, char* str, int base);

//function for reversing char array once all int digits have been converted to chars
char* revStr(char str[], int length);

int main(){
  char str[20];

  int var1 = 6070;
  printf("the string is: %d\n", var1);
  printf("the string is: %s\n", itoa(6070, str, 10));


}

char* itoa(int num, char* str, int base){

  //index of char[]
  int strDig = 0;
  //if 0 - positive number, if 1 - negative number
  int negative = 0;

  //case for num being 0
  if(num == 0){
    str[strDig] = '0';
    strDig++;
    str[strDig] = '\0';
    return str;
  }

  //case for negative numbers
  if(num < 0){
    num = -num;
    //only represents negative numbers in base 10
    if(base == 10)
      negative = 1;
  }

  //converts each digit to
  while(num != 0){

    //grabs least signif dig
    int remainder = num % base;

    //case for base 16 when digit is represented as a-f
    if(remainder > 9)
      str[strDig] = (remainder - 10) + 'a';
    //else case for all other circumstances: base 8, 10, and base 16 digits less than 10
    else
      str[strDig] = remainder + '0';

    //chops off least siginif dig
    num = num/base;
    strDig++;
  }

  //adds negative sign
  if(negative){
    str[strDig] = '-';
    strDig++;
  }
  //adds null terminator
  str[strDig] = '\0';

  return revStr(str,strDig);
}

char* revStr(char* str, int length){
  //new reversed array
  char copy[length];

  //index for new array
  int j = 0;

  //traverses original array
  for(int i = length-1; i >= 0; i--){
    copy[j] = str[i];
    j++;
  }

  //add null terminator to new array
  copy[j] = '\0';

  //point originally passed variable to new array
  str = copy;
  return str;
}

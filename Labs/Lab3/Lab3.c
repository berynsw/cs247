#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//defining booleans
typedef enum {
  false,true
}bool;

int CountSetBits(unsigned int var);

int IdentifyUniqueInteger(int arrayCount, int* intArray);

int ReverseBits(int var);

bool OnlyOneBitSet(int var);

bool OnlyOneBitSetInEvenPostion(int var);

int ModWithoutUsingModOperator(int numerator, int denominator);

int SwapNibbles(int var);

int NumberOfOperationsRequired(unsigned int);

bool ispowerof2(unsigned int counter);

int NumberOfOperationsRequired2(unsigned int counter);

int main(int argc, char* argv[]){

    int x = atoi(argv[1]);
    //int y = atoi(argv[2]);



    /*
    unsigned int arr[5];

    for(int i = 0; i < 5; i++){
      arr[i] = atoi(argv[i+1]);
      printf("%i, ", arr[i]);
    }
    printf("\n");
    */



    //printf("number of set bits in arg1: %i\n", CountSetBits(x));

    //printf("unique integer in arg array: %i\n", IdentifyUniqueInteger(argc-1, arr));

    //printf("arg1 reversed: %i\n", ReverseBits(x));

    //printf("arg1 only one bit? %i\n", OnlyOneBitSet(x));

    //printf("arg1 one bit in even position? %i\n", OnlyOneBitSetInEvenPostion(x));

    //printf("arg1 mod arg2(1 bit): %i\n", ModWithoutUsingModOperator(x, y));

    //printf("arg1 with nibbles swapped = %i\n", SwapNibbles(x));

    printf("arg1 number of operations to get to 1 = %i\n", NumberOfOperationsRequired(x));

    printf("arg1 number of operations to get to 1 = %i\n", NumberOfOperationsRequired2(x));


    return 0;

}

//counts the number of bits set that makeup a given int
int CountSetBits(unsigned int var){
  int count = 0;
  while(var){
    //if the current bit is set
    if(var & 1)
      count++;
    var >>= 1;
  }
  return count;
}

//checks for an int in an array that only occurs once
int IdentifyUniqueInteger(int arrayCount, int* intArray){
  int a = intArray[0];
  for(int i = 1; i < arrayCount; i++){
    a ^= intArray[i];
  }
  //a unique int exclusive or-ed with all others in array will yield itself
  return a;
}

//reverses an integer bitwise
int ReverseBits(int var){

  int reverse;

  for(int i = 0; i < 32; i++){
    //checks if bit at current position, i, is set
    int mask = (var & (1 << i));
    //if it is it's set on the opposite side in the reverse int
    if(mask)
      reverse |= 1 << (31 - i);
  }
  return reverse;
}

//checks if only 1 bit is used in a given integer returns 1 if true, 0 if more than one bit is used
bool OnlyOneBitSet(int var){
  int count = 0;
  for(int i = 0; i < 32; i++){
    int mask = (var & (1 << i));
    if(mask)
      count++;
  }
  if(count == 1)
    return true;
  else
    return false;
}


//checks if the position of the first set bit is even ie. int 1 = bit 1 = odd, int 8 = bit 4 = even
bool OnlyOneBitSetInEvenPostion(int var){
  if(OnlyOneBitSet(var)){
    for(int i = 1; i < 33; i++){
      int mask = (var & (1 << (i - 1)));
      //if bit is set and position is even
      if(mask && (i % 2 == 0))
        return true;
    }
  }
  return false;
}

//uses bitwise & to remove bit representing denominator and everything to the left of (more significant than) it
int ModWithoutUsingModOperator(int numerator, int denominator){
  if(OnlyOneBitSet(denominator))
    return numerator & (denominator - 1);
  return 0;
}

//swaps nibbles in each byte of an integer
int SwapNibbles(int var){
  int copy;

  //incrementing value of i used to switch to the next byte (+8 each iteration)
  for(int i = 0; i < 32; i += 8){
    //grab right nibble and shift left
    int rightnibble = ((0x0f << i) & var);
    rightnibble <<= 4;

    copy |= rightnibble;

    //grab left nibble and shift right
    int leftnibble = ((0xf0 << i) & var);
    leftnibble >>= 4;

    copy |= leftnibble;
  }
  return copy;

}

int NumberOfOperationsRequired(unsigned int counter){
  int numberofoperations = 0;
  while(counter > 1){
    //if counter is a power of 2
    if(ispowerof2(counter)){
      counter = counter >> 1;
    }
    //counter is not a power of 2
    else{
      unsigned int temp;
      for(temp = counter; temp >= 2; temp--){
        if(ispowerof2(temp)){
          counter = counter - temp;
          break;
        }
      }
    }
    numberofoperations++;
  }
  return numberofoperations;
}

//checks if number is power of 2
bool ispowerof2(unsigned int counter){
  if((counter & (counter - 1)) == 0)
    return true;
  return false;
}


int NumberOfOperationsRequired2(unsigned int counter){
  if(counter == 1)
    return 0;
  return CountSetBits(counter - 1);
}

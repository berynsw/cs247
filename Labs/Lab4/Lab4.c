/*
Beryn Staub-Waldenberg cs247 lab4 spring2019
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned float_bits;

int floatClass(float_bits f);

float_bits float_negate(float_bits f);

float_bits float_absval(float_bits f);

float_bits float_twice(float_bits f);

float_bits float_half(float_bits f);

int main (int argc, char *argv[]){


  unsigned int x = 0x00000030;

  printf("type of float: %X\n", floatClass(x));

  printf("negated float: %X\n", float_negate(x));

  printf("absval of float: %X\n", float_absval(x));

  printf("doubled float: %X\n", float_twice(x));

  printf("halved float: %X\n", float_half(x));


}

int floatClass(float_bits f){

  //bit mask for expon bits
  unsigned int expon = (0x7f800000 & f);


  //bit mask for mantessa bits
  unsigned int mantessa = 0x007fffff & f;


  if(expon == 0){

    //zero
    if(mantessa == 0)
      return 0;

    //small denormalized number
    else
      return 3;
  }
  //special case
  else if(expon == 0x7f800000){

    //infinity
    if(mantessa == 0)
      return 1;

    //NaN
    else
      return 2;
  }
  //normalized number
  else{
    return 4;
  }
}

//change the sign of a float
float_bits float_negate(float_bits f){

  //flips sign bit
  return (0x80000000 ^ f);
}

//take the absolute val of a float (set sign to 0)
float_bits float_absval(float_bits f){
  return (0x7fffffff & f);
}



float_bits float_twice(float_bits f){

  //float is normal
  if(floatClass(f) == 4)
    //adds 1 to the exponent bits
    return f + 0x00800000;

  //small denormalized
  else if(floatClass(f) == 3)
    return (f << 1);

  //zero, infinity, or NaN
  else
    return f;
}

float_bits float_half(float_bits f){

  //float is normal
  if(floatClass(f) == 4)
    //subtracts 1 from exponent bits
    return f - 0x00800000;

  //small denormalized
  else if(floatClass(f) == 3)
    return (f >> 1);

  //zero, infinity, or NaN
  else
    return f;
}

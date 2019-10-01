#include <stdio.h>
#include <string.h>









int main(){


  int x = 0;
  int y = 0;
  y += x;
  x += y;
  y += x++;



  //move x into eax
  //move 0x1(%rax) into x
  //add eax to y

  return 0;
}

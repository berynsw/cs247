#include <stdio.h>
#include <string.h>









int main(){
  int g;
  int arr[4];
  int arr2[3];


  arr2[0] = 23;   //-0x18(%rbp)  a
  arr2[1] = 19;  //-0x1c(%rbp)   b
  arr2[2] = 12;  //-0x20(%rbp)   c


  arr[0] = 2; //-0x28(%rbp)    d
  arr[1] = 3; //-0x2c(%rbp)    e
  arr[2] = 1; //-0x30(%rbp)    f




  g = 0;  //-0x34(%rbp)         g


  g  = arr[2] * arr2[0] + (arr[1] * arr2[1]);

  g += arr2[2] * arr[0];


  return 0;
}

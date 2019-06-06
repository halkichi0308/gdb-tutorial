#include <stdio.h>
#include <strings.h>

int secret = 0x12345678;

void runme(){
  static const int *buf[] = {
    0x66, 0x68, 0x59, 0x5b,
    0x6b, 0x34, 0x1c, 0x50,
    0x4c, 0xc, 0x37, 0x3b,
    0x34, 0x2e, 0x45
  };
  for(int i=0; i<15; i++){
    printf("%c", buf[i] + (int)i);
  }
  puts("\n");

  return 0;
}

int main(){
  char arg;
  puts("Do u know gdb?");

  scanf("%d", &arg);

  if(secret == 0xff){
    if(secret == 0x03){
      runme();
    }
  }
  puts("Please try again.");
  return 0;
}

//gcc -o runme runme.c -m32 -fno-stack-protector -std=c99
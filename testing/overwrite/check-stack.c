/**
   Demonstration that full stack contents can be accessed from subroutines
   main has a character array with the contents 'ABCDEFGHIJ'
   main calls f, the character array is on the stack
   f has a character array with the contents 'KLMNOPQRST'
   f calls g with a pointer (ptr) to f's character array which is on the stack
   g has no direct access to main's character string.
   But, beginning with the reference to f's character string, we can check the
   contents of the stack, byte by byte, in g's for loop and print it to the 
   console.  What we see is both 'ABCDEFGHIJ' and 'KLMNOPQRST' and they are not 
   far from each other.

   Note: on different platforms, a few minor adjustments may need to be made
   such as changing the range of i in the for loop to locate both strings.
   Note: to better locate the strings, any ascii value between 'A' and 'T'
   is printed as an ascii value and other values are printed either as hex
   values or as '.' depending on whether an argument is supplied to the 
   command.
   Note: run it several times for different numbers except for the strings.
   Note: g's for loop decrements i instead of increments i so that addresses
   are readable in the normal way (leftmost is highest value digit) but this 
   causes the strings to appear reversed.  If you don't like this, just
   modify g's for loop appropriately.

   Usage: check-stack            -> '.' printed if not between 'A','T'
          check-stack any-arg    -> hex values printed if not between 'A','T'
**/
#include <stdio.h>
int out;

void g(char *ptr) {
   int i;
   for (i=100 ; i > -5 ; i--) {
      if (*(ptr+i) < 85 && *(ptr+i) > 64) printf("%c",*(ptr+i));	
      else if (out == 1 && (unsigned char)(*(ptr+i)) < 16) printf("0%x",(unsigned char)(*(ptr+i)));
      else if (out == 1) printf("0%2x",(unsigned char)(*(ptr+i)));		
      else printf(".");
   }
   return;
}

void f(char *ptr) {
   int i;
   char s[10];
   s[0]='K'; s[1]='L'; s[2]='M'; s[3]='N'; s[4]='O'; 
   s[5]='P'; s[6]='Q'; s[7]='R'; s[8]='S'; s[9]='T';
   g(s);
   return;
}

int main (int argc, char **argv) {
   if (argc > 1) out = 1; else out = 0;
   char s[10];
   printf("main=%lx s=%lx\n", (unsigned long)main, (unsigned long)s);
   s[0]='A'; s[1]='B'; s[2]='C'; s[3]='D'; s[4]='E'; 
   s[5]='F'; s[6]='G'; s[7]='H'; s[8]='I'; s[9]='J';
   f(s);
   printf("\n");
}

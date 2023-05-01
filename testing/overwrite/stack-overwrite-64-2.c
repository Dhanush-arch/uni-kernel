/***
   Stack overwrite example.
   Overwrite return from main with address of g
   Compile with 
     gcc -fno-stack-protector stack-overwrite-2.c -o stack-overwrite-2

   Note: you may have to change the address bytes 0x7d, 0x5, 0x40
   look at the results printed to console for the proper address values
   (that is, the address of g). 
   Note: the address location may be different on your computer.  You can
   experiment by changing s[24], s[25]... to s[24+j], s[25+j]...
   and choosing different values of j.
***/
#include <stdio.h>
typedef unsigned long ul;

void g() {  printf("Gotcha!\n"); }

char *f (char *x) {
   int i;
   char *ptr;
   printf("Made it into f - returning now\n");
   return x;
}

int main () {
   int i;
   char s[8];
   printf("main=%lx, g=%lx\n",(ul)main, (long)g);
   for (i=0 ; i < 24 ; i++) s[i] = 'A';
   s[24] = 0x7d;
   s[25] = 0x5;
   s[26] = 0x40;
   for (i=27 ; i < 31 ; i++) s[i] = 0x0;
   f(s);
   printf("Done with f\n");
}

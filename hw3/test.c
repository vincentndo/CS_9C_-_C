#include <stdio.h>
#include <string.h>

int main ()
{
   char src[50], *dest;

   strcpy(src,  "This is source");
   strcpy(dest, "This is destination");

   strcat(dest, "   ");

   printf("Final destination string : |%s|\n", dest);
   
   return(0);
}
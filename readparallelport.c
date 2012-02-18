#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>

#define base 0x378   /* I/O address to read */
#define ctrl 0x37A   /* Control register */
#define read 32


main(int argc, char **argv)
{
   int value;

   if (ioperm(base,1,1))
      fprintf(stderr, "Couldn't get the port at %x\n", base), exit(1);
   if (ioperm(ctrl,1,1))
      fprintf(stderr, "Couldn't get the port at %x\n", ctrl), exit(1);

   outb(read,ctrl);

   for(;;)
   {
      value = inb(base);
      printf("Port 0x%x read value is %i \n",base,value);      usleep(100000);
   }
}
 
/* compile with  command line gcc -O lpt_read.c -o lpt_read */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>

#define base 0x378   /* I/O address to read */
#define stat 0x379   /* Status register */
#define ctrl 0x37A   /* Control register */
#define read 32      /* Read I/O port command*/
#define true 1
#define false 0


main(int argc, char **argv)
{
   int value;
   char flag;

   if (ioperm(base,1,1))
      fprintf(stderr, "Couldn't get the port at %x\n", base), exit(1);
   if (ioperm(stat,1,1))
      fprintf(stderr, "Couldn't get the port at %x\n", stat), exit(1);
   if (ioperm(ctrl,1,1))
      fprintf(stderr, "Couldn't get the port at %x\n", ctrl), exit(1);

   outb(read,ctrl);

   for(;;)
   {
      flag = inb(stat);
      flag = flag & 0x40;
      if(flag == 0x40)
      {
         value = inb(base);
         printf("Port 0x%x read value is %x %c\n",base,value,value);
         while(flag)
         {
         flag = inb(stat);
         flag = flag & 0x40;
         }               }
   }}
 
/* compile with  command line gcc -O lpt_read.c -o lpt_read */


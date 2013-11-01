#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/io.h>

#define base 0x378 /* I/O address to read */
#define stat 0x379 /* Status register */
#define ctrl 0x37A /* Control register */
#define read 32 /* Read I/O port command*/
#define true 1
#define false 0


main(int argc, char **argv)
{
   int value, epromaddr;
   char flag;
   FILE *fp;

   if (ioperm(base,1,1)) {
      fprintf(stderr, "Couldn't get the port at %x\n", base)
      exit(1);
   }
      if (ioperm(base,1,1)) {
      fprintf(stderr, "Couldn't get the port at %x\n", stat)
      exit(1);
   }
      if (ioperm(base,1,1)) {
      fprintf(stderr, "Couldn't get the port at %x\n", ctrl)
      exit(1);
   }

   fp = fopen("EPROM.txt", "w");
   if (fp == NULL) {
      printf("Error opening EPROM.txt\n");
      exit(0);
   }   

   outb(read,ctrl);

   for(epromaddr=0; epromaddr<=65536; ++epromaddr)
   {
      flag = inb(stat) & 0x40;
      if(flag == 0x40)
      {
         value = inb(base);
         printf("Address 0x%x read value is %x %c\n",epromaddr,value,value);
		 fprintf(fp, "%x : %x %c\n",epromaddr,value,value);
         while(flag)
         {
            flag = inb(stat) & 0x40;
         }
      }
   }
   fclose(fp);
   return(0);
}
/* YAEP
   Yet Another EPROM Programmer
   (C) Copywrite Marc Cleave 2013
   Project page: https://github.com/bootnecklad/PROM-project
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/io.h>

#define BASE 0x378 /* I/O address to read */
#define STAT 0x379 /* Status register */
#define CTRL 0x37A /* Control register */
#define TRUE 1
#define FALSE 0

#define WRITE_TIME_PERIOD 100 // number of milli seconds that EPROM needs to write value for

/*
   status register: 0x80 bit is READY?

   hardware commands:
    0x0 bit is
    0x1 bit is WRITE EPROM
    0x2 bit is
    0x3 bit is
    0x4 bit is
    0x5 bit is
    0x6 bit is
    0x7 bit is
    0x8 bit is
    0x9 bit is
    0xa bit is
    0xb bit is
    0xc bit is WRITE EPROM ADDRESS LOW
    0xd bit is WRITE EPROM ADDRESS HIGH
    0xe bit is WRITE BYTE LOW
    0xf bit is WRITE BYTE HIGH

    // steps for writing eprom
    //   output address LOW
    //   pulse CTRL 0x10
    //   output address HIGH
    //   pulse CTRL 0x20
    //   output byte
    //   pulse 0x01 [PROGRAM]

*/



void writeEPROM(FILE* inputFile);
int checkReady();
int checkPorts();
void msleep(int milliseconds);
void writeControl(int hexCommand, int timePeriod);

void writeEPROM(FILE* inputFile)
{
  char seperator, flag;
  int inputBytes[4], address;
  int byteCount;

  while(fscanf(inputFile, "%x %c %x %x %x %x", &address, &seperator, &inputBytes[0], &inputBytes[1], &inputBytes[2], &inputBytes[3]) == 6)
  {
    printf("%x %x %x %x %x\n", address, inputBytes[0], inputBytes[1], inputBytes[2], inputBytes[3]); // Displays current line to console

    for(byteCount=0; byteCount<5; byteCount++)
    {
      address = address + byteCount; // Matches correct address to correct bytes

      outb(address & 0xFF00, BASE); // Outputs low byte of address
      writeControl(0x0C, 100); // Writes low byte of address to register

      outb(address & 0x0FF, BASE); // Outputs high byte of address
      writeControl(0x0D, 100); // Writes high byte of address to register

      outb(inputBytes[byteCount] & 0xFF00, BASE); // Outputs low byte to be written
      writeControl(0x0E, 100); // Writes low byte of byte

      outb(inputBytes[byteCount] & 0x00FF, BASE); // Outputs high byte to be written
      writeControl(0x0E, 100); // Writes high byte of byte

      writeControl(0xFF, 100); // SOMETHING PREPARING, SWITCH VOLTAGE ON?
      writeControl(0x01, WRITE_TIME_PERIOD); // Actually writes byte to EPROM
      writeControl(0xFF, 100); // SOMETHING, SWITCH VOLTAGE OFF?
    }

  }
  fclose(inputFile);
}


void writeControl(int hexCommand, int timePeriod)
{
  outb(hexCommand, CTRL); // Sets line high
  msleep(timePeriod); // Time period
  outb(0x00, CTRL); // Sets line low
}

void msleep(int milliseconds)
{
  usleep(milliseconds * 1000);
}


int checkPorts()
{
  if (ioperm(BASE,1,1)) {
    fprintf(stderr, "Couldn't get the port at %x\n", BASE);
    return(1);
  }
  if (ioperm(BASE,1,1)) {
    fprintf(stderr, "Couldn't get the port at %x\n", STAT);
    return(1);
  }
  if (ioperm(BASE,1,1)) {
    fprintf(stderr, "Couldn't get the port at %x\n", CTRL);
    return(1);
  }

  return(0);
}


int checkReady()
{
  char flagReady; // Flag used to tell if hardware is ready
  flagReady = inb(STAT) & 0x80; // Gets flag from ready switch from hardware

  if(flagReady != 0x80)
  {
    printf("Hardware not ready, please check:\n");
    printf(" - EPROM is plugged in\n");
    printf(" - Ready switch is toggled correcly\n");
  }

  return(1);
}


int main(int argc, char** argv)
{
  printf("--------------------------------\n");
  printf(" Yet Another EPROM Programmer\n");
  printf("--------------------------------\n");

  FILE* inputFile; // File that machine code is read from

  // Reads console parameters and opens input file
  if(argc == 2)
  {
    inputFile = fopen(argv[1], "r"); // Opens input file
    if(inputFile == NULL) { // Checks if the input file actually exists
      fprintf(stderr, "Can't open input file!\n");
      return(1); // Not successful :(
    }

    printf("\nOpening file %s...\n", argv[1]);
    inputFile = fopen(argv[1], "r"); // Opens input file

    printf("\nChecking ports ...\n");
    if(checkPorts() == 1) // Checks if parallel ports is accessible
    {
      return(1);
    }

    printf("\nChecking if hardware is ready...\n");
    if(checkReady() == 1) // Checks if hardware is ready
    {
      return(1);
    }

    printf("\nAre you sure you want to burn this EPROM? [y/n]\n");
    if(getch() == 'y')
    {
      writeEPROM(inputFile); // Starts writing to the EPROM
    }

  } else {
    printf("Usage: %s input-file\n", argv[0]);
  }

  return(0);
}


/*

displayMachineCode things previously used

  //char thing, address[5];
  //int inputBytes[3], tempBytes[3];
  //char address[5], thing;// inputBytes[4][3];
  //printf("%s %s %s %s %s\n", address, inputBytes[0], inputBytes[1], inputBytes[2], inputBytes[3]);
//for(i=0, i<4, i++)
//{
//  tempBytes[i] =
//number = strtol(byteOne, NULL, 16);
//printf("%x\n", number);
//number = strtol(address, NULL, 16);
//printf("%x\n", number & 0x00FF);
*/
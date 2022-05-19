/*
 * @(#)keygen.c
 *
 * Copyright 2000, Aaron Ardiri (mailto:aaron@ardiri.com)
 * All rights reserved.
 *
 * This file was generated as part of the "dkjr" program developed for 
 * the Palm Computing Platform designed by Palm: http://www.palm.com/ 
 *
 * The contents of this file is confidential and proprietrary in nature 
 * ("Confidential Information"). Redistribution or modification without 
 * prior consent of the original author is prohibited. 
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_IDLENGTH 8

int
main(int argc, char *argv[])
{
  char hex[MAX_IDLENGTH+1] = {};
  char userID[256]         = {};

  printf("keygen MBRO\n");
  printf("  Copyright 2000 Aaron Ardiri (ardiri@palmgear.com)\n\n");

  fprintf(stdout, "userID:   ");
  fscanf(stdin, "%s", userID);

  // maybe it is in hex?
  if (sscanf(userID, "%x:%x:%x:%x:%x:%x:%x:%x:%x", 
             &hex[0], &hex[1], &hex[2], 
             &hex[3], &hex[4], &hex[5], 
             &hex[6], &hex[7], &hex[8]) == 9) {

    int i, checksum = 0;
    for (i=0; i<MAX_IDLENGTH; i++) {
      userID[i] = hex[i];
      checksum  ^= hex[i];
    }

    // lets check the checksum
    if (checksum != hex[MAX_IDLENGTH]) {
      fprintf(stdout, "ERROR: checksum digit invalid\n");
      exit(1);
    }
  }

  {
    FILE           *file;
    unsigned char  key;
    unsigned char  dhdr[88]     = {}; // header
    unsigned char  data[0x7FFF] = {}; // 32K of memory
    unsigned char  regi[0x7FFF] = {}; // 32K of memory
    unsigned short i, index, regiLength, dataLength;
  
    // read in the "encryption" key
    file       = fopen("@message","rb");
    dataLength = fread(data,1,0x7fff,file);
    fclose(file);

    // read in the data to encrypt
    file       = fopen("code0007.bin","rb");
    strcpy(regi, "|HaCkMe|");
    regiLength = fread(regi+16,1,0x7fff,file) + 16;
    fclose(file);
 
    strcpy(dhdr, "MBRO keygen");
    dhdr[0x20] = 0x80;
    dhdr[0x24] = 0xff; dhdr[0x25] = 0xff; dhdr[0x26] = 0xff; dhdr[0x27] = 0xff;
    dhdr[0x28] = 0xff; dhdr[0x29] = 0xff; dhdr[0x2a] = 0xff; dhdr[0x2b] = 0xff;
    dhdr[0x3c] = '_';  dhdr[0x3d] = 'k';  dhdr[0x3e] = 'e';  dhdr[0x3f] = 'y'; 
    dhdr[0x40] = 'M';  dhdr[0x41] = 'B';  dhdr[0x42] = 'R';  dhdr[0x43] = 'O'; 
    dhdr[0x4D] = 0x01;  // one record
    dhdr[0x51] = 0x58;  // offset (hard coded)

    // starting key = checksum
    key = 0;
    for (i=0; i<MAX_IDLENGTH; i++) {
      key += userID[i];
    }
    key = key & 0xff;
    if (key == 0) key = 0x20; // key *cannot* be zero

    // encrypt the chunk
    index = key;
    for (i=0; i<regiLength; i++) {

      // adjust the byte
      regi[i] ^= key;

      // dynamically update the key
      do {
        index = (index + key + 1) % dataLength;
        key   = data[index];
      } while (key == 0);
    }

    file = fopen("MBRO_keygen.pdb","wb");

    // write the data header 
    fwrite(dhdr,1,88,file);

    // write the new code into file
    fwrite(regi,1,regiLength,file);
    fclose(file);

    exit(0);
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "png_util/lab_png.h"
#include "png_util/crc.h"

int main(int argc, char *argv[]) {

    const char * file = "./images/red-green-16x16-corrupted.png";

    printf("READING PNG FILE: %s\n", file);

    FILE * pngStream = fopen(file, "rb");

    U8 * pngHeader = malloc(8 * sizeof(U8));

    fread(pngHeader, sizeof(U8), 8, pngStream);

    printf("PNG Header: ");
    // prints the header of a png file
    for (int i = 0; i < 8; i++) {
        printf("%x ", pngHeader[i]);
    }

    printf("\n");

    /*
    GET THE IHDR CHUNK
    ==================================================================
    */

    // read next 4 bytes to get the length of the data field
    int lengthIHDR = 0;
    fread(&lengthIHDR, 4, 1, pngStream);

    struct chunk ihdr = {.length = ntohl(lengthIHDR)};

    // read the type and data of chunk
    U8 ihdrMain[ihdr.length + 4];
    fread(&ihdrMain, 1, 4 + ihdr.length, pngStream);
    
    // set the type
    for (int i = 0; i < 4; i++) ihdr.type[i] = ihdrMain[i];

    // set the data
    U8 * ihdrData = malloc(ihdr.length * sizeof(U8));
    memcpy(ihdrData, ihdrMain+4, ihdr.length * sizeof(U8));
    ihdr.p_data = ihdrData;

    // read the crc
    fread(&ihdr.crc, sizeof(U32), 1, pngStream);
    ihdr.crc = ntohl(ihdr.crc);

    printf("Length of IHDR: %d\n", ihdr.length);
    printf("Type of IHDR: %c %c %c %c\n", ihdr.type[0], ihdr.type[1], ihdr.type[2], ihdr.type[3]);
    printf("Data of IHDR: ");
    for (int i = 0; i < ihdr.length; i++) {
        printf("%x ", ihdr.p_data[i]);
    }
    printf("\n");
    printf("CRC of IHDR: %x | Expected CRC of IHDR: %x\n", ihdr.crc, (U32)crc(ihdrMain, 4 + ihdr.length));

    /*
    GET THE IDAT CHUNK
    ==================================================================  
    */

    struct chunk idat;

    fread(&idat.length, 4, 1, pngStream);
    idat.length = ntohl(idat.length);

    U8 idatMain[idat.length + 4];
    fread(&idatMain, 1, 4 + idat.length, pngStream);

    for (int i = 0; i < 4; i++) idat.type[i] = idatMain[i];

    U8 * idatData = malloc(idat.length * sizeof(U8));
    memcpy(idatData, idatMain+4, idat.length * sizeof(U8));
    idat.p_data = idatData;

    // read the crc
    fread(&idat.crc, sizeof(U32), 1, pngStream);
    idat.crc = ntohl(idat.crc);

    printf("Length of IDAT: %d\n", idat.length);
    printf("Type of IDAT: %c %c %c %c\n", idat.type[0], idat.type[1], idat.type[2], idat.type[3]);
    printf("Data of IDAT: ");
    for (int i = 0; i < idat.length; i++) {
        printf("%x ", idat.p_data[i]);
    }
    printf("\n");
    printf("CRC of IDAT: %x | Expected CRC of IDAT: %x\n", idat.crc, (U32)crc(idatMain, 4 + idat.length));

    /*
    GET THE IEND CHUNK
    ==================================================================
    */

    struct chunk iend;

    fread(&iend.length, 4, 1, pngStream);
    iend.length = ntohl(iend.length);

    U8 iendMain[iend.length + 4];
    fread(&iendMain, 1, 4 + iend.length, pngStream);

    for (int i = 0; i < 4; i++) iend.type[i] = iendMain[i];

    U8 * iendData = malloc(iend.length * sizeof(U8));
    memcpy(iendData, iendMain+4, iend.length * sizeof(U8));
    iend.p_data = iendData;

    // read the crc
    fread(&iend.crc, sizeof(U32), 1, pngStream);
    iend.crc = ntohl(iend.crc);

    printf("Length of IEND: %d\n", iend.length);
    printf("Type of IEND: %c %c %c %c\n", iend.type[0], iend.type[1], iend.type[2], iend.type[3]);
    printf("Data of IEND: ");
    for (int i = 0; i < iend.length; i++) {
        printf("%x ", iend.p_data[i]);
    }
    printf("\n");
    printf("CRC of IEND: %x | Expected CRC of IEND: %x\n", iend.crc, (U32)crc(iendMain, 4 + iend.length));

    fclose(pngStream);
    free(ihdrData);
    free(idatData);
    free(iendData);
    free(pngHeader);

    return 0;
}
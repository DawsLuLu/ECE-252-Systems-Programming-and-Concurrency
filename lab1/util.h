#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#include "lab_png.h"
#include "crc.h"
#include "zutil.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

/*
    fp is the filepointer for the png file

    U8 is a pointer to an array of U8, the first 8 bytes will be set to the value of the png header

    Returns -1 on invalid png header, 0 otherwise

    NOTE: fp will be rewinded at the start of the functiom
*/
int check_png_header(U8 * out, FILE * fp);

/*
    fp is the filepointer for the png file, it is assumed to be pointing to the first byte of the chunk

    The out parameter will be set with the chunk type, length, data, and crc

    Returns -1 on error, 0 otherwise
*/
int get_chunk(chunk_p out, FILE * fp, long offset, int whence);

/*
    Calculates the crc for the chunk, returns the crc as a 32 bit unsigned integer in host order

    Returns the calculated crc
*/
U32 check_crc(chunk_p const chunk);

/*
    Prints out the information in a chunk
*/
void print_chunk(chunk_p const chunk);

/*
    Returns 0 if the given file name is a directory 
*/
int is_dir(char *dir);

/*
    Prints all pngs in the given directory, uses fullpath as the base directory to start logging file names 
*/
int findPng(char *directory, int *foundPng, char *fullPath);

/*
    Takes two compressed data fields, concats them, then sets * out to the compressed data

    Returns -1 on error, 0 otherwise
*/
int concat_data_chunks(chunk_p out, struct chunk * data, U64 width, U64 total_h, size_t imgc);

////////////////////////////////////////////////////////////////////////
// COMP1521 21T3 --- Assignment 2: `chicken', a simple file archiver
// <https://www.cse.unsw.edu.au/~cs1521/21T3/assignments/ass2/index.html>
//
// Written by MOHAMMAD M. RAKIB (z5361151) on 20/11/2.
//
// 2021-11-08   v1.1    Team COMP1521 <cs1521 at cse.unsw.edu.au>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "chicken.h"

#define TRUE 1
#define FALSE 0

// Assumes file pointer is set to start of egglet 
unsigned long get_egglet_size(FILE *eggfile) {
    long egglet_start_pos = ftell(eggfile);

    fseek(eggfile, EGG_OFFSET_PATHNLEN, SEEK_CUR);

    unsigned int path_byte1 = fgetc(eggfile);
    unsigned int path_byte2 = fgetc(eggfile);
    unsigned int pathnlen = path_byte1 | (path_byte2 << 8);

    fseek(eggfile, egglet_start_pos, SEEK_SET); // Reset file pntr back to egglet start

    fseek(eggfile, EGG_OFFSET_PATHNLEN + EGG_LENGTH_PATHNLEN + pathnlen, SEEK_CUR);

    unsigned long byte1 = (unsigned long)fgetc(eggfile);
    unsigned long byte2 = (unsigned long)fgetc(eggfile);
    unsigned long byte3 = (unsigned long)fgetc(eggfile);
    unsigned long byte4 = (unsigned long)fgetc(eggfile);
    unsigned long byte5 = (unsigned long)fgetc(eggfile);
    unsigned long byte6 = (unsigned long)fgetc(eggfile);
    unsigned long contentlen = byte1 | (byte2 << 8) | (byte3 << 16) |
                           (byte4 << 24) | (byte5 << 32) | (byte6 << 40);

    fseek(eggfile, egglet_start_pos, SEEK_SET); // Reset file pntr back to egglet start
    
    return (21 + pathnlen + contentlen);
}

// Assumes file pointer is set to start of egg
int get_egglet_count(FILE *eggfile) {
    int count = 0;
    rewind(eggfile);

    while (fseek(eggfile, get_egglet_size(eggfile), SEEK_CUR) == 0) {
        count++;
    }

    rewind(eggfile);

    return count;
}

// Assumes file pointer is set to start of egglet
// If successful, returns TRUE, if failure, returns FALSE
int egglet_next(FILE *eggfile) {
    if (fseek(eggfile, get_egglet_size(eggfile), SEEK_CUR) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// Assumes file pointer is set to start of egglet
// NOTE: requires free() call to deallocate memory
char *get_egglet_pathname(FILE *eggfile) {
    long egglet_start_pos = ftell(eggfile);

    fseek(eggfile, EGG_LENGTH_PATHNLEN, SEEK_CUR);

    int byte1 = fgetc(eggfile);
    int byte2 = fgetc(eggfile);
    int pathnlen = byte1 | (byte2 << 8);

    fseek(eggfile, egglet_start_pos, SEEK_SET); // Reset file pntr back to egglet start
    fseek(eggfile, EGG_LENGTH_PATHNLEN + 2, SEEK_CUR);

    char *pathname = malloc(pathnlen + 1);

    fread(pathname, 1, pathnlen, eggfile);
    pathname[pathnlen] = '\0';

    fseek(eggfile, egglet_start_pos, SEEK_SET); // Reset file pntr back to egglet start

    return pathname;
}

// print the files & directories stored in egg_pathname (subset 0)
//
// if long_listing is non-zero then file/directory permissions, formats & sizes are also printed (subset 0)

void list_egg(char *egg_pathname, int long_listing) {
    FILE *eggfile = fopen(egg_pathname, "r+b");
    rewind(eggfile); // Sets file pntr to beginning of file

    // int egglet_count = get_egglet_count(eggfile);
    // for (int i = 0; i < egglet_count; i++) {
    //     char *pathname = get_egglet_pathname(eggfile);

    //     printf("%s\n", pathname);

    //     free(pathname); // Necessary free() call after using get_egglet_pathname

    //     egglet_next(eggfile);
    // }

    // DEBUG INFO
    // int egglet_count = get_egglet_count(eggfile);
    // printf("egglet_count: %d\n", egglet_count);

    char *pathname1 = get_egglet_pathname(eggfile);
    printf("pathname1: %s\n", pathname1);

    unsigned long egglet_size = get_egglet_size(eggfile);
    printf("egglet1_size: %lu\n", egglet_size); 

    free(pathname1);

    fclose(eggfile);
}


// check the files & directories stored in egg_pathname (subset 1)
//
// prints the files & directories stored in egg_pathname with a message
// either, indicating the hash byte is correct, or
// indicating the hash byte is incorrect, what the incorrect value is and the correct value would be

void check_egg(char *egg_pathname) {

    // REPLACE THIS PRINTF WITH YOUR CODE

    printf("check_egg called to check egg: '%s'\n", egg_pathname);
}


// extract the files/directories stored in egg_pathname (subset 2 & 3)

void extract_egg(char *egg_pathname) {

    // REPLACE THIS PRINTF WITH YOUR CODE

    printf("extract_egg called to extract egg: '%s'\n", egg_pathname);
}


// create egg_pathname containing the files or directories specified in pathnames (subset 3)
//
// if append is zero egg_pathname should be over-written if it exists
// if append is non-zero egglets should be instead appended to egg_pathname if it exists
//
// format specifies the egglet format to use, it must be one EGGLET_FMT_6,EGGLET_FMT_7 or EGGLET_FMT_8

void create_egg(char *egg_pathname, int append, int format,
                int n_pathnames, char *pathnames[n_pathnames]) {

    // REPLACE THIS CODE PRINTFS WITH YOUR CODE

    printf("create_egg called to create egg: '%s'\n", egg_pathname);
    printf("format = %x\n", format);
    printf("append = %d\n", append);
    printf("These %d pathnames specified:\n", n_pathnames);
    for (int p = 0; p < n_pathnames; p++) {
        printf("%s\n", pathnames[p]);
    }
}
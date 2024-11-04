////////////////////////////////////////////////////////////////////////
// COMP1521 21T3 --- Assignment 2: `chicken', a simple file archiver
// <https://www.cse.unsw.edu.au/~cs1521/21T3/assignments/ass2/index.html>
//
// Written by Mohammad M. Rakib (z5361511) on 20th of November, 2021
//
// 2021-11-08   v1.1    Team COMP1521 <cs1521 at cse.unsw.edu.au>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h> // stat()
#include <unistd.h> // POSIX API, useful constants/fucntions
#include <dirent.h> // opendir(), readdir() and closedir()

#include "chicken.h"

// Truth definitions
#define TRUE 1
#define FALSE 0

// This function returns the length of a file
long get_file_length(FILE *stream) {
    long curr_pos = ftell(stream);
    long file_length;

    // Sets file pointer to end of file stream
    if (fseek(stream, 0, SEEK_END) != 0) {
        perror("error");
        exit(EXIT_FAILURE);
    } 

    file_length = ftell(stream); // Returns file position to end of file stream, 
                                 // which gives file length in bytes

    // Returns file pointer to wherever it was
    if (fseek(stream, curr_pos, SEEK_SET) != 0) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    return file_length;
}

// This function is a wrapper function for fseek()
// Has added error-checking and bounds-checking
int goto_pos(FILE *stream, long pos, int origin) {
    int error = fseek(stream, pos, origin);
    
    long curr_pos = ftell(stream);
    long file_length = get_file_length(stream);

    if (curr_pos >= file_length) return -1; // Checks if file pointer is past EOF
    if (ferror(stream) != 0) return -2;
    if (error != 0) return -3;

    return 0;
}

// This function returns the pathname length of an egglet
// Assumes file pointer is at start of egglet
uint16_t get_egglet_pathlen(FILE *stream) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    goto_pos(stream, EGG_OFFSET_PATHNLEN, SEEK_CUR); // Go to pathname length section of egglet

    // Retrieve two bytes in little-endian form
    uint16_t byte1 = fgetc(stream);
    uint16_t byte2 = fgetc(stream);

    // Combine two bytes, where byte1 is least sig. byte and byte2 is most sig. byte
    uint16_t pathlen = byte1 | (byte2 << 8);

    goto_pos(stream, start_pos, SEEK_SET); // Reset position to start of egglet

    return pathlen;
}

// This function returns the content length of an egglet
// Assumes file pointer is at start of egglet
uint64_t get_egglet_contentlen(FILE *stream) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    uint16_t pathlen = get_egglet_pathlen(stream); // Requires path length for content length offset

    // Go to content length section of egglet
    goto_pos(stream, EGG_OFFSET_PATHNLEN + EGG_LENGTH_PATHNLEN + pathlen, SEEK_CUR);

    // Retrieve six bytes in little-endian form
    uint64_t byte1 = fgetc(stream);
    uint64_t byte2 = fgetc(stream);
    uint64_t byte3 = fgetc(stream);
    uint64_t byte4 = fgetc(stream);
    uint64_t byte5 = fgetc(stream);
    uint64_t byte6 = fgetc(stream);
    
    // Combine six bytes, where byte1 is least sig. byte and byte6 is most sig. byte 
    uint64_t contentlen = byte1 | (byte2 << 8) | (byte3 << 16) | 
                (byte4 << 24) | (byte5 << 32) | (byte6 << 40);
    
    goto_pos(stream, start_pos, SEEK_SET); // Reset position to start of egglet

    return contentlen;
}

// This function returns the overall length of an egglet
// Assumes file pointer is at start of egglet
uint64_t get_egglet_size(FILE *stream) {
    uint16_t pathlen = get_egglet_pathlen(stream);
    uint64_t contentlen = get_egglet_contentlen(stream);

    uint64_t eggletlen = 21 + pathlen + contentlen; // 21 bytes for all metadata bytes in an egglet

    return eggletlen;
}

// This function returns the pathname of an egglet
// Assumes file pointer is at start of egglet
// NOTE: Needs to be free'd after use
char *get_egglet_pathname(FILE *stream) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    uint16_t pathlen = get_egglet_pathlen(stream);
    char *pathname = malloc(pathlen + 1); // Allocate space for pathname, +1 for null-terminator

    // Go to pathname section of egglet
    goto_pos(stream, EGG_OFFSET_PATHNLEN + EGG_LENGTH_PATHNLEN, SEEK_CUR);

    // Read pathlen bytes from stream
    if (fread(pathname, 1, pathlen, stream) != pathlen) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    pathname[pathlen] = '\0'; // Set null-terminator

    goto_pos(stream, start_pos, SEEK_SET); // Reset position to start of egglet

    return pathname;
}

// This function returns the mode of an egglet (string form)
// Assumes file pointer is at start of egglet
// NOTE: Needs to be free'd after use
char *get_egglet_mode(FILE *stream) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    char *mode = malloc(EGG_LENGTH_MODE + 1); // Allocate space for mode, +1 for null-terminator

    // Go to mode/permissions section of egglet
    goto_pos(stream, EGG_OFFSET_MODE, SEEK_CUR); 

    // Read 10 bytes into mode string
    for (int i = 0; i < EGG_LENGTH_MODE; i++) {
        mode[i] = fgetc(stream);
    }
    mode[EGG_LENGTH_MODE] = '\0'; // Set null-terminator

    goto_pos(stream, start_pos, SEEK_SET); // Reset position to start of egglet

    return mode;
}

// This function returns the format of an egglet
// Assumes file pointer is at start of egglet
uint8_t get_egglet_format(FILE *stream) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    // Go to format section of an egglet
    goto_pos(stream, EGG_OFFSET_FORMAT, SEEK_CUR); 

    uint8_t format = fgetc(stream); // Read format byte

    goto_pos(stream, start_pos, SEEK_SET); // Reset position to start of egglet

    return format;
}

// This function moves file pointer from start of current egglet to start of next egglet
// Assumes file pointer is at start of egglet
int egglet_next(FILE *stream) {
    // Offsets current position by size of current egglet to get start of next egglet
    // Performs bounds-checking, returns TRUE if valid and FALSE if invalid
    if (goto_pos(stream, get_egglet_size(stream), SEEK_CUR) == 0) {
        return TRUE; 
    } else {
        return FALSE;
    }
}

// This function returns the hash of an egglet
// Assumes file pointer is at start of egglet
uint8_t get_egglet_hash(FILE *stream) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    uint16_t pathlen = get_egglet_pathlen(stream);
    uint64_t contentlen = get_egglet_contentlen(stream);

    // Go to hash section of egglet
    goto_pos(stream, 20 + pathlen + contentlen, SEEK_CUR);

    uint8_t hash = fgetc(stream); // Read hash byte

    goto_pos(stream, start_pos, SEEK_SET); // Reset position to start of egglet

    return hash;
}

// This function calculates the hash of an egglet
// Assumes file pointer is at start of egglet
uint8_t calculate_hash(FILE *stream) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    uint64_t egglet_size = get_egglet_size(stream);

    uint8_t hash = 0; // Hash starts as zero
    uint8_t byte = 0;

    for (int i = 1; i < egglet_size; i++) {
        byte = fgetc(stream); // Read byte from egglet

        // Check for incorrect magic number
        if (i == 1) {
            if (byte != EGGLET_MAGIC) {
                fprintf(stderr, "error: incorrect first egglet byte: %#x should be 0x63\n", byte);
                exit(EXIT_FAILURE);
            } 
        }

        hash = egglet_hash(hash, byte); // Hash byte that was read from egglet
    }

    goto_pos(stream, start_pos, SEEK_SET); // Reset position to start of egglet

    return hash;
}

// This function calculates hash of egglet and checks it with already existing hash
// Assumes file pointer is at start of egglet
int check_egglet_hash(FILE *stream) {
    uint8_t saved_hash = get_egglet_hash(stream); // Retrieve already existing hash
    uint8_t hash = calculate_hash(stream); // Calculate new hash

    // Comapre checksums, if equal then data is unmodified, otherwise data is corrupt
    if (hash == saved_hash) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// This functions convert mode (string form) into mode (octal form)
// Assumes mode string is 10 characters long and has correct format
int convert_mode_to_octal(char *mode) {
    int octal = 0; // Initially zero mode, accumulatively determines octal mode

    for (int i = 0; i < EGG_LENGTH_MODE; i++) {
        if (mode[i] == 'd') octal += 01000; // Sets if directory

        if (mode[i] == 'r') {
            if (i >= 1 && i <= 3) octal += 0400; // User read permissions
            if (i >= 4 && i <= 6) octal += 040; // Group read permissions
            if (i >= 7 && i <= 9) octal += 04; // Other read permissions
        }

        if (mode[i] == 'w') {
            if (i >= 1 && i <= 3) octal += 0200; // User write permissions
            if (i >= 4 && i <= 6) octal += 020; // Group write permissions
            if (i >= 7 && i <= 9) octal += 02; // Other write permissions
        }

        if (mode[i] == 'x') {
            if (i >= 1 && i <= 3) octal += 0100; // User execute permissions
            if (i >= 4 && i <= 6) octal += 010; // Group execute permissions
            if (i >= 7 && i <= 9) octal += 01; // Other execute permissions
        }
    }

    return octal;
}

// This function converts mode (octal form) into mode (string form)
// Assumes octal model has correct format
// NOTE: Needs to be free'd after use
char *convert_octal_to_mode(int octal) {
    char *mode = malloc(EGG_LENGTH_MODE + 1); // 10 bytes for mode, +1 for null-terminator

    if (__S_IFDIR & octal) mode[0] = 'd'; else mode[0] = '-'; // Set if directory

    if (S_IRUSR & octal) mode[1] = 'r'; else mode[1] = '-'; // User read permissions
    if (S_IWUSR & octal) mode[2] = 'w'; else mode[2] = '-'; // User write permissions
    if (S_IXUSR & octal) mode[3] = 'x'; else mode[3] = '-'; // User execute permissions

    if (S_IRGRP & octal) mode[4] = 'r'; else mode[4] = '-'; // Group read permissions
    if (S_IWGRP & octal) mode[5] = 'w'; else mode[5] = '-'; // Group write permissions
    if (S_IXGRP & octal) mode[6] = 'x'; else mode[6] = '-'; // Group execute permissions

    if (S_IROTH & octal) mode[7] = 'r'; else mode[7] = '-'; // Other read permissions
    if (S_IWOTH & octal) mode[8] = 'w'; else mode[8] = '-'; // Other write permissions
    if (S_IXOTH & octal) mode[9] = 'x'; else mode[9] = '-'; // Other execute permissions

    mode[10] = '\0'; // Set null-terminator

    return mode;
}

// This function writes egglet content to a file
// Assumes file pointer is at start of egglet
void write_content_to_file(FILE *stream, FILE *output) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    uint16_t pathlen = get_egglet_pathlen(stream);
    uint64_t contentlen = get_egglet_contentlen(stream);

    // Go to content section of egglet
    goto_pos(stream, 20 + pathlen, SEEK_CUR);

    uint8_t byte;
    for (int i = 0; i < contentlen; i++) {
        byte = fgetc(stream); // Read byte from input
        fputc(byte, output); // Write byte to output
    }

    goto_pos(stream, start_pos, SEEK_SET); // Rest position to start of egglet
    goto_pos(output, 0, SEEK_SET); // Reset position to beginning of output file
}

// This function extracts a single egglet from a file
// Assumes file pointer is at start of egglet
void extract_egglet_to_file(FILE *stream, char *pathname) {
    long start_pos = ftell(stream); // Save position at start of egglet
    if (start_pos == -1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    char *mode = get_egglet_mode(stream); 
    
    // Checks if directory bit is set
    int is_directory = FALSE;
    if (mode[0] == 'd') {
        is_directory = TRUE;
    }

    // If directory bit is set, use mkdir() to create directory with correct mode
    // If directory bit is *not* set, use fopen() to create file and chmod() to set correct mode
    if (is_directory) {
        printf("Creating directory: %s\n", pathname);

        // Creates directory with mode
        if (mkdir(pathname, convert_mode_to_octal(mode)) != 0) {
            perror("error");
            exit(EXIT_FAILURE);
        } 
    } else {
        FILE *output = fopen(pathname, "w+b");
        if (output == NULL) {
            perror("error");
            exit(EXIT_FAILURE);
        }

        printf("Extracting: %s\n", pathname);
        write_content_to_file(stream, output); // Writes file

        // Sets mode
        if (chmod(pathname, convert_mode_to_octal(mode)) != 0) {
            perror("error");
            exit(EXIT_FAILURE);
        }

        fclose(output);
    }

    free(mode);

    goto_pos(stream, start_pos, SEEK_SET); // Reset position to start of egglet
}

// This function creates a memory buffer to hold egglet before writing
// Assumes file pointer is at start of egglet
// NOTE: Needs to be free'd after use
char *create_egglet_buffer(char *pathname, int format) {
    struct stat file_status; // Retrieves file status from file at 'pathname'
    if (stat(pathname, &file_status) != 0) { 
        // Error if 'pathname' not found, or otherwise
        fprintf(stderr, "error: could not retrieve file status for %s\n", pathname);
        exit(EXIT_FAILURE);
    }

    char *mode = convert_octal_to_mode(file_status.st_mode); // Retrieve string mode from stat() octal mode
    uint16_t pathlen = (uint16_t)strlen(pathname);

    uint64_t contentlen = 0; // By default, if directory, contentlen is nil
    if (!(__S_IFDIR & file_status.st_mode)) {
        // If not a directory, contentlen is set to file size
        contentlen = (uint64_t)file_status.st_size;
    }

    // Allocates space for an egglet
    // 21 is space needed for metadata
    char *buffer = malloc(21 + pathlen + contentlen);

    // Writes magic and format bytes to buffer
    buffer[0] = EGGLET_MAGIC;
    buffer[1] = format;
    
    // Writes 10 mode bytes to buffer
    for (int i = 2; i < 2 + EGG_LENGTH_MODE; i++) {
        buffer[i] = mode[i - 2];
    }

    // Writes 16-bit pathname length to buffer in little-endian form
    // 0xFF is mask for least sig. byte, 0xFF00 is mask for most sig. byte
    buffer[12] = (pathlen & 0xFF);
    buffer[13] = (pathlen & 0xFF00) >> 8;

    // Writes pathname to buffer
    for (int i = 14; i < 14 + pathlen; i++) {
        buffer[i] = pathname[i - 14];
    }

    // Writes 48-bit content length to buffer in little-endian form
    // 0xFF is mask for least sig. byte, 0xFF0000000000 is mask for most sig. byte
    buffer[14 + pathlen] = (contentlen & 0xFF); 
    buffer[14 + pathlen + 1] = (contentlen & 0xFF00) >> 8; 
    buffer[14 + pathlen + 2] = (contentlen & 0xFF0000) >> 16;
    buffer[14 + pathlen + 3] = (contentlen & 0xFF000000) >> 24;
    buffer[14 + pathlen + 4] = (contentlen & 0xFF00000000) >> 32; 
    buffer[14 + pathlen + 5] = (contentlen & 0xFF0000000000) >> 40; 

    FILE *stream = fopen(pathname, "rb");
    if (stream == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    // Skip writing content to buffer if directory
    if (!(__S_IFDIR & file_status.st_mode)) {
        for (int i = 14 + pathlen + 6; i < 14 + pathlen + 6 + contentlen; i++) {
            buffer[i] = fgetc(stream);
        }
    }

    uint8_t hash = 0; // Hash starts as zero
    for (int i = 0; i < 14 + pathlen + 6 + contentlen; i++) {
        hash = egglet_hash(hash, buffer[i]); // Hash calculated based off previous hash
    }
    buffer[14 + pathlen + 6 + contentlen] = hash; // Writes hash byte to buffer

    free(mode);
    fclose(stream);

    return buffer;
}

// This function writes a single egglet to a file
// Assumes file pointer is at start of egglet
void write_egglet_to_file(FILE *stream, char *pathname, char *buffer) {
    struct stat file_status; // Retrieves file status from pathname
    if (stat(pathname, &file_status) != 0) {
        // Throws error if 'pathname' does not exist, or otherwise
        fprintf(stderr, "error: could not retrieve file status for %s\n", pathname);
        exit(EXIT_FAILURE);
    }

    uint16_t pathlen = (uint16_t)strlen(pathname);
    uint64_t contentlen = 0; // Sets content length to zero, if directory
    if (!(__S_IFDIR & file_status.st_mode)) {
        // If not directory, sets content length to non-zero size of file
        contentlen = (uint64_t)file_status.st_size;
    }

    // Write all egglet buffer bytes to output file
    for (int i = 0; i < 21 + pathlen + contentlen; i++) {
        fputc(buffer[i], stream);
    }
}

// This function both creates an egglet buffer *and* writes buffer to file
// Used as recursive function for traversing directories
void add_egglet_to_file(FILE *stream, char *pathname, int format) {
    DIR *dir; // Directory stream
    struct dirent *entry; // Directory information

    // Allocates buffer, writes it to file, then free's it
    char *buffer = create_egglet_buffer(pathname, format);
    printf("Adding: %s\n", pathname);
    write_egglet_to_file(stream, pathname, buffer);
    free(buffer);

    // Checks if opendir() works, and stream pointer to 'dir'
    if ((dir = opendir(pathname)) != NULL) {
        // If opendir() does work, and is thus a directory, loops through all files/directories in directory
        while ((entry = readdir(dir)) != NULL) { // Loops until readdir() fails, meaning no more items in directory
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

            // Modifies pathname to recursively allocate, write and free directory items
            char new_pathname[4096];
            strcpy(new_pathname, pathname);
            strcat(new_pathname, "/");
            strcat(new_pathname, entry->d_name);
            
            add_egglet_to_file(stream, new_pathname, format); // Recursive call
        }
    }
}


//
// Subset 0
//
void list_egg(char *egg_pathname, int long_listing) {
    // Opens egg file to list
    FILE *stream = fopen(egg_pathname, "rb");
    if (stream == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    do {
        // Retrieve pathname of current egglet
        char *pathname = get_egglet_pathname(stream);

        // Adds extra printing variables with extra formatting for 'long_listing' option
        if (long_listing) {
            char *mode = get_egglet_mode(stream);
            uint8_t format = get_egglet_format(stream);
            uint64_t contentlen = get_egglet_contentlen(stream);

            // Converst hexadecimal format to decimal format so it is more readable
            if (format == 0x36) format = 6;
            if (format == 0x37) format = 7;
            if (format == 0x38) format = 8;
            
            // Advanced printf statement
            printf("%s  %d  %5lu  %s\n", mode, format, contentlen, pathname);

            free(mode);
        } else {
            // Simple printf statement
            printf("%s\n", pathname);
        }

        free(pathname);
    } while (egglet_next(stream)); // egglet_next() moves file pointer to next egglet

    fclose(stream);
}
// End of subset 0


//
// Subset 1
//
void check_egg(char *egg_pathname) {
    // Opens egg file for checksumming
    FILE *stream = fopen(egg_pathname, "rb");
    if (stream == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    do {
        // Retrieve pathname of current egglet
        char *pathname = get_egglet_pathname(stream);

        if (check_egglet_hash(stream)) {
            printf("%s - correct hash\n", pathname);
        } else {
            // If hash incorrect, print error with values of differing hashes
            uint8_t calculated_hash = calculate_hash(stream);
            uint8_t saved_hash = get_egglet_hash(stream);
            printf("%s - incorrect hash %#x should be %#x\n", 
                pathname, calculated_hash, saved_hash);
        }

        free(pathname);
    } while (egglet_next(stream)); // egglet_next() moves file pointer to next egglet

    fclose(stream);
}

void extract_egg(char *egg_pathname) {
    // Open egg file for extracting
    FILE *stream = fopen(egg_pathname, "rb");
    if (stream == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    } 

    do {
        // Retrieve pathname of current egglet
        char *pathname = get_egglet_pathname(stream);

        extract_egglet_to_file(stream, pathname);

        free(pathname);
    } while (egglet_next(stream)); // egglet_next() moves file pointer to next egglet

    fclose(stream);
}
// End of subset 1


//
// Subset 2
//
void create_egg(char *egg_pathname, int append, int format,
                int n_pathnames, char *pathnames[n_pathnames]) {
    // Open egg file for creating
    FILE *stream = fopen(egg_pathname, ((append) ? "a+b" : "w+b"));
    if (stream == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    // Cycle through all n_pathnames pathnames stored in array pathnames[]
    for (int i = 0; i < n_pathnames; i++) {
        char *pathname = pathnames[i];
        add_egglet_to_file(stream, pathname, format); // Buffer and write egglet to egg file
    }

    fclose(stream);
}
// End of subset 2
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>

#include "chicken.h"

#define TRUE 1
#define FALSE 0

void test_reset_color() {
    printf("\033[0m");
}

// Functions to test
long test_get_file_length(FILE *stream) {
    long curr_pos = ftell(stream);
    long file_length;

    fseek(stream, 0, SEEK_END);
    file_length = ftell(stream);
    fseek(stream, curr_pos, SEEK_SET);

    return file_length;
}

int test_goto_pos(FILE *stream, long pos, int origin) {
    int error = fseek(stream, pos, origin);
    
    long curr_pos = ftell(stream);
    long file_length = test_get_file_length(stream);

    if (curr_pos >= file_length) return -1;
    if (ferror(stream) != 0) return -2;
    if (error != 0) return -3;

    return 0;
}

// Assumes current position is start of egglet
uint16_t test_get_egglet_pathlen(FILE *stream) {
    long start_pos = ftell(stream);

    test_goto_pos(stream, EGG_OFFSET_PATHNLEN, SEEK_CUR);

    uint16_t byte1 = fgetc(stream);
    uint16_t byte2 = fgetc(stream);

    uint16_t pathlen = byte1 | (byte2 << 8);

    test_goto_pos(stream, start_pos, SEEK_SET);

    return pathlen;
}

// Assumes current position is start of egglet
uint64_t test_get_egglet_contentlen(FILE *stream) {
    long start_pos = ftell(stream);
    uint16_t pathlen = test_get_egglet_pathlen(stream);

    test_goto_pos(stream, EGG_OFFSET_PATHNLEN + EGG_LENGTH_PATHNLEN + pathlen, SEEK_CUR);

    uint64_t byte1 = fgetc(stream);
    uint64_t byte2 = fgetc(stream);
    uint64_t byte3 = fgetc(stream);
    uint64_t byte4 = fgetc(stream);
    uint64_t byte5 = fgetc(stream);
    uint64_t byte6 = fgetc(stream);
    
    uint64_t contentlen = byte1 | (byte2 << 8) | (byte3 << 16) | 
                (byte4 << 24) | (byte5 << 32) | (byte6 << 40);
    
    test_goto_pos(stream, start_pos, SEEK_SET);

    return contentlen;
}

// Assumes current position is start of egglet
uint64_t test_get_egglet_size(FILE *stream) {
    uint16_t pathlen = test_get_egglet_pathlen(stream);
    uint64_t contentlen = test_get_egglet_contentlen(stream);

    uint64_t eggletlen = 21 + pathlen + contentlen;

    return eggletlen;
}

// Assumes current position is start of egglet
// NOTE: Needs to be free'd after use
char *test_get_egglet_pathname(FILE *stream) {
    long start_pos = ftell(stream);

    uint16_t pathlen = test_get_egglet_pathlen(stream);
    char *pathname = malloc(pathlen + 1);

    test_goto_pos(stream, EGG_OFFSET_PATHNLEN + EGG_LENGTH_PATHNLEN, SEEK_CUR);

    fread(pathname, 1, pathlen, stream);
    pathname[pathlen] = '\0';

    test_goto_pos(stream, start_pos, SEEK_SET);

    return pathname;
}

// Assumes current position is start of egglet
// NOTE: Needs to be free'd after use
char *test_get_egglet_mode(FILE *stream) {
    long start_pos = ftell(stream);

    char *mode = malloc(EGG_LENGTH_MODE + 1); 

    test_goto_pos(stream, EGG_OFFSET_MODE, SEEK_CUR);

    for (int i = 0; i < EGG_LENGTH_MODE; i++) {
        mode[i] = fgetc(stream);
    }
    mode[EGG_LENGTH_MODE] = '\0';

    test_goto_pos(stream, start_pos, SEEK_SET);

    return mode;
}

// Assumes current position is start of egglet
uint8_t test_get_egglet_format(FILE *stream) {
    long start_pos = ftell(stream);

    test_goto_pos(stream, EGG_OFFSET_FORMAT, SEEK_CUR);

    uint8_t format = fgetc(stream);

    test_goto_pos(stream, start_pos, SEEK_SET);

    return format;
}

// Assumes current position is start of egglet
uint8_t test_get_egglet_hash(FILE *stream) {
    long start_pos = ftell(stream);

    uint16_t pathlen = test_get_egglet_pathlen(stream);
    uint64_t contentlen = test_get_egglet_contentlen(stream);

    test_goto_pos(stream, 20 + pathlen + contentlen, SEEK_CUR);

    uint8_t hash = fgetc(stream);

    test_goto_pos(stream, start_pos, SEEK_SET);

    return hash;
}

// Assumes current position is start of egglet
uint8_t test_calculate_hash(FILE *stream) {
    long start_pos = ftell(stream);
    uint64_t egglet_size = test_get_egglet_size(stream);

    uint8_t hash = 0;
    uint8_t byte = 0;

    for (int i = 1; i < egglet_size; i++) {
        byte = fgetc(stream);

        if (i == 1) {
            if (byte != EGGLET_MAGIC) {
                fprintf(stderr, "incorrect first egglet byte: %#x instead of 0x63\n", byte);
                exit(EXIT_FAILURE);
            } 
        }

        hash = egglet_hash(hash, byte);
    }

    test_goto_pos(stream, start_pos, SEEK_SET);

    return hash;
}

// Assumes current position is start of egglet
int test_check_egglet_hash(FILE *stream) {
    // long start_pos = ftell(stream);
    // uint64_t egglet_size = get_egglet_size(stream);
    uint8_t saved_hash = test_get_egglet_hash(stream);

    // uint8_t hash = 0;
    // uint8_t byte = 0;
    
    // for (int i = 1; i < egglet_size; i++) {
    //     byte = fgetc(stream);

    //     if (i == 1) {
    //         if (byte != EGGLET_MAGIC) {
    //             print_error("incorrect first egglet byte: %#x instead of 0x63", byte);
    //             exit(EXIT_FAILURE);
    //         } 
    //     }

    //     hash = egglet_hash(hash, byte);
    // }

    // goto_pos(stream, start_pos, SEEK_SET);
    uint8_t hash = test_calculate_hash(stream);

    if (hash == saved_hash) {
        return TRUE;
    } else {
        return FALSE;
    }
}

// Assumes mode string is 10 characters long and has correct format
int test_convert_mode_to_octal(char *mode) {
    int octal = 0;

    for (int i = 0; i < EGG_LENGTH_MODE; i++) {
        if (mode[i] == 'd') octal += 01000;

        if (mode[i] == 'r') {
            if (i >= 1 && i <= 3) octal += 0400;
            if (i >= 4 && i <= 6) octal += 040;
            if (i >= 7 && i <= 9) octal += 04;
        }

        if (mode[i] == 'w') {
            if (i >= 1 && i <= 3) octal += 0200;
            if (i >= 4 && i <= 6) octal += 020;
            if (i >= 7 && i <= 9) octal += 02;
        }

        if (mode[i] == 'x') {
            if (i >= 1 && i <= 3) octal += 0100;
            if (i >= 4 && i <= 6) octal += 010;
            if (i >= 7 && i <= 9) octal += 01;
        }
    }

    return octal;
}

// Assumes current position is start of egglet
void test_write_content_to_file(FILE *stream, FILE *output) {
    long start_pos = ftell(stream);

    uint16_t pathlen = test_get_egglet_pathlen(stream);
    uint64_t contentlen = test_get_egglet_contentlen(stream);

    test_goto_pos(stream, 20 + pathlen, SEEK_CUR);

    uint8_t byte;
    for (int i = 0; i < contentlen; i++) {
        byte = fgetc(stream);
        fputc(byte, output);
    }

    test_goto_pos(stream, start_pos, SEEK_SET);
    test_goto_pos(output, 0, SEEK_SET);
}

// Assumes current position is start of egglet
void test_extract_egglet_to_file(FILE *stream) {
    long start_pos = ftell(stream);

    char *pathname = test_get_egglet_pathname(stream);
    char *mode = test_get_egglet_mode(stream);

    FILE *output = fopen(pathname, "w+b");

    test_write_content_to_file(stream, output);
    chmod(pathname, test_convert_mode_to_octal(mode));

    free(pathname);
    free(mode);
    fclose(output);

    test_goto_pos(stream, start_pos, SEEK_SET);
}

// Assumes octal is in correct format
// NOTE: Needs to be free'd after use
char *test_convert_octal_to_mode(int octal) {
    char *mode = malloc(EGG_LENGTH_MODE + 1);

    if (__S_IFDIR & octal) mode[0] = 'd'; else mode[0] = '-';

    if (S_IRUSR & octal) mode[1] = 'r'; else mode[1] = '-';
    if (S_IWUSR & octal) mode[2] = 'w'; else mode[2] = '-';
    if (S_IXUSR & octal) mode[3] = 'x'; else mode[3] = '-';

    if (S_IRGRP & octal) mode[4] = 'r'; else mode[4] = '-';
    if (S_IWGRP & octal) mode[5] = 'w'; else mode[5] = '-';
    if (S_IXGRP & octal) mode[6] = 'x'; else mode[6] = '-';

    if (S_IROTH & octal) mode[7] = 'r'; else mode[7] = '-';
    if (S_IWOTH & octal) mode[8] = 'w'; else mode[8] = '-';
    if (S_IXOTH & octal) mode[9] = 'x'; else mode[9] = '-';

    mode[10] = '\0';

    return mode;
}

// NOTE: Needs to be free'd after use
char *test_create_egglet_buffer(char *pathname, int format) {
    struct stat file_status;
    if (stat(pathname, &file_status) != 0) {
        fprintf(stderr, "error: could not retrieve file status for %s\n", pathname);
        exit(EXIT_FAILURE);
    }

    char *mode = test_convert_octal_to_mode(file_status.st_mode);
    uint16_t pathlen = (uint16_t)strlen(pathname);
    uint64_t contentlen = (uint64_t)file_status.st_size;

    char *buffer = malloc(21 + pathlen + contentlen);

    buffer[0] = EGGLET_MAGIC;
    buffer[1] = format;
    
    for (int i = 2; i < 2 + EGG_LENGTH_MODE; i++) {
        buffer[i] = mode[i - 2];
    }

    buffer[12] = (pathlen & 0xFF);
    buffer[13] = (pathlen & 0xFF00);

    for (int i = 14; i < 14 + pathlen; i++) {
        buffer[i] = pathname[i - 14];
    }

    buffer[14 + pathlen] = (contentlen & 0xFF);
    buffer[14 + pathlen + 1] = (contentlen & 0xFF00);
    buffer[14 + pathlen + 2] = (contentlen & 0xFF0000);
    buffer[14 + pathlen + 3] = (contentlen & 0xFF000000);
    buffer[14 + pathlen + 4] = (contentlen & 0xFF00000000);
    buffer[14 + pathlen + 5] = (contentlen & 0xFF0000000000);

    FILE *stream = fopen(pathname, "rb");

    for (int i = 14 + pathlen + 6; i < 14 + pathlen + 6 + contentlen; i++) {
        buffer[i] = fgetc(stream);
    }

    uint8_t hash = 0;
    for (int i = 0; i < 14 + pathlen + 6 + contentlen; i++) {
        hash = egglet_hash(hash, buffer[i]);
    }
    buffer[14 + pathlen + 6 + contentlen] = hash;

    free(mode);
    fclose(stream);

    return buffer;
}





//
// Test suites
//
void set_color_green() {
    printf("\033[0;32m");
}

void TEST(int n) {
    printf("TEST %d: ", n);
}

void PASSED() {
    set_color_green();
    printf("Passed!\n");
    test_reset_color();
}

void TEST_SUITE_GOTO_POS() {
    FILE *stream = fopen("examples/exampletext.txt", "r+b");
    char ch;
    int error;

    printf("GOTO_POS\n");

    error = test_goto_pos(stream, 5, SEEK_SET);
    ch = fgetc(stream);

    TEST(1);
    printf("(%c) ", ch);
    assert(ch == 'e');
    assert(error == 0);
    PASSED();

    error = test_goto_pos(stream, 1000, SEEK_SET);

    TEST(2);
    assert(error == -1);
    PASSED();

    error = test_goto_pos(stream, -1, SEEK_SET);

    TEST(3);
    assert(error == -1);
    PASSED();

    fclose(stream);

    printf("\n");
}

void TEST_SUITE_GET_EGGLET_PATHLEN() {
    FILE *stream1 = fopen("examples/1_file.subdirectory.egg", "rb");
    FILE *stream2 = fopen("examples/2_files.egg", "rb");

    printf("GET_EGGLET_PATHLEN\n");

    uint16_t pathlen = test_get_egglet_pathlen(stream1);
    
    TEST(1);
    printf("(%d) ", pathlen);
    assert(pathlen == 5);
    PASSED();

    long pos_before = ftell(stream2);
    pathlen = test_get_egglet_pathlen(stream2);
    long pos_after = ftell(stream2);
    
    TEST(2);
    printf("(%d) ", pathlen);
    assert(pathlen == 9);
    PASSED();

    TEST(3);
    assert(pos_before == pos_after);
    PASSED();

    fclose(stream1);
    fclose(stream2);

    printf("\n");
}

void TEST_SUITE_GET_EGGLET_CONTENTLEN() {
    FILE *stream1 = fopen("examples/1_file.subdirectory.egg", "rb");
    FILE *stream2 = fopen("examples/2_files.egg", "rb");

    printf("GET_EGGLET_CONTENTLEN\n");

    uint64_t contentlen = test_get_egglet_contentlen(stream1);

    TEST(1);
    printf("(%lu) ", contentlen);
    assert(contentlen == 0);
    PASSED();

    long pos_before = ftell(stream2);
    contentlen = test_get_egglet_contentlen(stream2);
    long pos_after = ftell(stream2);

    TEST(2);
    printf("(%lu) ", contentlen);
    assert(contentlen == 56);
    PASSED();

    TEST(3);
    assert(pos_before == pos_after);
    PASSED();

    fclose(stream1);
    fclose(stream2);

    printf("\n");
}

void TEST_SUITE_GET_EGGLET_PATHNAME() {
    FILE *stream1 = fopen("examples/1_file.subdirectory.egg", "rb");

    printf("GET_EGGLET_PATHNAME\n");

    long pos_before = ftell(stream1);
    char *pathname = test_get_egglet_pathname(stream1);
    long pos_after = ftell(stream1);

    TEST(1);
    printf("(%s) ", pathname);
    assert(strcmp(pathname, "hello") == 0);
    PASSED();

    TEST(2);
    assert(pos_before == pos_after);
    PASSED();

    free(pathname);
    fclose(stream1);

    printf("\n");
}

void TEST_SUITE_GET_EGGLET_MODE() {
    FILE *stream1 = fopen("examples/1_file.subdirectory.egg", "rb");

    printf("GET_EGGLET_MODE\n");

    long pos_before = ftell(stream1);
    char *mode = test_get_egglet_mode(stream1);
    long pos_after = ftell(stream1);

    TEST(1);
    printf("(%s) ", mode);
    assert(strcmp(mode, "drwxr-xr-x") == 0);
    PASSED();

    TEST(2);
    assert(pos_before == pos_after);
    PASSED();

    free(mode);
    fclose(stream1);

    printf("\n");
}

void TEST_SUITE_GET_EGGLET_FORMAT() {
    FILE *stream1 = fopen("examples/1_file.subdirectory.egg", "rb");

    printf("GET_EGGLET_FORMAT\n");

    long pos_before = ftell(stream1);
    uint8_t format = test_get_egglet_format(stream1);
    long pos_after = ftell(stream1);

    TEST(1); 
    printf("(%d) ", format);
    assert(format == 0x38);
    PASSED();

    TEST(2);
    assert(pos_before == pos_after);
    PASSED();

    fclose(stream1);

    printf("\n");
}

void TEST_SUITE_GET_EGGLET_HASH() {
    FILE *stream1 = fopen("examples/1_file.subdirectory.egg", "rb");

    printf("GET_EGGLET_HASH\n");

    long pos_before = ftell(stream1);
    uint8_t hash = test_get_egglet_hash(stream1);
    long pos_after = ftell(stream1);

    TEST(1);
    printf("(%d) ", hash);
    assert(hash == 37);
    PASSED();

    TEST(2);
    assert(pos_before == pos_after);
    PASSED();

    fclose(stream1);

    printf("\n");
}

void TEST_SUITE_CHECK_EGGLET_HASH() {
    FILE *stream1 = fopen("examples/1_file.subdirectory.egg", "rb");
    FILE *stream2 = fopen("examples/hello_world.bad_hash.egg", "rb");
    FILE *stream3 = fopen("examples/hello_world.bad_magic.egg", "rb");

    printf("CHECK_EGGLET_HASH\n");

    long pos_before = ftell(stream1);
    int is_hash_correct = test_check_egglet_hash(stream1);
    long pos_after = ftell(stream1);

    TEST(1);
    printf("(%d) ", is_hash_correct);
    assert(is_hash_correct);
    PASSED();

    TEST(2);
    assert(pos_before == pos_after);
    PASSED();

    is_hash_correct = test_check_egglet_hash(stream2);

    TEST(3);
    printf("(%d) ", is_hash_correct);
    assert(is_hash_correct);
    PASSED();

    //TEST(4);
    //is_hash_correct = test_check_egglet_hash(stream3);

    fclose(stream1);
    fclose(stream2);
    fclose(stream3);

    printf("\n");
}

void TEST_SUITE_CONVERT_MODE_TO_OCTAL() {
    FILE *stream1 = fopen("examples/1_file.subdirectory.egg", "rb");
    FILE *stream2 = fopen("examples/hello_world.egg", "rb");

    char *mode = test_get_egglet_mode(stream1);
    int octal = test_convert_mode_to_octal(mode);

    printf("CONVERT_MODE_TO_OCTAL\n");

    TEST(1);
    printf("(%o) ", octal);
    assert(octal == 01755);
    PASSED();

    free(mode);

    mode = test_get_egglet_mode(stream2);
    octal = test_convert_mode_to_octal(mode);

    TEST(2);
    printf("(%o) ", octal);
    assert(octal == 0644);
    PASSED();

    fclose(stream1);
    fclose(stream2);

    printf("\n");
}

void TEST_SUITE_WRITE_CONTENT_TO_FILE() {
    FILE *stream1 = fopen("examples/hello_world.egg", "rb");
    FILE *output1 = fopen("testing/output.txt", "w+b");

    test_write_content_to_file(stream1, output1);

    fclose(stream1);
    fclose(output1);
}

void TEST_SUITE_EXTRACT_EGGLET_TO_FILE() {
    FILE *stream1 = fopen("examples/hello_world.egg", "rb");
    chdir("testing/");

    test_extract_egglet_to_file(stream1);

    chdir("../");
    fclose(stream1);
}

void TEST_SUITE_CONVERT_OCTAL_TO_MODE() {
    struct stat file_status;
    if (stat("examples/test.txt", &file_status) != 0) {
        fprintf(stderr, "error: could not retrieve file status\n");
        exit(EXIT_FAILURE);
    }

    char *mode = test_convert_octal_to_mode(file_status.st_mode);

    printf("CONVERT_OCTAL_TO_MODE\n");

    TEST(1);
    printf("(%s) ", mode);
    assert(strcmp(mode, "-rw-r--r--") == 0);
    PASSED();

    free(mode);

    printf("\n");
}

void TEST_SUITE_CREATE_EGGLET_BUFFER() {
    chdir("testing/");

    // FIRST TEST
    char *buffer = test_create_egglet_buffer("output.txt", EGGLET_FMT_8);

    struct stat file_status;
    if (stat("output.txt", &file_status) != 0) {
        fprintf(stderr, "error: could not retrieve file status for %s\n", "output.txt");
        exit(EXIT_FAILURE);
    }

    uint16_t pathlen = (uint16_t)strlen("output.txt");
    uint64_t contentlen = (uint64_t)file_status.st_size;

    printf("Buffer start\n\n");

    for (int i = 0; i < 21 + pathlen + contentlen; i++) {
        putchar(buffer[i]);
    }

    printf("\nBuffer end\n");

    FILE *stream = fopen("output", "w+b");

    for (int i = 0; i < 21 + pathlen + contentlen; i++) {
        fputc(buffer[i], stream);
    }

    fclose(stream);
    free(buffer);

    // SECOND TEST
    buffer = test_create_egglet_buffer("256.bin", EGGLET_FMT_8);

    if (stat("256.bin", &file_status) != 0) {
        fprintf(stderr, "error: could not retrieve file status for %s\n", "256.bin");
        exit(EXIT_FAILURE);
    }

    pathlen = (uint16_t)strlen("256.bin");
    contentlen = (uint64_t)file_status.st_size; printf("256.bin file size: %lu\n", contentlen);

    stream = fopen("output2", "w+b");

    for (int i = 0; i < 21 + pathlen + contentlen; i++) {
        fputc(buffer[i], stream);
    }

    fclose(stream);
    free(buffer);

    chdir("../");

    printf("\n");
}

int main(int argc, char **argv) {
    TEST_SUITE_GOTO_POS();
    TEST_SUITE_GET_EGGLET_PATHLEN();
    TEST_SUITE_GET_EGGLET_CONTENTLEN();
    TEST_SUITE_GET_EGGLET_PATHNAME();
    TEST_SUITE_GET_EGGLET_MODE();
    TEST_SUITE_GET_EGGLET_FORMAT();
    TEST_SUITE_GET_EGGLET_HASH();
    TEST_SUITE_CHECK_EGGLET_HASH();
    TEST_SUITE_CONVERT_MODE_TO_OCTAL();
    //TEST_SUITE_WRITE_CONTENT_TO_FILE();
    //TEST_SUITE_EXTRACT_EGGLET_TO_FILE();
    TEST_SUITE_CONVERT_OCTAL_TO_MODE();
    TEST_SUITE_CREATE_EGGLET_BUFFER();

    printf("All tests passed!\n");

    return 0;
}
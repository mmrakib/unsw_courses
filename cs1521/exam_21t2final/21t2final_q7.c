// COMP1521 21T2 ... final exam, question 7

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 4096

const mode_t NEW_DIR_MODE = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;

void
cp_directory (char *dir_from, char *dir_to)
{
	mkdir(dir_to, NEW_DIR_MODE);

	DIR *input = opendir(dir_from);
	DIR *output = opendir(dir_to);

	struct dirent *entry;
	while ((entry = readdir(input)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

		if (!(entry->d_type == DT_DIR)) {
			char in_file_name[MAX_LEN];
			char out_file_name[MAX_LEN];

			strcpy(in_file_name, dir_from);
			strcpy(out_file_name, dir_to);

			strcat(in_file_name, "/");
			strcat(out_file_name, "/");

			strcat(in_file_name, entry->d_name);
			strcat(out_file_name, entry->d_name);

			FILE *in_file = fopen(in_file_name, "r");
			FILE *out_file = fopen(out_file_name, "w");

			int byte;
			while ((byte = fgetc(in_file)) != EOF) {
				fputc(byte, out_file);
			}

			fclose(in_file);
			fclose(out_file);
		} else {
			char new_dir_from[MAX_LEN];
			char new_dir_to[MAX_LEN];

			strcpy(new_dir_from, dir_from);
			strcpy(new_dir_to, dir_to);

			strcat(new_dir_from, "/");
			strcat(new_dir_to, "/");

			strcat(new_dir_from, entry->d_name);
			strcat(new_dir_to, entry->d_name);

			cp_directory(new_dir_from, new_dir_to);
		}
	}

	closedir(input);
	closedir(output);
}

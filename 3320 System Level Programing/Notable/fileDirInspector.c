/*
hmwk4.txt (the text version of the log file)
Jash Thakkar
CSc 3320 Homework #4
Account: jthakkar3
Due date: 11/17

Description: shows the files within a file and the relevent info of the files

Input: file name and the output option (or -h for help)

Output: the file type the file path and the info relevent to the file

Usage: to quickly see what is in a file / folder
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>

// Function Prototypes
void print_help(const char *prog_name);
void process_file(const char *path, int short_mode);
void process_directory(const char *dir_name, int short_mode);

// Entry point of the program
int main(int argc, char *argv[]) {
    int short_mode = 0;
    int help_flag = 0;
    char *target_path = NULL;

    // Process command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            help_flag = 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            short_mode = 1;
        } else {
            target_path = argv[i];
        }
    }

    if (help_flag) {
        print_help(argv[0]);
        return 0;
    }

    if (!target_path) {
        target_path = ".";
    }

    struct stat path_stat;
    if (stat(target_path, &path_stat) == -1) {
        fprintf(stderr, "Error: File or directory \"%s\" not found\n", target_path);
        return 1;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        process_directory(target_path, short_mode);
    } else {
        process_file(target_path, short_mode);
    }

    return 0;
}

void print_help(const char *prog_name) {
    printf("Usage: %s [options (optional)] [file|directory]\n", prog_name);
    printf("Options:\n");
    printf("  -h           Show this help message\n");
    printf("  -s           Display short information (file name, type, size)\n");
    printf("If no file or directory is specified, the current directory is listed.\n");
    printf("\nExamples:\n");
    printf("  %s -h\n", prog_name);
    printf("  %s pictures\n", prog_name);
    printf("  %s -s pictures\n", prog_name);
}

void process_file(const char *path, int short_mode) {
    struct stat file_stat;
    if (stat(path, &file_stat) == -1) {
        fprintf(stderr, "Error: Cannot stat \"%s\"\n", path);
        return;
    }

    char *file_type;
    /*this if else if and else test was taken from the example*/
    if (S_ISREG(file_stat.st_mode)) file_type = "regular file";
    else if (S_ISDIR(file_stat.st_mode)) file_type = "directory";
    else file_type = "other";

    if (short_mode) {
        printf("%s, %s, %lld bytes\n", path, file_type, file_stat.st_size);
    } else {
        printf("%s, user %d, group %d, %s, %lld bytes, %s",
               path, file_stat.st_uid, file_stat.st_gid, file_type, file_stat.st_size,
               ctime(&file_stat.st_mtime));
    }
}

void process_directory(const char *dir_name, int short_mode) {
    DIR *dir = opendir(dir_name);
    if (!dir) {
        /*from the example*/
        fprintf(stderr, "Error: Cannot open directory \"%s\"\n", dir_name);
        return;
    }

    struct dirent *entry;
    char full_path[PATH_MAX];

    /*This loop was used from the example*/
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", dir_name, entry->d_name);
        process_file(full_path, short_mode);
    }

    closedir(dir);
}

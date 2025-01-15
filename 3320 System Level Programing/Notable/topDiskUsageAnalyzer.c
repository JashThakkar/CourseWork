/* 
Name: Jash Thakkar

Objective: 
  Program to get top 3 entries from du command in /usr/lib.
  - Uses getcwd to get initial directory.
  - Changes directory to /usr/lib.
  - Uses popen to execute du command.
  - Parses the output to get sizes and filenames.
  - Prints the top 3 entries and calculates the rest usage.
  - Includes a function to find the next space or tab in a string.
  - Tests the function with DEBUG mode.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEBUG 1  // Set to 1 to enable debug mode

char* next_space(const char* ptr) {
    if (ptr == NULL) return NULL;
    if (*ptr == ' ' || *ptr == '\t') {
        return (char*)ptr;
    }
    const char* p = ptr;
    while (*p != '\0' && *p != ' ' && *p != '\t') {
        p++;
    }
    return (char*)p;
}

int main() {
    if (DEBUG) {
        // Debug mode: Test the next_space function with various cases
        const char* test_strings[] = {
            "123 abc",
            "123\tabc",
            "123abc",
            " 123abc"
        };
        int num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
        for (int i = 0; i < num_tests; i++) {
            const char* test_str = test_strings[i];
            printf("Test %d: \"%s\"\n", i + 1, test_str);
            char* result = next_space(test_str);
            printf("Returned pointer points to: \"%s\"\n", result);
        }
        return 0;
    }

    // Main functionality
    FILE* outfile = fopen("output.txt", "w");
    if (outfile == NULL) {
        perror("Failed to open output file");
        return 1;
    }

    // Get current directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        fprintf(outfile, "Initial directory is %s\n", cwd);
    } else {
        perror("getcwd() error");
        fclose(outfile);
        return 1;
    }

    // Change directory to "/usr/lib"
    if (chdir("/usr/lib") != 0) {
        perror("chdir() to /usr/lib failed");
        fclose(outfile);
        return 1;
    }

    // Open pipe to du command
    FILE* pipe = popen("du -s . *", "r");
    if (pipe == NULL) {
        perror("popen() failed");
        fclose(outfile);
        return 1;
    }

    // Read du output
    struct entry {
        int size;
        char name[256];
    } entries[1000];  // Adjust size as needed
    int count = 0;

    char line[1024];
    while (fgets(line, sizeof(line), pipe) != NULL) {
        // Parse size and filename
        char* p = line;
        char* end_of_size = next_space(p);
        if (*end_of_size == '\0') {
            continue;  // Malformed line
        }

        // Copy size string
        char size_str[32];
        strncpy(size_str, p, end_of_size - p);
        size_str[end_of_size - p] = '\0';
        int size = atoi(size_str);

        // Skip spaces/tabs to get to the filename
        p = end_of_size;
        while (*p == ' ' || *p == '\t') {
            p++;
        }

        // Remove trailing newline character
        char* newline = strchr(p, '\n');
        if (newline) {
            *newline = '\0';
        }

        // Store the entry
        entries[count].size = size;
        strncpy(entries[count].name, p, sizeof(entries[count].name) - 1);
        entries[count].name[sizeof(entries[count].name) - 1] = '\0';
        count++;
    }

    pclose(pipe);

    if (count == 0) {
        fprintf(outfile, "No entries found.\n");
        fclose(outfile);
        return 1;
    }

    // Sort entries in decreasing order of size
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (entries[j].size < entries[j + 1].size) {
                struct entry temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }

    // Print top 3 entries (or fewer)
    int num_to_print = count < 3 ? count : 3;
    for (int i = 0; i < num_to_print; i++) {
        fprintf(outfile, "%d %s\n", entries[i].size, entries[i].name);
    }

    // Calculate "The rest use"
    if (num_to_print >= 3) {
        int total_size = entries[0].size;
        int size2 = entries[1].size;
        int size3 = entries[2].size;
        int rest_use = total_size - size2 - size3;
        fprintf(outfile, "The rest use %d\n", rest_use);
    } else {
        fprintf(outfile, "Not enough entries to calculate 'The rest use'.\n");
    }

    fclose(outfile);
    return 0;
}

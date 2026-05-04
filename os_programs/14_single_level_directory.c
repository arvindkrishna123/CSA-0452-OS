/*
 * Program 14: Single Level Directory Organization
 * Simulates a single-level directory structure where all files
 * reside in one directory. Operations: create, delete, search, display.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES    20
#define MAX_FILENAME 50

typedef struct {
    char filename[MAX_FILENAME];
    int  size;       /* in KB */
    char type[10];   /* e.g., "txt", "c", "bin" */
    int  is_used;
} FileEntry;

typedef struct {
    FileEntry files[MAX_FILES];
    int       count;
} Directory;

Directory dir;

void init_directory() {
    dir.count = 0;
    for (int i = 0; i < MAX_FILES; i++)
        dir.files[i].is_used = 0;
}

int find_file(const char *name) {
    for (int i = 0; i < MAX_FILES; i++)
        if (dir.files[i].is_used && strcmp(dir.files[i].filename, name) == 0)
            return i;
    return -1;
}

void create_file(const char *name, int size, const char *type) {
    if (dir.count >= MAX_FILES) {
        printf("  ERROR: Directory is full.\n");
        return;
    }
    if (find_file(name) != -1) {
        printf("  ERROR: File '%s' already exists.\n", name);
        return;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        if (!dir.files[i].is_used) {
            strncpy(dir.files[i].filename, name, MAX_FILENAME - 1);
            dir.files[i].size    = size;
            dir.files[i].is_used = 1;
            strncpy(dir.files[i].type, type, 9);
            dir.count++;
            printf("  File '%s' created successfully.\n", name);
            return;
        }
    }
}

void delete_file(const char *name) {
    int idx = find_file(name);
    if (idx == -1) {
        printf("  ERROR: File '%s' not found.\n", name);
        return;
    }
    dir.files[idx].is_used = 0;
    dir.count--;
    printf("  File '%s' deleted successfully.\n", name);
}

void search_file(const char *name) {
    int idx = find_file(name);
    if (idx == -1) {
        printf("  File '%s' NOT found in directory.\n", name);
    } else {
        printf("  File Found!\n");
        printf("  Name: %-20s | Size: %d KB | Type: %s\n",
               dir.files[idx].filename, dir.files[idx].size, dir.files[idx].type);
    }
}

void display_directory() {
    printf("\n  ========== Single Level Directory ==========\n");
    printf("  %-20s %-10s %-8s\n", "Filename", "Size (KB)", "Type");
    printf("  %s\n", "-------------------------------------------");
    if (dir.count == 0) {
        printf("  (Empty directory)\n");
    } else {
        for (int i = 0; i < MAX_FILES; i++) {
            if (dir.files[i].is_used)
                printf("  %-20s %-10d %-8s\n",
                       dir.files[i].filename,
                       dir.files[i].size,
                       dir.files[i].type);
        }
    }
    printf("  Total files: %d\n", dir.count);
    printf("  ============================================\n\n");
}

int main() {
    int choice, size;
    char name[MAX_FILENAME], type[10];

    init_directory();

    printf("=== Single Level Directory File Organization ===\n\n");

    /* Pre-populate with some demo files */
    printf("[Demo] Pre-creating some files...\n");
    create_file("readme.txt",   5,  "txt");
    create_file("main.c",       12, "c");
    create_file("program.exe",  45, "bin");
    create_file("data.csv",     20, "csv");
    display_directory();

    while (1) {
        printf("Menu:\n");
        printf("  1. Create File\n");
        printf("  2. Delete File\n");
        printf("  3. Search File\n");
        printf("  4. Display Directory\n");
        printf("  5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("  Enter filename: "); scanf("%s", name);
                printf("  Enter size (KB): "); scanf("%d", &size);
                printf("  Enter type (txt/c/bin/etc): "); scanf("%s", type);
                create_file(name, size, type);
                break;
            case 2:
                printf("  Enter filename to delete: "); scanf("%s", name);
                delete_file(name);
                break;
            case 3:
                printf("  Enter filename to search: "); scanf("%s", name);
                search_file(name);
                break;
            case 4:
                display_directory();
                break;
            case 5:
                printf("Exiting.\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
        printf("\n");
    }

    return 0;
}

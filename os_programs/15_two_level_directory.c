/*
 * Program 15: Two Level Directory File Organization
 * Level 1: Master File Directory (MFD) — holds user directories.
 * Level 2: User File Directory  (UFD) — holds files per user.
 *
 * Operations: add user, create file, delete file, search file, display MFD.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS     5
#define MAX_FILES     10
#define MAX_NAME      50

typedef struct {
    char filename[MAX_NAME];
    int  size;
    char type[10];
    int  is_used;
} File;

typedef struct {
    char  username[MAX_NAME];
    File  files[MAX_FILES];
    int   file_count;
    int   is_used;
} UserDirectory;

typedef struct {
    UserDirectory users[MAX_USERS];
    int           user_count;
} MasterDirectory;

MasterDirectory mfd;

void init_mfd() {
    mfd.user_count = 0;
    for (int i = 0; i < MAX_USERS; i++)
        mfd.users[i].is_used = 0;
}

int find_user(const char *uname) {
    for (int i = 0; i < MAX_USERS; i++)
        if (mfd.users[i].is_used && strcmp(mfd.users[i].username, uname) == 0)
            return i;
    return -1;
}

void add_user(const char *uname) {
    if (mfd.user_count >= MAX_USERS) { printf("  ERROR: Max users reached.\n"); return; }
    if (find_user(uname) != -1)      { printf("  ERROR: User '%s' already exists.\n", uname); return; }
    for (int i = 0; i < MAX_USERS; i++) {
        if (!mfd.users[i].is_used) {
            strncpy(mfd.users[i].username, uname, MAX_NAME - 1);
            mfd.users[i].file_count = 0;
            mfd.users[i].is_used    = 1;
            mfd.user_count++;
            printf("  User '%s' directory created.\n", uname);
            return;
        }
    }
}

void create_file(const char *uname, const char *fname, int size, const char *type) {
    int uidx = find_user(uname);
    if (uidx == -1) { printf("  ERROR: User '%s' not found.\n", uname); return; }

    UserDirectory *udir = &mfd.users[uidx];
    if (udir->file_count >= MAX_FILES) { printf("  ERROR: User directory full.\n"); return; }

    for (int i = 0; i < MAX_FILES; i++)
        if (udir->files[i].is_used && strcmp(udir->files[i].filename, fname) == 0) {
            printf("  ERROR: File '%s' already exists for user '%s'.\n", fname, uname); return;
        }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!udir->files[i].is_used) {
            strncpy(udir->files[i].filename, fname, MAX_NAME - 1);
            udir->files[i].size    = size;
            udir->files[i].is_used = 1;
            strncpy(udir->files[i].type, type, 9);
            udir->file_count++;
            printf("  File '%s' created in user '%s'\'s directory.\n", fname, uname);
            return;
        }
    }
}

void delete_file(const char *uname, const char *fname) {
    int uidx = find_user(uname);
    if (uidx == -1) { printf("  ERROR: User '%s' not found.\n", uname); return; }

    UserDirectory *udir = &mfd.users[uidx];
    for (int i = 0; i < MAX_FILES; i++) {
        if (udir->files[i].is_used && strcmp(udir->files[i].filename, fname) == 0) {
            udir->files[i].is_used = 0;
            udir->file_count--;
            printf("  File '%s' deleted from user '%s'\'s directory.\n", fname, uname);
            return;
        }
    }
    printf("  ERROR: File '%s' not found for user '%s'.\n", fname, uname);
}

void search_file(const char *uname, const char *fname) {
    int uidx = find_user(uname);
    if (uidx == -1) { printf("  ERROR: User '%s' not found.\n", uname); return; }

    UserDirectory *udir = &mfd.users[uidx];
    for (int i = 0; i < MAX_FILES; i++) {
        if (udir->files[i].is_used && strcmp(udir->files[i].filename, fname) == 0) {
            printf("  Found: /%s/%s | Size: %d KB | Type: %s\n",
                   uname, fname, udir->files[i].size, udir->files[i].type);
            return;
        }
    }
    printf("  File '%s' NOT found in user '%s'\'s directory.\n", fname, uname);
}

void display_mfd() {
    printf("\n  ====== Master File Directory (MFD) ======\n");
    if (mfd.user_count == 0) { printf("  (No users)\n"); return; }

    for (int i = 0; i < MAX_USERS; i++) {
        if (!mfd.users[i].is_used) continue;
        printf("\n  [User: %s]  (%d file(s))\n", mfd.users[i].username, mfd.users[i].file_count);
        printf("    %-20s %-10s %-8s\n", "Filename", "Size(KB)", "Type");
        printf("    %s\n", "--------------------------------------");
        if (mfd.users[i].file_count == 0) {
            printf("    (No files)\n");
        } else {
            for (int j = 0; j < MAX_FILES; j++) {
                if (mfd.users[i].files[j].is_used)
                    printf("    %-20s %-10d %-8s\n",
                           mfd.users[i].files[j].filename,
                           mfd.users[i].files[j].size,
                           mfd.users[i].files[j].type);
            }
        }
    }
    printf("\n  Total users: %d\n", mfd.user_count);
    printf("  ==========================================\n\n");
}

int main() {
    int  choice, size;
    char uname[MAX_NAME], fname[MAX_NAME], type[10];

    init_mfd();

    printf("=== Two Level Directory File Organization ===\n\n");

    /* Demo pre-population */
    printf("[Demo] Setting up users and files...\n");
    add_user("alice");
    add_user("bob");
    create_file("alice", "notes.txt",  3,  "txt");
    create_file("alice", "lab.c",      8,  "c");
    create_file("bob",   "project.c",  15, "c");
    create_file("bob",   "report.pdf", 50, "pdf");
    display_mfd();

    while (1) {
        printf("Menu:\n");
        printf("  1. Add User\n");
        printf("  2. Create File\n");
        printf("  3. Delete File\n");
        printf("  4. Search File\n");
        printf("  5. Display MFD\n");
        printf("  6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("  Username: "); scanf("%s", uname);
                add_user(uname);
                break;
            case 2:
                printf("  Username: ");  scanf("%s", uname);
                printf("  Filename: ");  scanf("%s", fname);
                printf("  Size (KB): "); scanf("%d", &size);
                printf("  Type: ");      scanf("%s", type);
                create_file(uname, fname, size, type);
                break;
            case 3:
                printf("  Username: "); scanf("%s", uname);
                printf("  Filename: "); scanf("%s", fname);
                delete_file(uname, fname);
                break;
            case 4:
                printf("  Username: "); scanf("%s", uname);
                printf("  Filename: "); scanf("%s", fname);
                search_file(uname, fname);
                break;
            case 5:
                display_mfd();
                break;
            case 6:
                printf("Exiting.\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
        printf("\n");
    }

    return 0;
}

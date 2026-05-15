/*
 * Program 24: UNIX System Calls for File Management
 *
 * Demonstrates the following UNIX file management system calls:
 *   creat()   - Create a new file
 *   open()    - Open a file
 *   close()   - Close a file descriptor
 *   read()    - Read from a file
 *   write()   - Write to a file
 *   lseek()   - Move file offset (seek)
 *   stat()    - Get file metadata/status
 *   rename()  - Rename a file
 *   unlink()  - Delete a file
 *   link()    - Create a hard link
 *   symlink() - Create a symbolic link
 *   access()  - Check file permissions
 *
 * Compile: gcc 24_unix_file_syscalls.c -o 24_unix_file_syscalls
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#define FILE1   "demo_file.txt"
#define FILE2   "demo_copy.txt"
#define HLINK   "demo_hardlink.txt"
#define SLINK   "demo_symlink.txt"

void separator(const char *title) {
    printf("\n--------------------------------------------\n");
    printf(" %s\n", title);
    printf("--------------------------------------------\n");
}

void demo_creat_open_write() {
    separator("1. creat() and write()");

    /* creat() = open() with O_CREAT|O_WRONLY|O_TRUNC */
    int fd = creat(FILE1, 0644);
    if (fd < 0) { perror("creat"); return; }
    printf("creat(\"%s\") -> fd = %d\n", FILE1, fd);

    const char *content =
        "Line 1: Hello from UNIX file system calls!\n"
        "Line 2: OS Lab Program 24 - File Management.\n"
        "Line 3: System calls: open, read, write, lseek, stat.\n"
        "Line 4: End of demo file content.\n";

    ssize_t bytes = write(fd, content, strlen(content));
    printf("write() -> wrote %zd bytes\n", bytes);

    close(fd);
    printf("close() -> file closed (fd=%d)\n", fd);
}

void demo_open_read() {
    separator("2. open() and read()");

    int fd = open(FILE1, O_RDONLY);
    if (fd < 0) { perror("open"); return; }
    printf("open(\"%s\", O_RDONLY) -> fd = %d\n", FILE1, fd);

    char buf[256];
    ssize_t n;
    printf("Contents:\n");
    while ((n = read(fd, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        printf("%s", buf);
    }
    printf("read() -> %zd bytes in last read\n", n);

    close(fd);
    printf("close() -> fd=%d closed\n", fd);
}

void demo_lseek() {
    separator("3. lseek() - File Seek");

    int fd = open(FILE1, O_RDONLY);
    if (fd < 0) { perror("open"); return; }

    /* Seek to end to find file size */
    off_t size = lseek(fd, 0, SEEK_END);
    printf("lseek(fd, 0, SEEK_END) -> file size = %ld bytes\n", (long)size);

    /* Seek to offset 7 from beginning and read */
    lseek(fd, 7, SEEK_SET);
    char buf[40];
    ssize_t n = read(fd, buf, 39);
    buf[n] = '\0';
    printf("lseek(fd, 7, SEEK_SET) -> read from offset 7: \"%s\"\n", buf);

    /* Seek back to beginning */
    off_t pos = lseek(fd, 0, SEEK_SET);
    printf("lseek(fd, 0, SEEK_SET) -> reset to offset %ld\n", (long)pos);

    close(fd);
}

void demo_stat() {
    separator("4. stat() - File Status/Metadata");

    struct stat st;
    if (stat(FILE1, &st) < 0) { perror("stat"); return; }

    printf("stat(\"%s\"):\n", FILE1);
    printf("  File size          : %ld bytes\n",      (long)st.st_size);
    printf("  Inode number       : %lu\n",             (unsigned long)st.st_ino);
    printf("  Hard link count    : %lu\n",             (unsigned long)st.st_nlink);
    printf("  UID / GID          : %d / %d\n",         st.st_uid, st.st_gid);
    printf("  Block size         : %ld bytes\n",       (long)st.st_blksize);
    printf("  Blocks allocated   : %ld\n",             (long)st.st_blocks);

    /* File type */
    printf("  File type          : ");
    if      (S_ISREG(st.st_mode))  printf("Regular file\n");
    else if (S_ISDIR(st.st_mode))  printf("Directory\n");
    else if (S_ISLNK(st.st_mode))  printf("Symbolic link\n");
    else                           printf("Other\n");

    /* Permissions */
    printf("  Permissions        : ");
    printf("%c%c%c%c%c%c%c%c%c\n",
           (st.st_mode & S_IRUSR) ? 'r' : '-',
           (st.st_mode & S_IWUSR) ? 'w' : '-',
           (st.st_mode & S_IXUSR) ? 'x' : '-',
           (st.st_mode & S_IRGRP) ? 'r' : '-',
           (st.st_mode & S_IWGRP) ? 'w' : '-',
           (st.st_mode & S_IXGRP) ? 'x' : '-',
           (st.st_mode & S_IROTH) ? 'r' : '-',
           (st.st_mode & S_IWOTH) ? 'w' : '-',
           (st.st_mode & S_IXOTH) ? 'x' : '-');

    char time_buf[64];
    struct tm *t = localtime(&st.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", t);
    printf("  Last modified      : %s\n", time_buf);
}

void demo_copy_with_syscalls() {
    separator("5. File Copy using read()/write()");

    int src = open(FILE1, O_RDONLY);
    int dst = open(FILE2, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (src < 0 || dst < 0) { perror("open"); return; }
    printf("Copying \"%s\" -> \"%s\"\n", FILE1, FILE2);

    char buf[256];
    ssize_t n;
    long total = 0;
    while ((n = read(src, buf, sizeof(buf))) > 0) {
        write(dst, buf, n);
        total += n;
    }

    close(src);
    close(dst);
    printf("Copied %ld bytes successfully.\n", total);
}

void demo_link_symlink() {
    separator("6. link() and symlink()");

    if (link(FILE1, HLINK) == 0)
        printf("link(\"%s\", \"%s\") -> hard link created\n", FILE1, HLINK);
    else
        printf("link() -> %s\n", strerror(errno));

    if (symlink(FILE1, SLINK) == 0)
        printf("symlink(\"%s\", \"%s\") -> symbolic link created\n", FILE1, SLINK);
    else
        printf("symlink() -> %s\n", strerror(errno));

    /* Verify hard link increases nlink count */
    struct stat st;
    stat(FILE1, &st);
    printf("Hard link count of \"%s\" = %lu\n", FILE1, (unsigned long)st.st_nlink);
}

void demo_access() {
    separator("7. access() - Permission Check");

    const char *files[] = { FILE1, FILE2, "/etc/passwd", "/root/secret_nonexistent" };
    for (int i = 0; i < 4; i++) {
        printf("\"%s\":\n", files[i]);
        printf("  Exists (F_OK)    : %s\n", access(files[i], F_OK)  == 0 ? "YES" : "NO");
        printf("  Readable (R_OK)  : %s\n", access(files[i], R_OK)  == 0 ? "YES" : "NO");
        printf("  Writable (W_OK)  : %s\n", access(files[i], W_OK)  == 0 ? "YES" : "NO");
        printf("  Executable (X_OK): %s\n", access(files[i], X_OK)  == 0 ? "YES" : "NO");
    }
}

void demo_rename_unlink() {
    separator("8. rename() and unlink() (cleanup)");

    /* Rename copy file */
    if (rename(FILE2, "demo_renamed.txt") == 0)
        printf("rename(\"%s\", \"demo_renamed.txt\") -> success\n", FILE2);
    else
        perror("rename");

    /* Delete files */
    const char *to_delete[] = { "demo_renamed.txt", HLINK, SLINK };
    for (int i = 0; i < 3; i++) {
        if (unlink(to_delete[i]) == 0)
            printf("unlink(\"%s\") -> deleted\n", to_delete[i]);
        else
            printf("unlink(\"%s\") -> %s\n", to_delete[i], strerror(errno));
    }

    /* Delete main file last */
    if (unlink(FILE1) == 0)
        printf("unlink(\"%s\") -> deleted\n", FILE1);
}

int main() {
    printf("============================================\n");
    printf("  UNIX System Calls for File Management    \n");
    printf("============================================\n");

    demo_creat_open_write();
    demo_open_read();
    demo_lseek();
    demo_stat();
    demo_copy_with_syscalls();
    demo_link_symlink();
    demo_access();
    demo_rename_unlink();

    separator("Summary of System Calls Demonstrated");
    printf("  creat()   - Create a new file\n");
    printf("  open()    - Open a file (various flags)\n");
    printf("  close()   - Close a file descriptor\n");
    printf("  read()    - Read bytes from file\n");
    printf("  write()   - Write bytes to file\n");
    printf("  lseek()   - Reposition file offset\n");
    printf("  stat()    - Get file metadata\n");
    printf("  link()    - Create hard link\n");
    printf("  symlink() - Create symbolic link\n");
    printf("  access()  - Check file permissions\n");
    printf("  rename()  - Rename a file\n");
    printf("  unlink()  - Delete a file\n");
    printf("\nAll demonstrations completed successfully.\n");

    return 0;
}

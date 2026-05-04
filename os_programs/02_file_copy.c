/*
 * Program 2: Copy content of one file to another using
 * system calls: open(), read(), write(), close().
 *
 * Usage: ./02_file_copy <source_file> <destination_file>
 * Example: ./02_file_copy source.txt dest.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];
    long total_bytes = 0;

    printf("=== File Copy using System Calls ===\n\n");

    /* Check command-line arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        fprintf(stderr, "Example: %s source.txt dest.txt\n", argv[0]);

        /* Demo: create a sample file and copy it */
        printf("\n[Demo Mode] Creating sample files for demonstration...\n");

        /* Create a sample source file using system calls */
        src_fd = open("sample_source.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (src_fd < 0) { perror("open source"); exit(EXIT_FAILURE); }
        const char *sample = "Hello, World!\nThis is a sample file.\nLine 3: OS Lab Demo.\nLine 4: File Copy using System Calls.\n";
        write(src_fd, sample, strlen(sample));
        close(src_fd);

        /* Now copy sample_source.txt -> sample_dest.txt */
        argv[1] = "sample_source.txt";
        argv[2] = "sample_dest.txt";
        printf("Source      : %s\nDestination : %s\n\n", argv[1], argv[2]);
    }

    /* Open source file for reading (O_RDONLY) */
    src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0) {
        fprintf(stderr, "Error opening source file '%s': %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Source file '%s' opened successfully (fd = %d)\n", argv[1], src_fd);

    /* Open destination file for writing; create if not exists, truncate if exists */
    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        fprintf(stderr, "Error opening destination file '%s': %s\n", argv[2], strerror(errno));
        close(src_fd);
        exit(EXIT_FAILURE);
    }
    printf("Destination file '%s' opened successfully (fd = %d)\n\n", argv[2], dest_fd);

    printf("Copying...\n");

    /* Read from source and write to destination in chunks */
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("write error");
            close(src_fd);
            close(dest_fd);
            exit(EXIT_FAILURE);
        }
        total_bytes += bytes_written;
    }

    if (bytes_read < 0) {
        perror("read error");
        close(src_fd);
        close(dest_fd);
        exit(EXIT_FAILURE);
    }

    /* Close both file descriptors */
    close(src_fd);
    close(dest_fd);

    printf("File copy completed successfully!\n");
    printf("Total bytes copied: %ld bytes\n\n", total_bytes);

    /* Verify by reading destination */
    printf("=== Contents of destination file '%s' ===\n", argv[2]);
    dest_fd = open(argv[2], O_RDONLY);
    while ((bytes_read = read(dest_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }
    close(dest_fd);
    printf("\n=== End of file ===\n");

    return 0;
}

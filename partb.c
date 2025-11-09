#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char* argv[]) {
    // Check arguments
    if (argc != 3) {
        printf("USAGE: partb FILEIN FILEOUT\n");
        return 1;
    }

    // Declare the buffer to use. Do NOT change!
    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];

    char* infile = argv[1];
    char* outfile = argv[2];
    int upper = 0, lower = 0, number = 0, space = 0, other = 0;
    
    int fd_in = open(infile, O_RDONLY);
    if (fd_in == -1) {
        printf("ERROR: %s not found\n", infile);
        return 2;
    }

    ssize_t bytesRead;
    while ((bytesRead = read(fd_in, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (isupper(buffer[i])) {
                upper++;
            } else if (islower(buffer[i])) {
                lower++;
            } else if (isdigit(buffer[i])) {
                number++;
            } else if (isspace(buffer[i])) {
                space++;
            } else {
                other++;
            }
        }
    }

    close(fd_in);

    // Open output file
    int fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        printf("ERROR: Cannot create %s\n", outfile);
        return 3;
    }

    // Write results to output file in CSV format
    char output[1024];
    int len = snprintf(output, sizeof(output),
        "Category,Count\n"
        "Upper,%d\n"
        "Lower,%d\n"
        "Number,%d\n"
        "Space,%d\n"
        "Other,%d\n",
        upper, lower, number, space, other);
    write(fd_out, output, len);

    close(fd_out);
    
    return 0;
}

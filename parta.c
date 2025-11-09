#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Declare the buffer to use. Do NOT change!
    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];
    
    int upper = 0, lower = 0, number = 0, space = 0, other = 0;
    int fd;

    // Check arguments
    if (argc != 2) {
        printf("USAGE: parta FILEIN\n");
        return 1;
    }

    // Open input file
    char* filename = argv[1];
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("ERROR: %s not found\n", filename);
        return 2;
    }

    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
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
    
    close(fd);
    
    printf("Upper,%d\n", upper);
    printf("Lower,%d\n", lower);
    printf("Number,%d\n", number);
    printf("Space,%d\n", space);
    printf("Other,%d\n", other);
    return 0;
}

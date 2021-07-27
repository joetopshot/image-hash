#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// see file md5.c
extern void md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest);

void calcMD5(const char* filename, uint8_t result[]) {

    FILE *f = fopen(filename, "rb" );
    if (!f) {
    	printf("Unable to open file!");
    	return;
    }

    // determine length of file
    fseek(f, 0, SEEK_END);
    int fileLength = ftell(f);
    fseek(f, 0, SEEK_SET);
//    printf("length of file is %d\n", fileLength);

    char * msg = malloc(fileLength);

    fread(msg, 1, fileLength, f);
    fclose(f);

    md5((uint8_t*)msg, fileLength, result);
    free(msg);
}

int main(int argc, char **argv) {
	uint8_t result[16];

    // fetch a file
    const char* fileName1 = "data/LS7_3551.JPG";
    calcMD5(fileName1, result);
    for (int i = 0; i < 16; i++) {
        printf("%2.2x", result[i]);
    }

}

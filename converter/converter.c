#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

char filename_in[2000];
char filename_out[2000];
int is_binary = 0;
int main(int argc, char** argv) {
    int c;
    while ((c = getopt (argc, argv, "b:f:o:")) != -1) {
        switch (c)
        {
            case 'f':
                if(optarg != NULL) {
                    strcpy(filename_in, optarg);
                    is_binary = 0;
                } else {
                    printf("No filename specified\n");
                    return -1;
                }
                break;
            case 'b':
                if(optarg != NULL) {
                    strcpy(filename_in, optarg);
                    is_binary = 1;
                } else {
                    printf("No filename specified\n");
                    return -1;
                }
                break;
            case 'o':
                if(optarg != NULL) {
                    strcpy(filename_out, optarg);
                } else {
                    printf("No filename specified\n");
                    return -1;
                }
                break;
        }
    }
    if(is_binary) {
        
    }
}

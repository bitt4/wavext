#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

void display_help();

int main(int argc, char *argv[]){

    static struct option long_options[] = {
                                           {"help", no_argument, NULL, 'h'},
                                           {NULL, 0, NULL, 0}
    };

    int c;

    while ((c = getopt_long(argc, argv, "h", long_options, NULL)) != -1) {
        switch (c)
            {
            case 'h':
                display_help();
                exit(EXIT_FAILURE);
            case '?':
                fprintf(stderr,
                        "%s: Use -h or --help to display options.\n", argv[0]);
                exit(EXIT_FAILURE);
            default: {}
            }
    }

    if(argc < 2){
        display_help();
        return -1;
    }

    FILE* root_file = fopen(argv[1], "rb");

    if(root_file == NULL){
        fprintf(stderr, "Couldn't open file '%s'.\n", argv[1]);
        return -1;
    }

    fseek(root_file, 0, SEEK_END);
    unsigned long root_file_size = ftell(root_file);

    if(root_file_size == 0){
        fprintf(stderr, "File '%s' is empty.\n", argv[1]);
        return -1;
    }

    fseek(root_file, 0, SEEK_SET);    /* Set position of file stream to the beginning */
    char* root_file_read_buffer = (char*)malloc(root_file_size);
    fread(root_file_read_buffer, root_file_size, 1, root_file);
    fclose(root_file);

    FILE* current_file = NULL;
    char filename[16];
    unsigned int file_count = 0;

    const int header_length = 44;
    unsigned int current_file_data_size = 0;

    for(int pos = 0; pos < root_file_size - 3 - 4 - 8; pos++){
        if(memcmp(root_file_read_buffer + pos, "RIFF", 4) == 0){    /* check for RIFF header */
            /* fprintf(stdout, "RIFF header found.\n"); */

            if(memcmp(root_file_read_buffer + pos + 8, "WAVEfmt ", 8) == 0){    /* check for WAVE and fmt headers */
                /* fprintf(stdout, "WAVE and fmt headers found.\n"); */
                int data_header_offset = 40;
                for(int i = 0; i < 100; i++){
                    if(memcmp(root_file_read_buffer + pos + i, "data", 4) == 0){
                        data_header_offset = i + 4;
                        break;
                    }
                }
                memcpy(&current_file_data_size, root_file_read_buffer + pos + data_header_offset, 4);    /* data size offset */
                /* fprintf(stdout, "Data block size: %u.\n", current_file_data_size); */

                /* Check if data block of current file isn't bigger than length of remaining data in root file */
                if(root_file_size - pos - header_length >= current_file_data_size){
                    /* fprintf(stdout, "Remaining bytes in data: %lu.\n", root_file_size - pos - header_length); */
                    snprintf(filename, 15, "wav_%04u.wav", ++file_count);
                    current_file = fopen(filename, "wb");
                    fwrite(root_file_read_buffer + pos, header_length + current_file_data_size, 1, current_file);
                    fclose(current_file);
                    fprintf(stdout, "'%s' extracted.\n", filename);
                }
            }
        }
    }

    if(file_count == 0){
        fprintf(stdout, "No wav files found.\n");
    }
    else {
        fprintf(stdout, "%u files extracted.\n", file_count);
    }

    free(root_file_read_buffer);

    return 0;
}

void display_help(){
    fprintf(stdout, "Usage: wavext [OPTIONS...] [FILE]\n");
    /* not fully implemented */
}

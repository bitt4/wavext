#include <stdio.h>
#include <stdlib.h>

void displayHelp();

int main(int argc, char *argv[]){

    if(argc < 2){
        displayHelp();
        return -1;
    }

    FILE* root_file = fopen(argv[1], "rb");

    if(root_file == NULL){
        fprintf(stderr, "Couldn't open file '%s'.\n", argv[1]);
        return -1;
    }

    fseek(root_file, 0, SEEK_END);
    long root_file_size = ftell(root_file);

    if(root_file_size == 0){
        fprintf(stderr, "File '%s' is empty.\n", argv[1]);
        return -1;
    }

    fseek(root_file, 0, SEEK_SET);    /* Set position of file stream to the beginning */
    char* root_file_read_buffer = (char*)malloc(root_file_size);
    fread(root_file_read_buffer, root_file_size, 1, root_file);
    fclose(root_file);

    for(int i = 0; i < root_file_size; i++){
        if(   root_file_read_buffer[i + 0] == 0x52    /* check for RIFF header */
           && root_file_read_buffer[i + 1] == 0x49
           && root_file_read_buffer[i + 2] == 0x46
           && root_file_read_buffer[i + 3] == 0x46){
            fprintf(stdout, "RIFF header found.\n");
        }
    }

    return 0;
}

void displayHelp(){
    fprintf(stderr, "Usage: wavext [OPTIONS] [FILE]\n");
    /* not fully implemented */
}

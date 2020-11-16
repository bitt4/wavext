#include <stdio.h>

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

    return 0;
}

void displayHelp(){
    fprintf(stderr, "Usage: wavext [OPTIONS] [FILE]\n");
    /* not fully implemented */
}

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 
#define MAX_FILENAME_LEN 100
#define SHM_FILE "shm1" // It'll be placed in /dev/shm
#define SHM_FILE_PATH "/dev/shm/"SHM_FILE
#define BUF_SIZE 128
 
void parent_process();
void child_process();

int r, fd_sh;
char bufor[BUF_SIZE];

int main(void)
{

    // Create file used for data sharing
    fd_sh = shm_open(SHM_FILE,  O_CREAT | O_RDWR, 0666);

    pid_t pid;
    if ((pid = fork()) == -1){
        fprintf(stderr, "Error: couldn't create a subprocess.\n");
        exit(-2);
    }
    if (pid > 0){
        parent_process();
    }
    else{
        child_process();
    }
    return 0;
}
 
void parent_process(){
    char* map;
    char file_name[MAX_FILENAME_LEN];
    char option;

    while ((option != 'q') && (option != EOF)){
        // Unmap memory if necessary
  
        printf("\nPlease enter the name of the image file.\n");
        scanf("%s", file_name);
        int image_file = open(file_name, O_RDONLY);
        if (image_file == -1){
            fprintf(stderr, "Error: Incorrect file path.\n");
            exit(-3);
        }

        // Get the image file statistic and resize memory file

        struct stat fileStat;
        stat(file_name, &fileStat);
        int tmpSize = fileStat.st_size;
        ftruncate(fd_sh, tmpSize);

        // Create a new mapping in virtual address space

        map = (char *)mmap(NULL, tmpSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_sh, 0);

        if(map == MAP_FAILED){
            printf("Memory error!\n");
            shm_unlink(SHM_FILE);
            close(fd_sh);
            exit(0);
        }

        // Read data from file and write it to the memory.

        int i=0;
        while((r=read(image_file, bufor, BUF_SIZE))>0){
            map[i]=bufor[0];
            i++;
        }
 
        printf("\nPush 't' to load next file. Otherwise push 'q'.\n");
        while ((option = getc(stdin)) == '\n');

        close(image_file);
        munmap(map, tmpSize);
    }
}
 
void child_process(){

    // Check if first image has been written and print all received images

    execlp("display", "display", SHM_FILE_PATH, (char *) NULL);
    printf("jestem tu\n");
}

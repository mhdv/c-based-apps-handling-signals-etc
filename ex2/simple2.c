#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#define BUF_SIZE 128000

int main(int argc, char * argv[]) {
        int r;
        char bufor[BUF_SIZE];
        int pid = fork();

	
        if(pid == -1){
                exit(0);
        }
        if (pid == 0) {
                execlp("tail","tail","-f","FIFO1",(char *) NULL);
                exit(0);
        }else{
                int fifofd = mknod("FIFO1",O_WRONLY, 0666);
                int fd=open(argv[1], O_RDONLY);

                while ((r=read(fd, bufor, BUF_SIZE)) > 0)
                        write(fifofd, bufor, r);
                close(fifofd);
        }
}
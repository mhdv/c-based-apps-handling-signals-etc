#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#define BUF_SIZE 128000

int main(int argc, char * argv[]) {
        int potok_fd[2], r;
        char bufor[BUF_SIZE];
        pipe(potok_fd);
        int pid = fork();

        int fd=open(argv[1], O_RDONLY);


        if(pid == -1){
                exit(0);
        }
        if (pid == 0) {
                // CHILD
                close(0);
                dup(potok_fd[0]);

                close(potok_fd[1]); 
                close(potok_fd[0]);

                execlp("display","display", (char *) NULL);
                close(1);
                exit(0);
        }else{
                // PARENT
                close(1);
                while ((r=read(fd, bufor, BUF_SIZE)) > 0)
                        write(potok_fd[1], bufor, r);
        }
}

// parent
// int fd_sh = shm_open(...)     /dev/shm
// shm_vlink
/* while(1){
        //get file name
        int fd = open(...)
        fstat(...) ~rozmiar pliku
        ftmucate(...)  ~ shm_file/ ustawiamy tak jak odczytamy
        ptr = mmap(...) /mapować shm_file do pamięci wirtualnej
        while(read(obraz1)){
                ptr[i++]=c;
        }
        unmap/nunmap/munmap
        close(fd)
        }      
}
//child
   // wait for first image
   execlp("display", ze ścieżką do pliku wymiany)
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>


int main()
{
  int pamiec = shm_open("pamiec", O_CREAT | O_RDWR, 0666);
  
  pid_t proc;
  proc = fork();

  if(proc)
  {
        char nazwa[40];
	int img, potok, readBits, wB;
 	int buffSize = 4096;
  	char imgBuff[buffSize];

	while(1)
  	{
  	   printf("Wprowadz nazwe pliku: ");
  	   scanf("%s", nazwa);
  	   printf("Plik: %s\n", nazwa);
	
	   img = open(nazwa, O_RDONLY);
           if(img < 0)
           {
              printf("Brak takiego pliku!\n");
	      shm_unlink("pamiec");
	      close(pamiec);
              exit(0);
           }

           if(pamiec < 0)
           {
              printf("Brak pamieci!\n");
              exit(0);
           }

	  struct stat imgStatus; 
	  stat(nazwa, &imgStatus);

	  ftruncate(pamiec, imgStatus.st_size);

	  char* addr = (char*)mmap(NULL, imgStatus.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, pamiec, 0);

	  if(addr == MAP_FAILED)
	  {
		printf("Blad zaalokowania pamieci!\n");
		shm_unlink("pamiec");
		close(pamiec);
		exit(0);
	  }

	  int wsk = 0;

          do
          {
              readBits = read(img, imgBuff, buffSize);
	      wB = 0;

		for(int i = readBits; i; --i) 
	      	{
		   addr[wsk] = imgBuff[readBits - i];
		   ++wsk;
		   ++wB;
		}

              printf("Proces: %i, rB = %i, wB = %i\n", proc, readBits, wB);
          } while(readBits);

	  close(img);
	  munmap(addr, imgStatus.st_size);
       }
  } else
  {	
	struct stat memStatus; 
	
	do
	{
	    fstat(pamiec, &memStatus);
	    sleep(1);
	} while(!memStatus.st_size); 

	char* arg[5];
	char program[] = "display";
	char plik[] = "/dev/shm/pamiec";
	char opcja[] = "-update";
	char opVal[] = "1";
	arg[0] = program;
	arg[1] = opcja;
	arg[2] = opVal;
	arg[3] = plik;
	arg[4] = NULL;

	execvp(arg[0], arg);
  }

  return 0;
}
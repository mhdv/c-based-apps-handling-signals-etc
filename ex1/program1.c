#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
 
// The function used as a handler.
void message(int sig)
{
   printf("\nReceived signal nr=%d", sig);
}
 
int main()
{
   struct sigaction act;
   act.sa_handler = message;  // Pointer to handler
 
   sigemptyset(&act.sa_mask); // Clears the mask of signals
   sigaction(SIGQUIT, &act, NULL); // Sets the signal bit in signals mask
   sigaction(SIGINT, &act, NULL); // Sets the signal bit in signals mask
   while (1)
   {
      sleep(2);
   }
}
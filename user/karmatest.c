#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;

  printf("--- KARMA TEST START ---\n");

  // Child 1: The CPU Hog
  pid = fork();
  if(pid == 0) {
    set_priority(10); // Start at middle priority
    printf("Child 0 (HOG) started with priority %d\n", get_priority());
    
    for(int i = 0; i < 5; i++) {
      // Hard work
      volatile int count = 0;
      for(int j = 0; j < 200000000; j++) { count++; }
      
      printf("HOG Update: My priority is now %d (getting worse)\n", get_priority());
    }
    printf("HOG: FINISHED\n");
    exit(0);
  }

  // Child 2: The Good Citizen (Interactive)
  pid = fork();
  if(pid == 0) {
    set_priority(10); // Start at middle priority
    printf("Child 1 (CITIZEN) started with priority %d\n", get_priority());
    
    for(int i = 0; i < 5; i++) {
      // Do a tiny bit of work then sleep (simulate I/O)
      volatile int count = 0;
      for(int j = 0; j < 1000000; j++) { count++; }
      
      pause(5); // Simulate waiting for user input or disk
      
      printf("CITIZEN Update: My priority is now %d (getting better)\n", get_priority());
    }
    printf("CITIZEN: FINISHED\n");
    exit(0);
  }

  // Parent waits for both
  wait(0);
  wait(0);

  printf("\n--- KARMA TEST COMPLETE ---\n");
  exit(0);
}

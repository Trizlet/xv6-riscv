#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

unsigned int next = 1;

int
rand(void)
{
  next = next * 1103515245 + 12345;
  return (unsigned int)(next / 65536) % 32768;
}

void
srand(unsigned int seed)
{
  next = seed;
}

int
main(int argc, char *argv[])
{
  int n = 5;
  int pid;
  int priorities[5];

  srand(uptime());

  printf("--- SCHEDULER BENCHMARK START ---\n");
  
  // Phase 1: Setup (Parent does all the talking here)
  for(int i = 0; i < n; i++) {
    priorities[i] = (rand() % 20) + 1;
    printf("Child %d: Assigned Priority %d\n", i, priorities[i]);
  }

  printf("\n--- FORKING CHILDREN ---\n");

  // Phase 2: Execution
  for(int i = 0; i < n; i++) {
    pid = fork();
    if(pid < 0) {
      exit(1);
    }
    if(pid == 0) {
      set_priority(priorities[i]);
      
      // Increased workload to give the scheduler more "thinking time"
      volatile int count = 0;
      for(int j = 0; j < 200000000; j++) {
        count++;
      }
      
      printf("Child %d (prio %d): FINISHED\n", i, priorities[i]);
      exit(0);
    }
  }

  // Parent waits silently
  for(int i = 0; i < n; i++) {
    wait(0);
  }

  printf("--- BENCHMARK COMPLETE ---\n");
  exit(0);
}

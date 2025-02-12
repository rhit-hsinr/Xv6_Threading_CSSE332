#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/spinlock.h"

struct spinlock lock;

int global_counter = 0;

void thread_fn(void* arg) {
  global_counter++;
  int num = *(int*) arg;
  printf("hello from thread with argument: %d\n", num);
  printf("global counter from thread: %d\n", global_counter);
  exit(0);
}

int main(int argc, char *argv[]) {
  printf("Hello from my test case in xv6\n");
  void *stack = malloc(100);
  if (stack == 0) {
    printf("failed kalloc\n");
    exit(0);
  }
  uint64 p = 0xdeadbeef;
  int thread_arg = 10;
  spoon((void*)p);
  int pid = clone(thread_fn, &thread_arg, stack);
  sleep(3);
  join(pid, &stack);
  free(stack);
  printf("global counter: %d\n", global_counter);
  exit(0);
}

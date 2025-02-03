#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void thread_fn(void* arg){
  printf("hello from thread\n");
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
  join(pid);
  free(stack);
  exit(0);
}

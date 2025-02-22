#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/spinlock.h"
#include "user/utensils.h"

#define PGSIZE 4096

struct spinlock lock;

void thread_fn1(void* arg) {
  int num = *(int*) arg;
  printf("hello from thread 1 with argument: %d\n", num);
  sleep(5);
  exit(0);
}

void thread_fn2(void *arg) {
  int num = *(int*) arg;
  printf("hello from thread 2 with argument: %d\n", num);
  exit(0);
}
  

void *create_stack(int sz) {
  return malloc(sz) + sz;
}

int main(int argc, char *argv[]) {
  printf("Hello from my test case for graceful exit\n");
  void *stack1 = create_stack(PGSIZE);
  void *stack2 = create_stack(PGSIZE);
  void *stack3 = create_stack(PGSIZE);

  int thread_arg = 10;
  int thread_arg2 = 11;
  int thread_arg3 = 12;

  int pid = uspork_create(thread_fn1, &thread_arg, stack1);
  int pid2 = uspork_create(thread_fn1, &thread_arg2, stack2);
  int pid3 = uspork_create(thread_fn2, &thread_arg3, stack3);
  sleep(3);

  int rpid = uspork_join(pid, &stack1);
  int rpid2 = uspork_join(pid2, &stack2);
  int rpid3 = uspork_join(pid3, &stack3);

  free(stack1 - PGSIZE);
  free(stack2 - PGSIZE);
  free(stack3 - PGSIZE);

  if (rpid + rpid2 + rpid3 == 0) {
    printf("Every thread has exited gracefully\n");
    exit(0);
  } else {
    printf("your threads ain't it\n");
    exit(0);
  }
}

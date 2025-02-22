#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/spinlock.h"
#include "user/utensils.h"

#define PGSIZE 4096

struct spinlock lock;

int global_counter = 0;

void thread_fn(void* arg) {
  global_counter++;
  int num = *(int*) arg;
  printf("hello from thread with argument: %d\n", num);
  printf("global counter from thread: %d\n", global_counter);
  sleep(5);
  exit(0);
}

void thread_fn2(void *arg) {
  sleep(20);
  global_counter++;
  int num = *(int*) arg;
  printf("from thread 2 with argument: %d\n", num);
  printf("global counter from thread 2: %d\n", global_counter);
  exit(0);
}
  

void *create_stack(int sz) {
  return malloc(sz) + sz;
}

int main(int argc, char *argv[]) {
  printf("Hello from my test case in xv6\n");
  void *stack1 = create_stack(PGSIZE);
  void *stack2 = create_stack(PGSIZE);
  void *stack3 = create_stack(PGSIZE);

  uint64 p = 0xdeadbeef;
  int thread_arg = 10;
  int thread_arg2 = 11;
  int thread_arg3 = 12;

  spoon((void*)p);
  int pid = uspork_create(thread_fn, &thread_arg, stack1);
  int pid2 = uspork_create(thread_fn, &thread_arg2, stack2);
  int pid3 = uspork_create(thread_fn2, &thread_arg3, stack3);
  sleep(3);
  int rpid = uspork_join(pid, &stack1);
  int rpid2 = uspork_join(pid2, &stack2);
  int rpid3 = uspork_join(pid3, &stack3);
  printf("do you got me gng pid: %d\n", rpid);
  printf("do you rlly got me gng pid: %d\n", rpid2);
  printf("do you rlly rlly got me gng pid: %d\n", rpid3);
  free(stack1 - PGSIZE);
  free(stack2 - PGSIZE);
  free(stack3 - PGSIZE);
  printf("global counter: %d\n", global_counter);
  exit(0);
}

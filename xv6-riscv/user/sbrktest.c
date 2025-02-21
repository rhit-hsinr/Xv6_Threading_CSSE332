#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/utensils.h"
#include "kernel/spinlock.h"

#define PGSIZE 4096

int *p = (int *)0xdeadbeef;

void thread_t1(void *arg) {
  printf("Value of p: %p\n", p);
  p = (int *)sbrk(2*PGSIZE);
  printf("New Value of p: %p\n", p);

  if (p == (void *) -1) {
    printf("sbrk failed chief\n");
    return;
  }

  *p = 123;

  printf("in thread one p = %d\n", *p);

  printf("Thread 1 allocated mem and add values\n");
  exit(0);
}

void thread_t2(void *arg) {
  sleep(20);

  printf("Value of p: %p\n", p);

  if (p == (int *)0xdeadbeef) {
    printf("gg you failed gng... p is the same\n");
    return;
  }

  printf("did you pass through here gang\n");

  printf("thread 2 p = %d\n", *p);
  
  if (p[0] == 1 && p[1] == 2 && p[2] == 1) {
    printf("wait you cooked\n");
  } else {
    printf("damn you failed on the second step gng\n");
  }

  printf("what about here gang\n");

  if(sbrk(-PGSIZE) == (void *)-1) {
    printf("sbrk can't delete a page\n");
  } else {
    printf("Thread 2 deallocated one page chat\n");
  }
  exit(0);
}

void *create_stack(int sz) {
  return malloc(sz) + sz;
}

int main(int argc, char *argv[]) {
  printf("Hello from my sbrk test case in xv6\n");
  void *stack1 = create_stack(PGSIZE);
  void *stack2 = create_stack(PGSIZE);
  int arg = 1;
  int pid = uspork_create(thread_t1, &arg, stack1);
  int pid2 = uspork_create(thread_t2, &arg, stack2);
  uspork_join(pid, &stack1);
  uspork_join(pid2, &stack2);
  free(stack1 - PGSIZE);
  free(stack2 - PGSIZE);
  exit(0);
}

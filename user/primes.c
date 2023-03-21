#include "kernel/types.h"
#include "user/user.h"

void
process(int p[], int initNumber)
{
  close(p[1]);
  int number = 0;

  printf("prime %d\n", initNumber);

  int readNum;
  while((readNum = read(p[0], &number, 4)) != 0){
    if(number % initNumber != 0) break;
  }
  if(readNum != 0){
    int pSon[2];
    pipe(pSon);

    int rc = fork();
    if(rc == 0){
      process(pSon, number);
    }else{
      close(pSon[0]);
      while(1){
        int readNum = read(p[0], &number, 4);
        if(readNum == 0){
          close(p[0]);
          break;
        }
        if(number % initNumber != 0){
          write(pSon[1], &number, 4);
        }
      }
      close(pSon[1]);
      wait(0);
    }
  }
}

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: primes\n");
    exit(1);
  }

  int p[2];
  pipe(p);

  printf("prime 2\n");
  int number = 0;
  for(int i = 2; i <= 35; i++){
    if(i % 2 != 0){
        number = i;
        break;
    }
  }

  int rc = fork();
  if(rc == 0){
    process(p, number);
  }else{
    close(p[0]);
    for(int i = number; i <= 35; i++){
      int number = i;
      if(i % 2 != 0){
        write(p[1], &number, 4);
      }
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
}
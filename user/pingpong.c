#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: pingpong\n");
    exit(1);
  }

  int p1[2];
  pipe(p1);
  int p2[2];
  pipe(p2);

  char aByte[1];
  aByte[0] = 'B';

  int rc = fork();
  if(rc == 0){
    close(p1[1]); // close write 
    char gByte[1];
    read(p1[0], gByte, 1);
    close(p1[0]);
    fprintf(1, "%d: received ping\n", (int)getpid());

    write(p2[1], gByte, 1);
    close(p2[0]);
    close(p2[1]);
    exit(0);
  }else{
    write(p1[1], aByte, 1);
    close(p1[0]);
    close(p1[1]);

    close(p2[1]); // close write
    char gByte[1];
    read(p2[0], gByte, 1);
    close(p2[0]);
    fprintf(1, "%d: received pong\n", (int)getpid());
    exit(0);
  }
}
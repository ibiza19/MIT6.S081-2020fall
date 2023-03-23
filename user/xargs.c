#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

void
exeArgs(char argvPre[], int argc)
{
  char *argvExe[argc+1];
  for(int i = 0; i < argc+1; i++){
    argvExe[i] = 0;
  }
  for(int i = 0; i < argc; i++){
    argvExe[i] = malloc(strlen(argvPre)+1);
    strcpy(argvExe[i], argvPre+i*MAXARG);
  }
  int rc = fork();
  if(rc == 0){
    exec(argvExe[0], argvExe);
  }else{
    wait(0);
  }
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: xargs [exec] [argv...]");
    exit(1);
  }

  char argvPre[MAXARG*MAXARG];
  memset(argvPre, '\0', MAXARG * MAXARG);

  int i = 0;
  int readNum = 0;
  for(i = 0; i < argc - 1; i++){
    memmove(argvPre+MAXARG*i, argv[i+1], strlen(argv[i+1]));
  }

  char *p = argvPre+MAXARG*i;
  while((readNum = read(0, p, 1)) != 0){
    if(*p == ' '){
      *p = '\0';
      i++;
      p = argvPre+MAXARG*i;
      continue;
    }
    if(*p == '\n'){
      *p = '\0';
      exeArgs(argvPre, i+1);
      i = argc - 1;
      p = argvPre+MAXARG*i; 
      continue; 
    }
    p++;
  }

  exit(0);
}
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  int r = 0;
  r = fork();
  for (int i = 0; i < 3; i++)
  {
    r = execl("/bin/echo","/bin/echo","Hello World",NULL);
    printf("%d\n", r);
  }
  return 0;
} 





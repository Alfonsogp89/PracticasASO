#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include   <unistd.h>

int main(void)
{
    printf("Hello, ASO!\n");
    printf("Pi pid es %d y el de mi padre es %d \n", getpid(), getppid());
    return 0;
}
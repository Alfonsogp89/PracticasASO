#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Internamente todas las funciones realizan la llamada al sistema write*/
int main(void)
{
    char msg[] = "Hello, ASO!\n";

    printf(         "%s", msg);

    fprintf(stdout, "%s", msg);

    /*Salida de error (2)*/
    fprintf(stderr, "%s", msg);

    write(STDOUT_FILENO,  msg, strlen(msg));

    return 0;
}
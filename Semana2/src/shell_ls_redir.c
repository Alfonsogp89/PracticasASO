#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t pid; /* Usado en el proceso padre para guardar el PID del proceso hijo */
    int fd;
    int status; 

    char *argumentos[] = {"ls", "-la", NULL};

    switch (pid = fork())
    {
    case -1: /* fork() falló */
        perror("fork()");
        exit(EXIT_FAILURE);
        break;
    case 0:                             /* Ejecución del proceso hijo tras fork() con éxito */
        if (close(STDOUT_FILENO) == -1) /* Cierra la salida estándar */
        {
            perror("close()");
            exit(EXIT_FAILURE);
        }
        /* Abre el fichero "listado" al que se asigna el descriptor de fichero no usado más bajo, es decir, STDOUT_FILENO(1) */
        if ((fd = open("listado", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU |  S_IRUSR |  S_IWUSR)) == -1)
        /* Las dos últimas opciones son para que el usuario tenga lectura y escritura*/
        /* 
        - 1º opción : abrir con permiso de escritura
        Si listado existe y no usamos O_TRUNK: no se vacía primero el archivo si esta lleno. Por tanto, tendrá la salida de ls y los datos que habían anteriormente */
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }
        /*execlp("ls", "ls", "-la", NULL);       Sustituye el binario actual por /bin/ls */
        //execv("ls", argumentos);
        execlp("sleep", "slepp", "3600", NULL);
        /* Salta el error sin comprobación porque el execlp solo lo ejecuta si ha habido algún error ya que, el excecpl cambia su código por el de ls*/
        fprintf(stderr, "execlp() failed\n"); /* Esta línea no se debería ejecutar si la anterior tuvo éxito */
        exit(EXIT_FAILURE);
        break;
    default:                  /* Ejecución del proceso padre tras fork() con éxito */
        /*
        if (wait(NULL) == -1)  Espera a que termine el proceso hijo 
        {    Espera a que termine un solo hijo
            perror("wait()");
            exit(EXIT_FAILURE);
        }
        break;
        */
       if (waitpid(pid, &status, 0) == -1)  
        {    
            perror("wait()");
            exit(EXIT_FAILURE);
        }
        if(WIFEXITED(status)){
            printf("El proceso ha terminado de manera normal"); 
            exit(EXIT_SUCCESS);
        }

        if(WIFSIGNALED(status)){
            printf("El proceso ha terminado con la señal %d \n", WTERMSIG(status));
            exit(EXIT_FAILURE);
        }
        break;
    }

    /*Reservar memoria para el buffer de lectura*/

    return EXIT_SUCCESS;
}

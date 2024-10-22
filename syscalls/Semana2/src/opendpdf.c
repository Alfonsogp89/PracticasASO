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

    switch (pid = fork())
    {
    case -1: /* fork() falló */
        perror("fork()");
        exit(EXIT_FAILURE);
        break;
    case 0:                             /* Ejecución del proceso hijo tras fork() con éxito */
        
        if(argc != 2){
            fprintf(stderr, "Uso: ./opendpdf archivo.pdf \n");
            exit(EXIT_FAILURE);
        }
        /*R_OK comprueba que se puede leer*/
        if(access(argv[1], R_OK) == -1){
            fprintf(stderr, "El ficero no se puede leer \n");
            exit(EXIT_FAILURE);

        }
        /*      ruta     nombre del programa*/
        execlp("evince", "evince", argv[1], NULL);      /* Sustituye el binario actual por /bin/ls */
        /* Salta el error sin comprobación porque el execlp solo lo ejecuta si ha habido algún error ya que, el excecpl cambia su código por el de ls*/
        fprintf(stderr, "execlp() failed\n"); /* Esta línea no se debería ejecutar si la anterior tuvo éxito */
        exit(EXIT_FAILURE);
        break;
    default:                  /* Ejecución del proceso padre tras fork() con éxito */
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

    return EXIT_SUCCESS;

}
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

/*  Ejemplo: shell_ytdl_pipe_ffmeg -u YTURL -f FICHERO */

void print_help(char* program_name)
{
    fprintf(stderr, "Uso: %s -u YTURL -f FICHERO\n", program_name);
}

int main(int argc, char *argv[])
{
    int opt;
    char *url = NULL;
    char *filename = NULL;

    int pid_hijos[2];
    int status;

    optind = 1;
    /*Siempre que se necesien argumentos usamos getop. "u:" indica que es obligatorio usar el caracter */
    while ((opt = getopt(argc, argv, "u:f:h")) != -1)
    {
        switch (opt)
        {
        case 'u':
            url = optarg;
            break;
        case 'f':
            filename = optarg;
            break;
        case 'h':
            print_help(argv[0]);
            exit(EXIT_SUCCESS);
        default:
            print_help(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (url == NULL || filename == NULL)
    {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefds[2]; /* Descriptores de fichero de la tubería */

    if (pipe(pipefds) == -1) /* Paso 0: Creación de la tubería */
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    /* Paso 1: Creación del hijo izquierdo de la tubería */
    switch (pid_hijos[0]= fork())
    {
    case -1:
        perror("fork(1)");
        exit(EXIT_FAILURE);
        break;
    case 0: /* Hijo izquierdo de la tubería */
        /* Paso 2: Cierro El extremo de lectura no se usa */
        if (close(pipefds[0]) == -1)
        {
            perror("close(1)");
            exit(EXIT_FAILURE);
        }
        /* Paso 3: Redirige la salida estándar al extremo de escritura de la tubería */
        if (dup2(pipefds[1], STDOUT_FILENO) == -1)
        {
            perror("dup2(1)");
            exit(EXIT_FAILURE);
        }
        /* Paso 4: Cierra el descriptor duplicado */
        if (close(pipefds[1]) == -1)
        {
            perror("close(2)");
            exit(EXIT_FAILURE);
        }
        /* Paso 5: Reemplaza el binario actual por el de `youtube-dl` */
        /* -o redirige yt-dlp a la salida estandar*/
        execlp("./yt-dlp", "yt-dlp", url, "-q", "-o", "-", NULL);
        perror("execlp(izquierdo)");
        exit(EXIT_FAILURE);
        break;
    default: /* El proceso padre continúa... */
        break;
    }

    /* Paso 6: Creación del hijo derecho de la tubería */
    switch (pid_hijos[1] = fork())
    {
    case -1:
        perror("fork(2)");
        exit(EXIT_FAILURE);
        break;
    case 0: /* Hijo derecho de la tubería  */

        /*Redirigir salida estandar a fichero.log*/
        int fderr = open("ffmeg.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if(fderr == -1){
            perror("fderr");
            exit(EXIT_FAILURE);
        }
        if(dup2(STDERR_FILENO, fderr) == -1){
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        if(close(fderr) == -1){
            perror("close");
            exit(EXIT_FAILURE);
        }

        /* Paso 7: El extremo de escritura no se usa */
        if (close(pipefds[1]) == -1)
        {
            perror("close(3)");
            exit(EXIT_FAILURE);
        }
        /* Paso 8: Redirige la entrada estándar al extremo de lectura de la tubería */
        if (dup2(pipefds[0], STDIN_FILENO) == -1)
        {
            perror("dup2(2)");
            exit(EXIT_FAILURE);
        }
        /* Paso 9: Cierra el descriptor duplicado */
        if (close(pipefds[0]) == -1)
        {
            perror("close(4)");
            exit(EXIT_FAILURE);
        }
        /* Paso 10: Reemplaza el binario actual por el de `ffmpeg` */
        execlp("ffmpeg", "ffmpeg", "-i", "-", filename, NULL);
        perror("execlp(derecho)");
        exit(EXIT_FAILURE);
        break;
    default: /* El proceso padre continúa... */
        break;
    }

    /* El proceso padre cierra los descriptores de fichero no usados */
    if (close(pipefds[0]) == -1)
    {
        perror("close(pipefds[0])");
        exit(EXIT_FAILURE);
    }
    if (close(pipefds[1]) == -1)
    {
        perror("close(pipefds[1])");
        exit(EXIT_FAILURE);
    }
    /* El proceso padre espera a que terminen sus procesos hijo */
/*Hijo de la izquierda*/
    if (waitpid(pid_hijos[0], &status, 0) == -1)  
    {
        perror("wait(1)");
        exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status))
    {
        if(WEXITSTATUS(status) == 0) {
            printf(stderr, "El proceso ha terminado por la señal %d\n ", pid_hijos[0], WEXITSTATUS(status));
            exit(EXIT_SUCCESS);
        }
    }
    if(WIFSIGNALED(status)){
        perror("wait(2)");
        exit(EXIT_FAILURE);

    }

/*Hijo de la derecha*/
    if (waitpid(pid_hijos[2], &status, 0) == -1)  
    {
        perror("wait(1)");
        exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status))
    {
        if(WEXITSTATUS(status) == 0) {
            printf(stderr, "El proceso ha terminado por la señal %d\n ", pid_hijos[0], WEXITSTATUS(status));
            exit(EXIT_SUCCESS);
        }
    }
    if(WIFSIGNALED(status)){
        perror("wait(2)");
        exit(EXIT_FAILURE);

    }

    exit(EXIT_SUCCESS);
}

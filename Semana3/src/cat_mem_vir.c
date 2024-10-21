#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

/*  Ejemplo: cat_mem_vir [-o FILEOUT] FILEIN1 [FILEIN2 ... FILEINn]
    Notas:   Por defecto, se escribe en 'stdout'.
*/

void print_help(char* program_name)
{
    fprintf(stderr, "Uso: %s [-o FILEOUT] FILEIN1 [FILEIN2 ... FILEINn]\n", program_name);
}

int write_all(int fdout, char* buf, int num_read){
    ssize_t acc_written;
    acc_written = 0;
    while(acc_written < num_read){   
        ssize_t num_written = write(fdout, buf + acc_written, num_read - acc_written);

        acc_written +=num_written;
        if (num_written == -1)
        {
            return -1;
        }
    }
    return 0; 
}

void catfd(int fdin, int fdout)
{
    ssize_t num_written;
    char *fdin_addr;

    /* Calcula el tamaño del fichero de entrada */
    struct stat fdin_stat;
    if (fstat(fdin, &fdin_stat) == -1)
    {
        perror("fstat(fdin)");
        exit(EXIT_FAILURE);
    }

    /* Mapea en memoria el fichero de entrada completo */
    fdin_addr = mmap(NULL, fdin_stat.st_size /* length */, PROT_READ, MAP_PRIVATE, fdin, 0 /* offset */);
    if (fdin_addr == MAP_FAILED)
    {
        perror("mmap(fdin)");
        exit(EXIT_FAILURE);
    }
    /* El fichero se cierra porque se leerá a través del mapeo de memoria */
    if (close(fdin) == -1)
    {
        perror("close(fdin)");
        exit(EXIT_FAILURE);
    }

    /* Escribe el fichero de entrada en 'fdout' */
    num_written = write(fdout, fdin_addr, fdin_stat.st_size);
    if (num_written == -1)
    {
        perror("write(fdout)");
        exit(EXIT_FAILURE);
    }
    /* Escrituras parciales no tratadas */
    
    if(write_all(fdout, fdin_addr, fdin_stat.st_size) == -1){
        perror("munmap(fdin)");
        exit(EXIT_FAILURE);
    }
    /* Se elimina el mapeo de memoria */
    if (munmap(fdin_addr, fdin_stat.st_size) == -1)
    {
        perror("munmap(fdin)");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int opt;
    char *fileout = NULL;
    int fdout;
    int fdin;

    optind = 1;
    while ((opt = getopt(argc, argv, "o:h")) != -1)
    {
        switch (opt)
        {
        case 'o':
            fileout = optarg;
            break;
        case 'h':
            print_help(argv[0]);
            exit(EXIT_SUCCESS);
        default:
            print_help(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind == argc)
    {
        fprintf(stderr, "Uso: %s [-o FILEOUT] FILEIN1 [FILEIN2 ... FILEINn]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Abre el fichero de salida */
    if (fileout != NULL)
    {
        fdout = open(fileout, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fdout == -1)
        {
            perror("open(fileout)");
            exit(EXIT_FAILURE);
        }
    }
    else /* Por defecto, la salida estándar */
        fdout = STDOUT_FILENO;

    /* Abre cada fichero de entrada y lo escribe en 'fileout' */
    for (int i = optind; i < argc; i++)
    {
        int fdin = open(argv[i], O_RDONLY);
        if (fdin == -1)
        {
            perror("open(fdin)");
            continue;
        }
        catfd(fdin, fdout);
    }

    /* Cierra el fichero de salida */
    if (fileout != NULL)
    {
        if (close(fdout) == -1)
        {
            perror("close(fdout)");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}

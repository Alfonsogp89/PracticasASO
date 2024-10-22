#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>



bool fin_linea = false;
int pos_linea = 0;


int gesitionBuffer(char *buf, char *line, int BUF_SIZE, int pos_buffer, int long_linea, int LINE_SIZE){
    int nr = 0;
    int cont = 0;
    bool fin_linea = false;
    if(pos_buffer == BUF_SIZE-1){
        // Tenemos que empezar desde el principio del buffer
        pos_buffer = 0;
        if((nr=read(STDIN_FILENO, buf, BUF_SIZE-1)) < 0){
            perror("read");
            exit(EXIT_FAILURE);
        }
    }
    if(nr != 0){
        // Analizar el buffer 
        bool sig_linea = false;
        while((pos_buffer + cont) < pos_buffer+nr && sig_linea == false && fin_linea == false){
            if(buf[pos_buffer] == '\n'){
                sig_linea = true;
            }else if (buf[pos_buffer] == '\0'){
                fin_linea = true;
            }
            cont ++;
        }
        //Escribimos en line hasta donde encontramos el \n o donde termina el buffer
        if(cont + long_linea < LINE_SIZE){
            write(atoi(line), buf+pos_buffer, cont+pos_buffer);
        }else{
            cont = -1;
        }
    }else{
        cont = 0;
    }
    //Si no hay nada que leer, read devuelve 0 y se termina el programa
    // Si devolvemos -1 nos pasamos del líte
    return cont;
}

bool analizarLinea(char *line, char *buf, int LINE_SIZE, int BUF_SIZE){
    int long_linea = 0;
    int pos_buffer = 0;
    int nr = 0;
    int nw = 0;
    int pos_linea = 0;

    while(line[pos_linea] != '\n' && fin_linea == false && line[pos_linea] != '\0'){
        //Si en la posción actual no hemos llegado al final de la línea,
        //Tendremos que leer del buffer
        if(pos_linea < LINE_SIZE-1 && line[pos_linea] == NULL){
            if((nr = gesitionBuffer(buf, line, BUF_SIZE, pos_buffer, long_linea, LINE_SIZE)) == 0){
                fin_linea = true;
            //Si la línea es demasiado larga, terminamos el programa
            }else if (nr == -1){
                perror("Error: La línea es demasiado larga");
                exit(EXIT_FAILURE);
            }
            // Actualizamo hasta donde hemos llegado en la línea
            long_linea = long_linea + nr;
        }
        pos_linea++;
        pos_buffer = (pos_buffer + nr) % BUF_SIZE;
    }
    if(fin_linea == true && pos_linea < LINE_SIZE-1){
        return true;
    }else if(pos_linea < LINE_SIZE-1){
        return true;
    }else{
        return false;
    }

}



int main(int argc, char const *argv[])
{


    const int MIN_BUF_SIZE = 1;
    const int MAX_BUF_SIZE = 1024;
    const int MIN_LINE_SIZE = 16;
    const int MAX_LINE_SIZE = 128;
    //Valores predeterminado de reservas de buffer y line
    int BUF_SIZE = 32;
    int LINE_SIZE = 16;
    //Valores predeterminado de reservas de buffer y line
    //char *argumentos[];
    //Necesito leer los argumentos y pasarlos a la función execv
    int pid_hijos[3];
    char *buf = NULL;
    char *line = NULL;
    int opt;
    int optind = 1;

    while ((opt = getopt(argc, argv, "b:l:h")) != -1){
        switch (opt)
        {
        case 'b':
            /*optarg: argumento de la opción*/
            buf = optarg;
            if (buf == NULL) {
                perror("Error: No argument provided for option -b\n");
                exit(EXIT_FAILURE);
            }else if (atoi(buf) < MIN_BUF_SIZE || atoi(buf) > MAX_BUF_SIZE) {
                perror("-b BUF_SIZE Tamaño del buffer de entrada 1<=BUF_SIZE<=8192 -l MAX_LINE_SIZE Tamaño máximo de línea 16<=MAX_LINE_SIZE<=1024");
                exit(EXIT_FAILURE);            
            } else {
                BUF_SIZE = atoi(buf);
            }
            break;
        case 'l':
            line = optarg;
            if (line == NULL) {
                perror("Error: No argument provided for option -l\n");
                exit(EXIT_FAILURE);
            } else if (atoi(line) < MIN_LINE_SIZE || atoi(line) > MAX_LINE_SIZE) {
                perror("-b BUF_SIZE Tamaño del buffer de entrada 1<=BUF_SIZE<=8192 -l MAX_LINE_SIZE Tamaño máximo de línea 16<=MAX_LINE_SIZE<=1024");
                exit(EXIT_FAILURE);            
            }else{
                LINE_SIZE = atoi(line);
            }
            break;
        case 'h':
            printf("Uso: ./exec_lines [-b BUF_SIZE] [-l MAX_LINE_SIZE] "
            "Lee de la entrada estándar una secuencia de líneas conteniendo órdenes para ser ejecutadas y lanza los procesos necesarios para ejecutar cada línea, esperando a su terminación para ejecutar la siguiente. -b BUF_SIZE"
            "Tamaño del buffer de entrada 1<=BUF_SIZE<=8192 -l MAX_LINE_SIZE"
            "Tamaño máximo de línea 16<=MAX_LINE_SIZE<=1024");
            exit(EXIT_SUCCESS);
        default:
            
        }
    }
    // Reservar memoria buffer
    if((buf = (char*)malloc(BUF_SIZE * sizeof(char))) == NULL){
        perror("malloc(buffer)");
        exit(EXIT_FAILURE);
    }
    //Reservar memoria para line
    if((line = (char*)malloc(LINE_SIZE * sizeof(char))) == NULL){
        perror("malloc(line)");
        exit(EXIT_FAILURE);
    }


    int linea = 1;
    while(fin_linea == false){
        if(analizarLinea(line, buf, LINE_SIZE, BUF_SIZE)){
            //ejecutarInstrucción();
            printf("Contenido de la línea %d: %s\n", linea, line);
        }
        linea++;
    }




    free(buf);
    free(line); 

    return 0;
}


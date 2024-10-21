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

/*
 1. Analizar flags de exec_lines con getop
 2. Reservar memoria para el buffer y la línea
 
 4. Almacenar linea hasta llegar a \n
 5. Si completamos la linea y no supera el tamaño, llamamos a ejecuta instrucción

 
*/

char *BUFFER = NULL;
char *LINE = NULL;
bool fin = false;
int pos_buffer = 0;

//Valores predeterminado de reservas de buffer y line
int LINE_SIZE = 32; 
int BUF_SIZE = 16;


bool analizarLinea();
int getSiguiente();
void ejecutarInstrucción();

int main(int argc, char *argv[]){
    //Valores máxmimos y mínimos de buffer y line
    const int MIN_BUF_SIZE = 1;
    const int MAX_BUF_SIZE = 1024;
    const int MIN_LINE_SIZE = 16;
    const int MAX_LINE_SIZE = 128;
    //Valores predeterminado de reservas de buffer y line
 
    //char *argumentos[];
    //Necesito leer los argumentos y pasarlos a la función execv
    int pid_hijos[3];
    char *buf = NULL;
    char *line = NULL;
    int opt;
    optind = 1;

    /*Comprobación de los argumentos */
    // Recorro la línea para optener los posibles flagsflags 
    while ((opt = getopt(argc, argv, "b:l:h")) != -1)
    {
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

    // Reservar memoria buffer y line
    if ((BUFFER = (char *) malloc(BUF_SIZE * sizeof(char))) == NULL){
        perror("malloc(buffer)");
        exit(EXIT_FAILURE);
    }
    if ((LINE = (char *)malloc(LINE_SIZE *sizeof(char))) == NULL){
        perror("malloc(line)");
        exit(EXIT_FAILURE);
    }

    //Bucle que almacene la salida estandar en la línea 
    int num_linea = 1; //Llevará la cuenta de las lineas que hemos analizado     
    
    while(fin == false){
        if(analizarLinea() == true){
            //ejecutarInstrucción();
            printf("Contenido de la línea %d: %s\n", num_linea, LINE);
        }else{
            printf("Error de la linea %d. %s\n", num_linea, LINE);
        }
        num_linea++;
    }
}

//Si devuelve verdadero, se mandará a ejecutar la linea con ejecutarLinea

bool analizarLinea(){
    char * line_aux = malloc(LINE_SIZE * sizeof(char));
    int tam_linea = 0;  //Longitud de line_aux
    bool sig_linea = false;
    
    while(tam_linea < LINE_SIZE-1 && sig_linea == false){
        int leido_buffer = getSiguiente();
        // Si no hay nada que leer es que hemos terminado 
        if(leido_buffer == 0){
            sig_linea = true; 
        }else{
            //Leeré(desde donde leí la última vez hasta donde me devuleve getSig) de BUFFER y lo almacenaré en line_aux
            int nr = read(*BUFFER, line_aux + tam_linea, tam_linea + leido_buffer);
            if(nr < 0){
                perror("read()");
                exit(EXIT_FAILURE);
            }
                //Analizar lo último leido de la línea para comprobar si hay un '\n'
                int cont = tam_linea;
                while(sig_linea != false || tam_linea < (tam_linea+nr)){
                    if(line_aux[cont] == '\n'){ // Comprobamos si hemos llegado al final de la linea
                        sig_linea == true;
                    }
                    cont ++;                     //Actualizo el contador 
                }
            
            tam_linea += leido_buffer;      //Actualizo lo leido 
        }
        
    }

    //copio line_aux en LINE
    //Libero line aux y retorno true
    if(tam_linea < LINE_SIZE-1){
            if (strncpy(LINE, line_aux, tam_linea) == NULL) {
                perror("strncpy");
                free(line_aux);
                exit(EXIT_FAILURE);
            }
    // Tendré que copiar hasta el límite de la linea ya que me he pasado del límite   
    }else{
        if (strncpy(LINE, line_aux, LINE_SIZE-1) == NULL) {
            perror("strncpy");
            free(line_aux);
            exit(EXIT_FAILURE);
        }
        sig_linea = false;    
    }
    free(line_aux);
    return true;
    
}


int getSiguiente(){
    bool sig_linea = false;
    int nr = 0;
    //Si nos encontramos al final del buffer, borramos su contenido y leemos de el
    if(pos_buffer == BUF_SIZE-1){
        memset(BUFFER, 0, BUF_SIZE-1);
        pos_buffer = 0;
        nr = read(STDIN_FILENO, BUFFER, BUF_SIZE-1);    //nr = numero leido de bits
        if (nr < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        //Si no tiene nada que leer significa que hemos terminado 
        }else if(nr == 0){
            fin = true;
        }else{
            //Analizo BUFFER buscando un \n, Si lo encuentor saldré del bucle y devolveré a AnalizarLinea hasta donde
            // tiene que leer de BUFFER para meterlo en la línea 
            while(pos_buffer < pos_buffer+nr && sig_linea == false){
                if(BUFFER[pos_buffer] == '\n'){
                    sig_linea = true;
                }
                pos_buffer++;
            }
            nr = nr - (nr-pos_buffer);
        }
    }else{
        nr = read(STDERR_FILENO, BUFFER + pos_buffer, BUF_SIZE-1);
        if (nr < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }else if(nr == 0){
            fin = true;
        }else{
            
            while(pos_buffer < pos_buffer+nr && sig_linea == false){
                if(BUFFER[pos_buffer] == '\n'){
                    sig_linea = true;
                }
                pos_buffer++;
            }
            nr = nr - (nr-pos_buffer); 
        }
    }
    return nr;
    // Despues de controlar escritura del buffer, leerlo y comprobar si no hay \n
    //      Si lo hay, devolver hasta la posición donde está \n
    // Si no hay más líneas, activar variable fin a verdadero y retornar hasta donde se haya llegado

}


void ejecutarInstrucción(){
    char * simbolos[] = {"<",">>",">","|"};
    for (int i = 0; i < sizeof(simbolos) / sizeof(simbolos[0]); i++) {
        if (strstr(LINE, simbolos[i]) != NULL) {
            
        }
    }

}

//Antes de ejecutar una instrucción debemos quitar los espacios contiguos
void removeExtraSpaces() {
    int i = 0, j = 0;
    int length = strlen(LINE);
    int space = 0;

    while (i < length) {
        if (LINE[i] != ' ') {
            LINE[j++] = LINE[i];
            space = 0;
        } else if (space == 0) {
            LINE[j++] = LINE[i];
            space = 1;
        }
        i++;
    }
    LINE[j] = '\0';
}

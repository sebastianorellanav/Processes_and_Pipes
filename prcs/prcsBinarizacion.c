#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <jpeglib.h>
#include "../incl/binarizacion.h"
#include "../incl/lecturaImagenes.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char *argv[])
{
    printf("Aqui inicia el proceso de Binarizacion\n");
    //Se crean las variables para guardar los datos leidos del pipe45
    int umbralBin = 0;
    int umbralNeg = 0;
    int flagResultados = 0;
    int numImagen = 0;
    int height = 0;
    int width = 0;
    int lenImagen = 0;
    JpegData jpegData;

    printf("Se va a comenzar a leer el pipe en el proceso binarizacion\n");
    //Se leen los datos del pipe
    read(STDIN_FILENO, &umbralBin, sizeof(int));
    read(STDIN_FILENO, &umbralNeg, sizeof(int));
    read(STDIN_FILENO, &flagResultados, sizeof(int));
    read(STDIN_FILENO, &numImagen, sizeof(int));
    read(STDIN_FILENO, &height, sizeof(int));
    read(STDIN_FILENO, &width, sizeof(int));
    lenImagen = height * width;
    jpegData.height = height;
    jpegData.width = width;
    jpegData.ch = 1;
    alloc_jpeg(&jpegData);
	for (int i = 0; i < lenImagen; i++)
    {
        read(STDIN_FILENO, &(jpegData.data[i]), sizeof(uint8_t));
    }
    printf("Se leyo correctamente el pipe en el proceso binarizacion\n");
    //-----------------------------------------------------
    //Se Binariza la imagen
    jpegData = binarizarImagen(jpegData, umbralBin);
    printf("Se binariza Correctamente la imagen\n");

    //---------------------------------------------------------
    //Se crea un nuevo pipe y un nuevo proceso
    int pipe56[2];
    int status;
    pid_t pid;
    pipe(pipe56);

    pid = fork();
    if(pid < 0) //No se pudo crear el proceso hijo
    {
        fprintf(stderr, "No se pudo crear el proceso de Clasificacion\n");
        return 1;
    }

    else if (pid > 0) //es el padre
    {
        close(pipe56[LECTURA]);
        //Se escribe en el pipe56 los datos para enviar al hijo
        write(pipe56[ESCRITURA], &umbralNeg, sizeof(int));
        write(pipe56[ESCRITURA], &flagResultados, sizeof(int));
        write(pipe56[ESCRITURA], &numImagen, sizeof(int));
        write(pipe56[ESCRITURA], &height, sizeof(int));
        write(pipe56[ESCRITURA], &width, sizeof(int));
        for (int i = 0; i < lenImagen; i++)
        {
           write(pipe56[ESCRITURA], &(jpegData.data[i]), sizeof(uint8_t));
        } 
        
        //Se espera a que el hijo termine su ejecucion
        waitpid(pid, &status, 0);
    }

    else //es el hijo
    {
        close(pipe56[ESCRITURA]);
        dup2(pipe56[LECTURA], STDIN_FILENO);

        //Se cambia el codigo del hijo
        char *args[]={"./pClasificacion",NULL}; 
        execv(args[0],args);
    }

    liberarJpeg(&jpegData);
    printf("El proceso de Binarización Termina su ejecucion\n");
    return 1;
}
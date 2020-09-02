#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <jpeglib.h>
#include "../incl/lecturaImagenes.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char *argv[])
{
    //Se crean variables para obtener los datos del pipe12 proveniente del main
    int umbralBin = 0;
    int umbralNeg = 0;
    int flagResultados = 0;
    int numImagen = 0;
    int lenNombreMasc = 0;

    //Se leen los datos del pipe12 proviente del main
    read(STDIN_FILENO, &umbralBin, sizeof(int));
    read(STDIN_FILENO, &umbralNeg, sizeof(int));
    read(STDIN_FILENO, &flagResultados, sizeof(int));
    read(STDIN_FILENO, &numImagen, sizeof(int));
    read(STDIN_FILENO, &lenNombreMasc, sizeof(int));
    char nombreArchivoMasc[lenNombreMasc];
    read(STDIN_FILENO, nombreArchivoMasc, lenNombreMasc);

    //------------------------------------------------------
    //Se lee la imagen haciendo uso de la función leerImagenes
    char filename[30] = "";
    sprintf(filename,"./imagen_%i.jpg",numImagen);
    JpegData jpegData = leerImagenes(filename);
    int height = jpegData.height;
    int width = jpegData.width;
    int lenImagen = 3*height*width; //Se obtiene largo de la imagen

    //------------------------------------------------------
    //Crear nuevo pipe y nuevo proceso
    int pipe23[2];  //Se declara pipe 23 
    pipe(pipe23);   //Del pipe 2 al pipe 3
    int status;
    pid_t pid;

    pid = fork();
    if(pid < 0) //No se pudo crear el proceso
    {
        fprintf(stderr, "No se pudo crear el proceso de Conversion\n");
        return 1;
    }

    else if (pid > 0) //Es el padre
    {
        close(pipe23[LECTURA]);
        //Se escriben las variables en el pipe para enviarlas al proceso hijo
        write(pipe23[ESCRITURA], &umbralBin, sizeof(int));
        write(pipe23[ESCRITURA], &umbralNeg, sizeof(int));
        write(pipe23[ESCRITURA], &flagResultados, sizeof(int));
        write(pipe23[ESCRITURA], &numImagen, sizeof(int));
        write(pipe23[ESCRITURA], &lenNombreMasc, sizeof(int));
        write(pipe23[ESCRITURA], &(jpegData.height), sizeof(int));
        write(pipe23[ESCRITURA], &(jpegData.width), sizeof(int));

        //Se entrega cada bit de la imagen al pipe23
        for (int i = 0; i < lenImagen; i++)
        {
            write(pipe23[ESCRITURA], &(jpegData.data[i]), sizeof(uint8_t));
        }
        
        write(pipe23[ESCRITURA], nombreArchivoMasc, lenNombreMasc*sizeof(char));

        //Espera a que el hijo termine su ejecución
        waitpid(pid, &status, 0);
    }

    else //es el hijo
    {
        //Ingresa al hijo. Hijo del prcsLectura -> pConversion
        close(pipe23[ESCRITURA]);
        dup2(pipe23[LECTURA], STDIN_FILENO);
        
        //se cambia el codigo del proceso hijo
        char *args[]={"./pConversion",NULL}; 
        execv(args[0],args);
    }

    liberarJpeg(&jpegData);
    printf("El proceso de Lectura finaliza tu ejecucion\n");
    return 1;
    
}
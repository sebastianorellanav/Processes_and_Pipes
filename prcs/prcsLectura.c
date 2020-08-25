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
	printf("Este es el proceso de lectura\n");
    //Crear variables para almacenar los datos del pipe12
    int umbralBin = 0;
    int umbralNeg = 0;
    int flagResultados = 0;
    int numImagen = 0;
    int lenNombreMasc = 0;

    //Leer datos del pipe12
    read(STDIN_FILENO, &umbralBin, sizeof(int));
    read(STDIN_FILENO, &umbralNeg, sizeof(int));
    read(STDIN_FILENO, &flagResultados, sizeof(int));
    read(STDIN_FILENO, &numImagen, sizeof(int));
    read(STDIN_FILENO, &lenNombreMasc, sizeof(int));
    char nombreArchivoMasc[lenNombreMasc];
    read(STDIN_FILENO, nombreArchivoMasc, lenNombreMasc);

    //------------------------------------------------------
    //Leer la imagen
    char filename[30] = "";
    sprintf(filename,"./imagen_%i.jpg",numImagen);
    JpegData jpegData = leerImagenes(filename);
    int height = jpegData.height;
    int width = jpegData.width;
    int lenImagen = 3*height*width;
	printf("antes de crear el array\n");
	uint8_t dataImagen[lenImagen];
	printf("si llega hasta aqui se pudo asignar la memoria\n");
	for (int i = 0; i < lenImagen; i++)
	{
		dataImagen[i] = jpegData.data[i];
		printf("%d",jpegData.data[i]);
	}
	printf("\n\n\n");
	

    //------------------------------------------------------
    //Crear nuevo pipe y nuevo proceso
    int pipe23[2];
    pipe(pipe23);
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
        //Escribo en el pipe los datos para enviar al proceso hijo
        write(pipe23[ESCRITURA], &umbralBin, sizeof(int));
        write(pipe23[ESCRITURA], &umbralNeg, sizeof(int));
        write(pipe23[ESCRITURA], &flagResultados, sizeof(int));
        write(pipe23[ESCRITURA], &numImagen, sizeof(int));
        write(pipe23[ESCRITURA], &lenNombreMasc, sizeof(int));
        write(pipe23[ESCRITURA], &(jpegData.height), sizeof(int));
        write(pipe23[ESCRITURA], &(jpegData.width), sizeof(int));
        write(pipe23[ESCRITURA], dataImagen, sizeof(uint8_t)*lenImagen);
        write(pipe23[ESCRITURA], nombreArchivoMasc, lenNombreMasc*sizeof(char));

        //Espera a que el hijo termine su ejecución
        waitpid(pid, &status, 0);
    }

    else //es el hijo
    {
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
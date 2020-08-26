#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <jpeglib.h>
#include "../incl/clasificacion.h"
#include "../incl/lecturaImagenes.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char *argv[])
{
    printf("Aqui inicia el proceso de Clasificacion\n");
    //se crean las variables para guardar los datos leidos del pipe
    int umbralNeg = 0;
    int flagResultados = 0;
    int numImagen = 0;
    int height = 0;
    int width = 0;
    int lenImagen = 0;
    JpegData jpegData;

    printf("Se va a comenzar a leer el pipe en el proceso clasificacion\n");
    //Se leen los datos del pipe56
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

    printf("Se leyo correctamente el pipe en el proceso de clasificacion\n");

    //-----------------------------------------------------------
    //Se clasifica la imagen
    char imagename[30] = "";
    sprintf(imagename, "imagen_%i",numImagen);
    char *nearlyBlack = analisisDePropiedad(jpegData, umbralNeg);
    if(flagResultados){
		if(nearlyBlack[0] == 'n'){
			printf("|          %s       |             %s           |\n", imagename, nearlyBlack);
		}
		else{
			printf("|          %s       |             %s          |\n", imagename, nearlyBlack);
		}
	}
    printf("Se clasifico correctamente la imagen\n");
    //-------------------------------------------------------------------
    //Se crea un nuevo pipe y un nuevo proceso
    int pipe67[2];
    int status;
    pid_t pid;
    pipe(pipe67);

    pid = fork();
    if(pid < 0) //No se pudo crear el proceso
    {
        fprintf(stderr, "No se pudo crear el proceso de Escritura\n");
        return 1;
    }

    else if (pid > 0) //es el padre
    {
        close(pipe67[LECTURA]);
        //Se escriben los datos en el pipe67
        write(pipe67[ESCRITURA], &numImagen, sizeof(int));
        write(pipe67[ESCRITURA], &height, sizeof(int));
        write(pipe67[ESCRITURA], &width, sizeof(int));
        for (int i = 0; i < lenImagen; i++)
        {
            write(pipe67[ESCRITURA], &(jpegData.data[i]), sizeof(uint8_t));
        }
        

        //Se espera al hijo
        waitpid(pid, &status, 0);
    }

    else //es el hijo
    {
        close(pipe67[ESCRITURA]);
        dup2(pipe67[LECTURA], STDIN_FILENO);

        //Se cambia el codigo del hijo
        char *args[]={"./pEscritura",NULL}; 
        execv(args[0],args);
    }
    
    liberarJpeg(&jpegData);
    printf("El proceso de Clasificación termina su ejecucion\n");
    return 1;
    
}
#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <jpeglib.h>
#include "../incl/conversion.h"
#include "../incl/lecturaImagenes.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char *argv[])
{
	printf("Aqui inicia el proceso de conversion\n");
    //Crear variables para guardar datos leidos del pipe
    int umbralBin = 0;
    int umbralNeg = 0;
    int flagResultados = 0;
    int numImagen = 0;
    int lenNombreMasc = 0;
    int height = 0;
    int width = 0;
    int lenImagen = 0;
    JpegData jpegData;

    printf("Se va a comenzar a leer el pipe en el proceso Conversion\n");
    //Leer datos del pipe23
    read(STDIN_FILENO, &umbralBin, sizeof(int));
    read(STDIN_FILENO, &umbralNeg, sizeof(int));
    read(STDIN_FILENO, &flagResultados, sizeof(int));
    read(STDIN_FILENO, &numImagen, sizeof(int));
    read(STDIN_FILENO, &lenNombreMasc, sizeof(int));
    read(STDIN_FILENO, &height, sizeof(int));
    read(STDIN_FILENO, &width, sizeof(int));
    lenImagen = 3 * height * width;
    jpegData.height = height;
    jpegData.width = width;
    jpegData.ch = 3;
    alloc_jpeg(&jpegData);
	for (int i = 0; i < lenImagen; i++)
    {
       read(STDIN_FILENO, &(jpegData.data[i]), sizeof(uint8_t));
    }
    char nombreArchivoMasc[lenNombreMasc];
    read(STDIN_FILENO, nombreArchivoMasc, lenNombreMasc*sizeof(char));

	//for (int i = 0; i < lenImagen; i++)
	//{
	//	printf("%d",jpegData.data[i]);
	//}
	
    printf("Se leyo correctamente el pipe del proceso conversion\n");
    //-----------------------------------------------------------
    //Convertir Imagen a Blanco y Negro
    jpegData = convertirAEscalaGrises(jpegData);
    printf("Se convierte la imagen exitosamente\n");

	//-----------------------------------------------------------------
	
	lenImagen = jpegData.width*jpegData.height;	

    //-----------------------------------------------------------------
    //Se crea un nuevo pipe y un nuevo proceso
    int pipe34[2];
    int status;
    pid_t pid;
    pipe(pipe34);

    pid = fork();
    if(pid < 0) //No se pudo crear el proceso
    {
        fprintf(stderr, "No se pudo crear el proceso de Filtro\n");
        return 1;
    }

    else if (pid > 0) //es el padre
    {
        close(pipe34[LECTURA]);
        //Se escriben los datos para enviar por el pipe34
        write(pipe34[ESCRITURA], &umbralBin, sizeof(int));
        write(pipe34[ESCRITURA], &umbralNeg, sizeof(int));
        write(pipe34[ESCRITURA], &flagResultados, sizeof(int));
        write(pipe34[ESCRITURA], &numImagen, sizeof(int));
        write(pipe34[ESCRITURA], &lenNombreMasc, sizeof(int));
        write(pipe34[ESCRITURA], &height, sizeof(int));
        write(pipe34[ESCRITURA], &width, sizeof(int));
        for (int i = 0; i < lenImagen; i++)
        {
            write(pipe34[ESCRITURA], &(jpegData.data[i]), sizeof(uint8_t));
        }
        
        write(pipe34[ESCRITURA], nombreArchivoMasc, lenNombreMasc*sizeof(char));

        //Se espera a que el proceso hijo termine su ejecucion
        waitpid(pid, &status, 0);
    }


    else //es el hijo
    {
        close(pipe34[ESCRITURA]);
        dup2(pipe34[LECTURA], STDIN_FILENO);
        //Se cambia el codigo del proceso hijo
        char *args[]={"./pFiltro",NULL}; 
        execv(args[0],args);
    }    

    liberarJpeg(&jpegData);
    printf("El proceso de Conversion finaliza su ejecución\n");
    return 1;

}
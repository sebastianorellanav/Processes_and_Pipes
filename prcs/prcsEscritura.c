#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <jpeglib.h>
#include "../incl/escrituraImagenes.h"
#include "../incl/lecturaImagenes.h"

#define LECTURA 0
#define ESCRITURA 1
int main(int argc, char *argv[])
{
    printf("Aqui inicia el proceso de Escritura\n");
    //se crean las variables para guardar los datos leidos del pipe67
    int numImagen = 0;
    int height = 0;
    int width = 0;
    int lenImagen = 0;
    JpegData jpegData;

    printf("Se va a leer el pipe en el proceso de Escritura\n");
    //Se leen los datos del pipe67
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

    printf("Se leyó correctamente el pipe en el proceso de Escritura\n");

    //------------------------------------------------------------------
    //Se escribe la imagen
    char fileout[30];
	sprintf(fileout,"./out_%i.jpg",numImagen);
    escribirImagenes(jpegData, "escalagrises",fileout);

    liberarJpeg(&jpegData);
    
    printf("El proceso de Escritura finaliza su ejecucion\n");
    return 1; 
    
    
}
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

int main(int argc, char *argv[]) {
	printf("soy el proceso hijo que escribe la imagen la imagen\n");
    //*******************************************************************************//
    //Leer mensajes del pipe del proceso padre
    
    int numImagen;
    pid_t pid;
    JpegData nueva;
    printf("antes de leer del pipe\n");
	read(STDIN_FILENO, &nueva, sizeof(JpegData));
    printf("lee la structura\n");
	int len = nueva.height*nueva.width*nueva.ch;
	alloc_jpeg(&nueva);
	read(STDIN_FILENO, nueva.data, sizeof(uint8_t *)*len);
    printf("lee el puntero de datos\n");
    read(STDIN_FILENO, &numImagen, sizeof(int));

    printf("lee todas las cosas del pipe\n");
    //*******************************************************************************//
    //6. Escribir imagen
	char fileout[30];
	sprintf(fileout,"out_%i.jpg",numImagen);
    printf("crea el string del nombre de la imagn a guaradar: %s\n",fileout);
	escribirImagenes(nueva, "escalagrises",fileout);
    printf("Se escribe la imagen correctamente\n");
    
    //7. liberar memoria
	liberarJpeg(&nueva);
    
    printf("Aqui termina la cadena de procesos y vuelve al proceso padre a ver si hay mas imagenes");
    return 0; 
}
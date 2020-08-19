#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <jpeglib.h>
#include "../incl/filtro.h"
#include "../incl/lecturaImagenes.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char *argv[]) {
	printf("soy el proceso hijo que filtra la imagen\n");
    //*******************************************************************************//
    //Leer mensajes del pipe del proceso padre
    int umbralBin;
	int umbralNeg;
	int flagResultados;
	int numImagen;
	int lenNombreMasc;
	char nombreArchivoMasc[lenNombreMasc];
	char imagename[30];
    pid_t pid;
	JpegData nueva;

    read(STDIN_FILENO, &umbralBin, sizeof(int));
    read(STDIN_FILENO, &umbralNeg, sizeof(int));
    read(STDIN_FILENO, &flagResultados, sizeof(int));
    read(STDIN_FILENO, &lenNombreMasc, sizeof(int));
    read(STDIN_FILENO, nombreArchivoMasc, lenNombreMasc*sizeof(char));
	read(STDIN_FILENO, imagename, 30*sizeof(char));
	read(STDIN_FILENO, &nueva, sizeof(JpegData));
	int len = nueva.height*nueva.width*nueva.ch;
	alloc_jpeg(&nueva);
	read(STDIN_FILENO, nueva.data, sizeof(uint8_t *)*len);
	read(STDIN_FILENO, &numImagen, sizeof(int));


    //*******************************************************************************//
    //3. aplicar filtro laplaciano
	printf("nombre mascara= %s\n",nombreArchivoMasc);
    int **mascara = leerMascara(nombreArchivoMasc);
	printf("lee la mascara existosamente\n");
	nueva = aplicarFiltroLaplaciano(nueva,mascara); 
	printf("El filtro se aplicó correctamente\n");


    //*******************************************************************************//
    //Crear nuevo pipe para enviar mensajes al proceso hijo
    int *pipe4 = (int*)malloc(sizeof(int)*2); //se reserva memoria para el pipe
	pipe(pipe4); //inicializa el pipe
    int status;


    //*******************************************************************************//
    //Crear proceso hijo
    pid = fork(); 
		if(pid < 0){
			fprintf(stderr, "No se pudo crear el proceso hijo" ); 
        	return 1;
		}

		if(pid > 0){ //Es el padre

			close(pipe4[LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
			write(pipe4[ESCRITURA], &umbralBin, sizeof(int));
        	write(pipe4[ESCRITURA], &umbralNeg, sizeof(int));
			write(pipe4[ESCRITURA], &flagResultados, sizeof(int));
			write(pipe4[ESCRITURA], imagename, 30*sizeof(char));
            write(pipe4[ESCRITURA], &nueva, sizeof(JpegData));
			write(pipe4[ESCRITURA], nueva.data, sizeof(uint8_t*)*len);
			write(pipe4[ESCRITURA], &numImagen, sizeof(int));
			printf("al parecer soy el padre y mi pid es: %i\n" , getpid());
        	printf("Ya escribi el arr en el pipe\n");
			waitpid(pid, &status,0);
		}
		else{ //Es el hijo
            printf("Soy el hijo del filtro\n");
			close(pipe4[ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
			dup2(pipe4[LECTURA], STDIN_FILENO);
			char *args[]={"./prcsBinarizacion",NULL}; 
        	execv(args[0],args);
		}
    return 0; 
}
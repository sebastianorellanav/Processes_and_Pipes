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

int main(int argc, char *argv[]) {
	printf("soy el proceso hijo que lee la imagen\n");
    int umbralBin;
	int umbralNeg;
	int flagResultados;
	int numImagen;
	char *nombreArchivoMasc = NULL;
    char filename[30];
	char imagename[30];
    pid_t pid;

    read(STDIN_FILENO, &umbralBin, sizeof(int));
    read(STDIN_FILENO, &umbralNeg, sizeof(int));
    read(STDIN_FILENO, &flagResultados, sizeof(int));
    read(STDIN_FILENO, nombreArchivoMasc, strlen(nombreArchivoMasc)+1);
	read(STDIN_FILENO, imagename, 30);
	read(STDIN_FILENO, filename, 30);
	read(STDIN_FILENO, &numImagen, sizeof(int));

    //1. Leer la imagen
    JpegData jpegData = leerImagenes(filename);

    int *pipe2 = (int*)malloc(sizeof(int)*2); //se reserva memoria para el pipe
	pipe(pipe2); //inicializa el pipe
    int status;

    pid = fork(); 
		if(pid < 0){
			fprintf(stderr, "No se pudo crear el proceso hijo" ); 
        	return 1;
		}

		if(pid > 0){ //Es el padre

			close(pipe2[LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
			write(pipe2[ESCRITURA], &umbralBin, sizeof(int));
        	write(pipe2[ESCRITURA], &umbralNeg, sizeof(int));
			write(pipe2[ESCRITURA], &flagResultados, sizeof(int));
			write(pipe2[ESCRITURA], nombreArchivoMasc, strlen(nombreArchivoMasc)+1);
			write(pipe2[ESCRITURA], imagename, 30);
            write(pipe2[ESCRITURA], &jpegData, sizeof(JpegData));
			write(pipe2[ESCRITURA], &numImagen, sizeof(int));
			printf("al parecer soy el padre y mi pid es: %i\n" , getpid());
        	printf("Ya escribi el arr en el pipe\n");
			waitpid(pid, &status,0);
		}
		else{ //Es el hijo
            printf("Soy el hijo de la lectura");
			close(pipe2[ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
			dup2(pipe2[LECTURA], STDIN_FILENO);
			execl("/prcs/prcsConversion", "ls","-al", NULL);
		}
    return 0; 
}
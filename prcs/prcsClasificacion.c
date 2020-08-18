#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <jpeglib.h>
#include "../incl/clasificacion.h"

#define LECTURA 0
#define ESCRITURA 1

int main(int argc, char *argv[]) {
	printf("soy el proceso hijo que clasifica la imagen\n");
    //*******************************************************************************//
    //Leer mensajes del pipe del proceso padre
	int umbralNeg;
	int flagResultados;
    int numImagen;
	char imagename[30];
    JpegData jpegData;
    pid_t pid;

    read(STDIN_FILENO, &umbralNeg, sizeof(int));
    read(STDIN_FILENO, &flagResultados, sizeof(int));
	read(STDIN_FILENO, imagename, 30);
	read(STDIN_FILENO, &jpegData, sizeof(JpegData));
    read(STDIN_FILENO, &numImagen, sizeof(int));


    //*******************************************************************************//
    //5. Clasificar imagen
	char *nearlyBlack = analisisDePropiedad(jpegData, umbralNeg);
    if(flagResultados){
		if(nearlyBlack[0] == 'n'){
			printf("|          %s       |             %s           |\n", imagename, nearlyBlack);
		}
		else{
			printf("|          %s       |             %s          |\n", imagename, nearlyBlack);
		}
	}


    //*******************************************************************************//
    //Crear nuevo pipe para enviar mensajes al proceso hijo
    int *pipe6 = (int*)malloc(sizeof(int)*2); //se reserva memoria para el pipe
	pipe(pipe6); //inicializa el pipe
    int status;


    //*******************************************************************************//
    //Crear proceso hijo
    pid = fork(); 
		if(pid < 0){
			fprintf(stderr, "No se pudo crear el proceso hijo" ); 
        	return 1;
		}

		if(pid > 0){ //Es el padre

			close(pipe6[LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
            write(pipe6[ESCRITURA], &jpegData, sizeof(JpegData));
            write(pipe6[ESCRITURA], &numImagen, sizeof(int));
			printf("al parecer soy el padre y mi pid es: %i\n" , getpid());
        	printf("Ya escribi el arr en el pipe\n");
			waitpid(pid, &status,0);
		}
		else{ //Es el hijo
            printf("Soy el hijo de la clasificacion");
			close(pipe6[ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
			dup2(pipe6[LECTURA], STDIN_FILENO);
			execl("/prcs/prcsEscritura", "ls","-al", NULL);
		}
    return 0; 
}
//Directivas de Preprocesamiento
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <jpeglib.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include "../incl/lecturaImagenes.h"
#include "../incl/escrituraImagenes.h"
#include "../incl/binarizacion.h"
#include "../incl/clasificacion.h"
#include "../incl/conversion.h"
#include "../incl/filtro.h"

#define LECTURA 0
#define ESCRITURA 1

//Funcion Main
int main (int argc, char **argv)
{
	//Inicialización de Variables
	int cantImagenes = 0;
	int umbralBin = 0;
	int umbralNeg = 0;
	int flagResultados = 0;
	char *nombreArchivoMasc = NULL;
	int index;
	int c;

	opterr = 0;

	//el siguiente ciclo se utiliza para recibir los parametros de entrada usando getopt
	while ((c = getopt (argc, argv, "c:u:m:n:b")) != -1)
		switch (c)
			{
			case 'c':
				sscanf(optarg, "%d", &cantImagenes);
				break;
			case 'u':
				sscanf(optarg, "%d", &umbralBin);
				break;
			case 'n':
				sscanf(optarg, "%d", &umbralNeg);
				break;
			case 'm':
				nombreArchivoMasc = optarg;
				break;
			case 'b':
				flagResultados = 1;
				break;
			case '?':
				if (optopt == 'c')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
				fprintf (stderr,
						"Unknown option character `\\x%x'.\n",
						optopt);
				return 1;
			default:
				abort ();
			}

	if(flagResultados){
		printf("|          image          |       nearly black       |\n");
		printf("|-------------------------|--------------------------|\n");
	}

	// Para cada imagen
	for (int i = 1; i <= cantImagenes; i++)
	{
		int *pipes = (int*)malloc(sizeof(int)*2); //se reserva memoria para el pipe
		pipe(pipes); //inicializa el pipe
		int status;
    	pid_t pid;
		char filename[30];
		char imagename[30];
		sprintf(filename,"./imagen_%i.jpg",i);
		sprintf(imagename, "imagen_%i",i);
		int numImagen = i;

		pid = fork(); 
		if(pid < 0){
			fprintf(stderr, "No se pudo crear el proceso hijo" ); 
        	return 1;
		}

		if(pid > 0){ //Es el padre

			close(pipes[LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
			write(pipes[ESCRITURA], &umbralBin, sizeof(int));
        	write(pipes[ESCRITURA], &umbralNeg, sizeof(int));
			write(pipes[ESCRITURA], &flagResultados, sizeof(int));
			write(pipes[ESCRITURA], nombreArchivoMasc, strlen(nombreArchivoMasc)+1);
			write(pipes[ESCRITURA], imagename, 30);
			write(pipes[ESCRITURA], filename, 30);
			write(pipes[ESCRITURA], &numImagen, sizeof(int));
			printf("al parecer soy el padre y mi pid es: %i\n" , getpid());
        	printf("Ya escribi el arr en el pipe\n");
			waitpid(pid, &status,0);
		}
		else{ //Es el hijo
		
			close(pipes[ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
			printf("al parecer soy el hijo y mi pid es: %i\n" , getpid());
			printf("ahora voy a cambiar el fd de lectura del pipe\n");
			dup2(pipes[LECTURA], STDIN_FILENO);
        	printf("ahora voy a cambiar mi codigo con excev\n");
			execl("/prcs/prcsLectura", "ls","-al", NULL);
		}

	printf("Termina el padre\n");
	}

	return 0;
}
	/*
	int **mascara = leerMascara(nombreArchivoMasc);

	uint8_t num = 10;
	num = num*(-1);
	int entero = (int)num;


    // Para cada imagen
	for (int i = 1; i <= cantImagenes; i++)
	{

		char filename[30];
		char imagename[30];
		sprintf(filename,"./imagen_%i.jpg",i);
		sprintf(imagename, "imagen_%i",i);
		
		//1. Leer la imagen
		JpegData jpegData = leerImagenes(filename); 

		//2. Convertir a escala de grises
		jpegData = convertirAEscalaGrises(jpegData); 
		
		//3. aplicar filtro laplaciano
		jpegData = aplicarFiltroLaplaciano(jpegData,mascara); 
		
		//4. binarizar imagen
		jpegData = binarizarImagen(jpegData, umbralBin);
		
		//5. Clasificar imagen
		char *nearlyBlack = analisisDePropiedad(jpegData, umbralNeg);

		//6. Escribir imagen
		char fileout[30];
		sprintf(fileout,"./out_%i.jpg",i);
		escribirImagenes(jpegData, "escalagrises",fileout);

		//7. liberar memoria
		liberarJpeg(&jpegData);

		if(flagResultados){
			if(nearlyBlack[0] == 'n'){
				printf("|          %s       |             %s           |\n", imagename, nearlyBlack);
			}
			else{
				printf("|          %s       |             %s          |\n", imagename, nearlyBlack);
			}
		}
	}
*/
   
    

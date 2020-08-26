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
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include "../incl/lecturaImagenes.h"
#include "../incl/escrituraImagenes.h"
#include "../incl/binarizacion.h"
#include "../incl/clasificacion.h"
#include "../incl/conversion.h"
#include "../incl/filtro.h"

#define LECTURA 0
#define ESCRITURA 1

//Funcion Main
int main (int argc, char *argv[]){

	int cantImagenes = 0;
	int umbralBin = 0;
	int umbralNeg = 0;
	int flagResultados = 0;
	int numImagen = 0;
	int c = 0;
	char *nombreArchivoMasc = NULL;

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

	//Para cada Imagen
	for (int i = 1; i <= cantImagenes; i++)
	{
		printf("Este es el for del proceso main\n");
		int pipe12[2];
		pipe(pipe12);
		int status;
		pid_t pid;
		int lenNombreMasc = strlen(nombreArchivoMasc);

		pid = fork();
		if(pid < 0)	//Si no se pudo crear el proceso
		{
			fprintf(stderr, "No se pudo crear el proceso de Lectura\n");
			return 1;
		}

		else if (pid > 0) //Si es el padre
		{
			printf("entra al padre (Main)\n");
			close(pipe12[LECTURA]);
			//Escribo las cosas en el pipe para enviarselas al proceso hijo
			write(pipe12[ESCRITURA], &umbralBin, sizeof(int));
			write(pipe12[ESCRITURA], &umbralNeg, sizeof(int));
			write(pipe12[ESCRITURA], &flagResultados, sizeof(int));
			write(pipe12[ESCRITURA], &i, sizeof(int));
			write(pipe12[ESCRITURA], &lenNombreMasc, sizeof(int));
			write(pipe12[ESCRITURA], nombreArchivoMasc, lenNombreMasc);

			//Espero al hijo
			waitpid(pid, &status, 0);
		}

		else
		{
			printf("entra al hijo (Hijo del main -> Plectura)\n");
			close(pipe12[ESCRITURA]);
			dup2(pipe12[LECTURA], STDIN_FILENO);

			//cambiar código del hijo con execv
			char *args[]={"./pLectura",NULL}; 
        	execv(args[0],args);
		}

	printf("Se proceso correctamente la imagen %d\n",i);

	}
	printf("Finaliza el procesamiento de imagenes\n");
	return 1;
	
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
   
    

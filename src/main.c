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
		int pipe12[2];	//Se declara pipe 12 
		pipe(pipe12);	//Del pipe 1 al pipe 2
		int status;
		pid_t pid;
		int lenNombreMasc = strlen(nombreArchivoMasc); //Largo del kernel

		pid = fork();	//Se crea proceso
		if(pid < 0)		//Si no se pudo crear el proceso
		{
			fprintf(stderr, "No se pudo crear el proceso de Lectura\n");
			return 1;
		}

		else if (pid > 0) //Si es el padre
		{
			close(pipe12[LECTURA]);
			//Se escriben las variables en el pipe para enviarlas al proceso hijo
			write(pipe12[ESCRITURA], &umbralBin, sizeof(int));				
			write(pipe12[ESCRITURA], &umbralNeg, sizeof(int));
			write(pipe12[ESCRITURA], &flagResultados, sizeof(int));
			write(pipe12[ESCRITURA], &i, sizeof(int));
			write(pipe12[ESCRITURA], &lenNombreMasc, sizeof(int));
			write(pipe12[ESCRITURA], nombreArchivoMasc, lenNombreMasc);

			//Espera a que el hijo termine su ejecución
			waitpid(pid, &status, 0);
		}

		else
		{
			//Ingresa al hijo. Hijo del main -> Plectura
			close(pipe12[ESCRITURA]);
			dup2(pipe12[LECTURA], STDIN_FILENO);

			//cambiar código del hijo con execv
			char *args[]={"./pLectura",NULL}; 
        	execv(args[0],args);
		}
	}
	return 1;
	
}



//Directivas de Preprocesamiento
#include <stdio.h>
#include <jpeglib.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "../incl/filtro.h"
#include "../incl/lecturaImagenes.h"

//Entradas:     - string (puntero a char) que representa el nombre del archivo de la mascara
//Funcionamiento: se encarga de abrir el archivo, leer la mascara contenida en el y almacenar
//                la mascara en una matriz de enteros.
//Salidas:      - Matriz de enteros que representa la mascara leída.

int **leerMascara(char *nombreMascara){
    FILE *archivo = fopen(nombreMascara,"r");
    if(archivo == NULL){
        printf("Error: no se ha detectado un archivo de texto \n"); 
    }

    int **mascara = crearPunteroMascara();
    int i = 0;
    int j = 0;
    int num1,num2,num3;
    for(i = 0;i < 3; i++){
        fscanf(archivo,"%d %d %d", &num1,&num2,&num3);
        mascara[i][0] = num1;
        mascara[i][1] = num2;
        mascara[i][2] = num3;
    }
    return mascara;
}

//Entradas:     
//Funcionamiento: asigna un espacio de memoria a la matriz de enteros que representa a la mascara.
//Salidas:      - Matriz de enteros y el espacio de memoria pertinente.
int **crearPunteroMascara(){
    int i= 0;
    int **mascara = (int**)malloc(3*sizeof(int*));
    for(i = 0; i < 3; i++){
        mascara[i] = (int*)malloc(3*sizeof(int));
    }
    return mascara;
}

//Entradas:     - Imagen en escala de grises y del tipo JpegData
//              - Matriz de enteros que representa la mascara para aplicar el filtro.
//Funcionamiento: se encarga de recorrer cada pixel de la imagen, con el objetivo de aplicar
//                el filtro laplaciano. Se utilizan las dimensiones originales simulando una
//                matriz de de píxeles.
//Salidas:      - Imagen del tipo JpegData luego de aplicar el filtro laplaciano sobre ella

JpegData aplicarFiltroLaplaciano(JpegData img,int **mascara){
    int w = img.width;
    int h = img.height;
    int loc = w +1;
    int i,j;
    
   JpegData nueva;
    nueva.height = h;
    nueva.width = w;
    nueva.ch = 1;
    alloc_jpeg(&nueva);

    for (int i = 0; i < w*h; i++){
        nueva.data[i] = img.data[i];
    }

    //printf("\n");
    for(i = 1; i < h -1; i++){
        for(j = 1; j < w - 1; j++){
            calcularFiltro(&img,&nueva,mascara,loc,w,h);
            loc++;
        }
        loc+=2;         //Permite omitir los margenes de la matriz (en términos visuales) de píxeles
    }
 liberarJpeg(&img);
    return nueva;

}

//Entradas:     - Imagen original en escala de grises y del tipo JpegData
//              - Imagen copiada (nueva) de la original en escala de grises y del tipo JpegData
//              - Matriz de enteros que representa la mascara para aplicar el filtro.
//              - entero llamado loc que representa la locacion del pixel que cambiará
//                tras aplicar el filtro.
//              - Dos enteros, w y h, quienes representan el ancho y alto de la imagen original.
//Funcionamiento: se encarga de aplicar el filtro laplaciano en cada locación de la imagen copiada. Para
//                hacer las operaciones en cada pixel, se hace uso de la imagen original, debido a que
//                esta no posee los cambios luego de implementar el filtro.
//Salidas:      - Imagen copiada del tipo JpegData luego de modificar uno de sus pixeles.

void  calcularFiltro(JpegData *img,JpegData *nueva,int **mascara,int loc,int w, int h){
    int n1 = img->data[loc - w -1];
    n1 = n1* mascara[0][0];
    int n2 = img->data[loc - w] ; 
    n2= n2 * mascara[0][1];
    int n3 = img->data[loc - w + 1] ; 
    n3 = n3 * mascara[0][2];
    int n4 = img->data[loc - 1] ; 
    n4 = n4 * mascara[1][0];
    int n5 = img->data[loc]; 
    n5 = n5 * mascara[1][1];
    int n6 = img->data[loc + 1] ;
    n6 = n6 * mascara[1][2]; 
    int n7 = img->data[loc + w -1] ;
    n7 = n7 * mascara[2][0]; 
    int n8 = img->data[loc + w] ; 
    n8 = n8 * mascara[2][1];
    int n9 = img->data[loc + w +1] ;
    n9 = n9* mascara[2][2];
    int resultado = n1 + n2 + n3 + n4 + n5 + n6 +n7 +n8 +n9; 
    nueva->data[loc] = resultado;
 
}
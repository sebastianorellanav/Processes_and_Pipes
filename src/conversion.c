//Directivas de Preprocesamiento
#include <stdio.h>
#include <jpeglib.h>
#include <stdint.h>
#include "../incl/conversion.h"
#include "../incl/lecturaImagenes.h"


//Entradas:     - Imagen en formato RGB del tipo JpegData.
//Funcionamiento: se crea una copia de la imagen original, para luego recorrer
//                cada píxel y efectuar la ecuación de luminiscencia en cada uno
//                de ellos. Cada resultado de la ecuación se guarda en la variable
//                Y, la cual representará los nuevos pixeles de la imagen en 
//                escala de grises.
//Salidas:      - Imagen en escala de grises del tipo JpegData
JpegData convertirAEscalaGrises(JpegData img){
    int len = img.height*img.width*img.ch;
    int loc = 0;
    int R, G, B;
    uint8_t Y;
    
    JpegData nuevaImg;
    nuevaImg.width = img.width;
    nuevaImg.height = img.height;
    nuevaImg.ch = 1;            //canal = 1. Representa una imagen en escala de grises.
    alloc_jpeg(&nuevaImg);
    
    for (int i = 0; i < len; i+=3)
    {
        R = img.data[i]; 
        G = img.data[i+1];
        B = img.data[i+2];
        Y = R*0.3 + G*0.59 + B*0.11;
        nuevaImg.data[loc] = Y;
        loc++;
    }
    return nuevaImg;
}

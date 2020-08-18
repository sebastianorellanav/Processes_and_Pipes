//Directivas de Preprocesamiento
#include "../incl/binarizacion.h"
#include <stdint.h>
#include <stdio.h>

//Entradas:     - JpegData que representa la imagen filtrada (Filtro de realce).
//              - uint8_t que representa el umbral de binarización de la imagen.       
//Funcionamiento: se compara el valor de cada pixel de la imagen de entrada con
//                el umbral ingresado. Si el pixel es mayor al umbral, entonces
//                cambiará su valor a 255 (blanco) y en caso contrario, el valor 
//                del pixel  será 0 (negro). 
//Salidas:      - Una imagen del tipo JpegData binarizada (con píxeles brancos y negros).

JpegData binarizarImagen(JpegData img, uint8_t umbral){

    int len = img.width*img.height;

    for (int i = 0; i < len; i++)
    {
        if(img.data[i] > umbral){
            img.data[i] = 255;
        }
        else{
            img.data[i] = 0;
        }
    }

    return img; 
}
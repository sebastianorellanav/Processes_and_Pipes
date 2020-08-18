//Directivas de Preprocesamiento
#include <stdint.h>
#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


// Estructura de imagen JPEG
typedef struct {
    uint8_t *data;   // arreglo de píxeles
    uint32_t width;
    uint32_t height;
    uint32_t ch;     // canal del color:  3 para RGB
                    //                    1 para escala de grises
} JpegData;

#endif
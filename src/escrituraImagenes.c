//Directivas de Preprocesamiento
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <inttypes.h>
#include "../incl/escrituraImagenes.h"


//Entradas:     - Imagen del tipo JpegData llamada jpegData
//              - Nombre de salida del tipo char llamado nombreSalida
//              - Puntero a estructura de libreria jpeglib llamado jerr
//              - Un string (puntero a char) que indica el tipo de color: 
//                RGB o escala de grises. Tiene un nombre de tipoColor.
//Funcionamiento: Se escribe una imagen del tipo JPEG, para lo cual se 
//                comprime la imagen utilizado a lo largo del programa.
//Salidas:      - Entero que indica la correcta creación del archivo (valor 1).
//              - Imagen con formato JPEG
int escribirJpeg(const JpegData *jpegData,
                const char *nombreSalida,
                struct jpeg_error_mgr *jerr, 
                char *tipoColor)
{
    // 1. Creación del objeto de compresión JPEG.
    struct jpeg_compress_struct cinfo;
    jpeg_create_compress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(nombreSalida, "wb");
    if (fp == NULL) {
        printf("Error: failed to open %s\n", nombreSalida);
        return 0;
    }
    // 2. Especificar el destino del data.
    jpeg_stdio_dest(&cinfo, fp);

    // 3. Cambiando parámetros para la compresión.
    cinfo.image_width      = jpegData->width;
    cinfo.image_height     = jpegData->height;
    cinfo.input_components = jpegData->ch;
    if(tipoColor == "rgb")
        cinfo.in_color_space   = JCS_RGB;
    else
        cinfo.in_color_space   = JCS_GRAYSCALE;
    
    jpeg_set_defaults(&cinfo);

    // 4. Inicio de la compresión.
    jpeg_start_compress(&cinfo, 1);

    // 5. Lectura linea a linea de la imagen.
    uint8_t *row = jpegData->data;
    const uint32_t stride = jpegData->width * jpegData->ch;
    for (int y = 0; y < jpegData->height; y++) {
        jpeg_write_scanlines(&cinfo, &row, 1);
        row += stride;
    }

    // 6. Fin de la compresión.
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(fp);

    return 1;
}

//Entradas:     - Imagen del tipo JpegData llamada jpegData
//              - Un string (puntero a char) que indica el tipo de color: 
//                RGB o escala de grises. Tiene un nombre de tipoColor.
//              - Nombre de salida del tipo char llamado nombreSalida
//Funcionamiento: Se escribe una imagen del tipo JPEG. Esta función es llamada 
//                por el main.
//Salidas:      - Un JpegData que representa a la imagen de entrada.
JpegData escribirImagenes(JpegData jpegData, char *tipoColor, char *nombreSalida){
    struct jpeg_error_mgr jerr;
    char *dst = nombreSalida;
    if (!escribirJpeg(&jpegData, dst, &jerr, tipoColor)){
        liberarJpeg(&jpegData);
        return jpegData;
    }
    return jpegData;
}

    
//Directivas de Preprocesamiento
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <jpeglib.h>
#include <inttypes.h>
#include "../incl/lecturaImagenes.h"


//Entradas:     - Una imagen del tipo JpegData.
//Funcionamiento: crea un espacio de memoria para la imagen
//Salidas:      
void alloc_jpeg(JpegData *jpegData){
    jpegData->data = NULL;
    jpegData->data = (uint8_t*) malloc(sizeof(uint8_t)  *
                                       jpegData->width  *
                                       jpegData->height *
                                       jpegData->ch);
}

//Entradas:     - Imagen del tipo JpegData.
//Funcionamiento: libera la memoria asignada para la imagen
//Salidas:    
void liberarJpeg(JpegData *jpegData){
    if (jpegData->data != NULL) {
        free(jpegData->data);
        jpegData->data = NULL;
    }
}

// read JPEG image
// 1. create JPEG decompression object
// 2. specify source data
// 3. read JPEG header
// 4. set parameters
// 5. start decompression
// 6. scan lines
// 7. finish decompression

//Entradas:     - Imagen del tipo JpegData llamada jpegData
//              - Nombre de archivo de entrada del tipo char llamado nombreArchivo
//              - Puntero a estructura de libreria jpeglib llamado jerr
//Funcionamiento: Se lee una imagen del tipo JPEG, para lo cual se 
//                descomprime la imagen.
//Salidas:      - Entero que indica la correcta lectura del archivo (valor 1).
//              - Imagen del tipo JpegData   
int leerJpeg(JpegData *jpegData,
              const char *nombreArchivo,
              struct jpeg_error_mgr *jerr)
{
    // 1. Creación del objeto de descompresión JPEG
    struct jpeg_decompress_struct cinfo;
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(nombreArchivo, "rb");
    if (fp == NULL) {
        printf("Error: failed to open %s\n", nombreArchivo);
        return 0;
    }
    // 2. Especificando los datos de origen.
    jpeg_stdio_src(&cinfo, fp);

    // 3.
    jpeg_read_header(&cinfo, 1);

    // 4. Inicio de descompresión.
    jpeg_start_decompress(&cinfo);

    jpegData->width  = cinfo.image_width;
    jpegData->height = cinfo.image_height;
    jpegData->ch     = cinfo.num_components;

    alloc_jpeg(jpegData);

    // 5. Lectura linea a linea.
    uint8_t *row = jpegData->data;
    const uint32_t stride = jpegData->width * jpegData->ch;
    for (int y = 0; y < jpegData->height; y++) {
        jpeg_read_scanlines(&cinfo, &row, 1);
        row += stride;
    }

    // 6. Finalización de la descompresión.
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return 1;
}


//Entradas:     - Nombre de archivo de entrada del tipo char llamado nombreEntrada
//Funcionamiento: Se lee una imagen del tipo JPEG. Esta función es llamada  por el main.
//Salidas:       - Un JpegData que representa a la imagen de salida.
JpegData leerImagenes(char *nombreEntrada){
    JpegData jpegData;
    struct jpeg_error_mgr jerr;
    char *src = nombreEntrada;
    
    if (!leerJpeg(&jpegData, src, &jerr)){
        liberarJpeg(&jpegData);
        return jpegData;
    }
    return jpegData;
}





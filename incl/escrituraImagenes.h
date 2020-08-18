//Directivas de Preprocesamiento
#ifndef ESCRITURA_H
#define ESCRITURA_H
#include "estructuras.h"

//Cabeceras de Funciones
JpegData escribirImagenes(JpegData jpegData1, char *typeColorSpace, char *nombreSalida);
int escribirJpeg(const JpegData *jpegData,
                const char *dstfile,
                struct jpeg_error_mgr *jerr,
                char *typeColorSpace);
void liberarJpeg(JpegData *jpegData);

#endif
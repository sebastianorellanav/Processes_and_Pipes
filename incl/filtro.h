//Directivas de Preprocesamiento
#include "estructuras.h"
#ifndef FILTRO_H
#define FILTRO_H

//Cabeceras de funciones
JpegData aplicarFiltroLaplaciano(JpegData img,int **mascara);
int **leerMascara(char *nombreMascara);
int **crearPunteroMascara();
void calcularFiltro(JpegData *img,JpegData *nuevo,int **mascara,int loc,int w, int h);
#endif
//Directivas de Preprocesamiento
#include "../incl/clasificacion.h"


//Entradas:     - Una imagen del tipo JpegData que ha sido binarizada.
//              - Un entero que representa al umbral de clasificación.
//Funcionamiento: se cuentan todos los píxeles que son negros en la
//                imagen de entrada, para luego obtener su porcentaje
//                de aparición. Si el procentaje de píxeles negros es
//                mayor o igual al umbral, se dice que la imagen es 
//                nearly black.
//Salidas:      - Un puntero a char (string) que indica si la imagen es
//                nearly black o no.
char *analisisDePropiedad(JpegData img, int umbral){

    int len = img.width*img.height;
    int cont = 0;

    for (int i = 0; i < len; i++)
    {
        //Si es pixel negro
        if(img.data[i] == 0){
            //se suma 1 al contador
            cont++;
        }
    }
    
    float porcentajeNegrura = (cont*100)/len;

    if (porcentajeNegrura >= umbral){
        return "yes";
    }
    else{
        return "no";
    }
    
}


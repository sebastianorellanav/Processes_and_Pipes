# Processes_and_Pipes

## Estructura del Código 📃

El codigo se divide en 4 carpetas:

- **incl/**: Contiene los archivos .h que a su vez contienen las cabeceras de las funciones utilizadas en el programa
- **obj/**: Contiene los archivos .o creados al compilar el programa
- **prcs/**: Contiene los archivos .c utilizados para crear los nuevos procesos
- **src/**: Contiene los archivos .c utilizados que a su vez contienen las funciones utilizadas en los procesos

Además, en la carpeta raiz se encuentran los siguientes archivos:

* Makefile: Utilizado para compilar el programa
* mascara: contiene el kernel utilizado para el filtro laplaciano
* README.md: contiene las instrucciones para ejecutar el codigo
* imagenes.jpg: son imagenes de prueba para usar el programa

## Como Compilar ⚙️

Para compilar el programa se debe utilizar el siguiente comando dentro de la carpeta raiz:

```
make
```

Luego de compilar el programa generará 7 archivos ejecutables: *ejecutable*, *pLectura*, *pConversion*, *pFiltro*, 
*pBinarizacion*, *pClasificacion* y *pEscritura*. El primero corresponde al ejecutable del programa los demás corresponden a los ejecutables de cada proceso.

Por otro lado en la carpeta **obj/** se crearán los archivos .o generados en la compilación.

## Como Ejecutar 🚀

El nombre del programa creado es "Ejecutable" por lo que para ejecutarlo se debe usar ese nombre. Además, para ejecutar el programa el programa se deben utilizar las siguientes flags:

* -u [Entero]  ->  Umbral para binarizar la imagen
* -n [Entero]  ->  Umbral para la clasificación de negrura de la imagen
* -c [Entero]  ->  Cantidad de imagenes a analizar
* -m [String]  ->  Nombre del archivo que contiene la mascara a utilizar
* -b (opcional)  ->  Mostrar resultados por consola (Esta flag es opcional)

A continuación se muestran ejemplos de como ejecutar el programa:

```
./ejecutable -u 80 -n 99 -m mascara -c 7
```

```
./ejecutable -c 4 -u 50 -n 70 -m mascara -b
```

## Información Importante ❗❗

El programa se **demora un tiempo considerable** cuando se procesan imagenes muy grandes, debido a que al pasar la imagen por el pipe entre cada proceso tiene un alto costo en terminos de tiempo de ejecución.
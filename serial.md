# Proyecto 2 - Motion Vectors Algorithm

    Stiven Ramírez Arango - sramir70@eafit.edu.co
    Sebastián Ospina Cabarcas - sospin26@eafit.edu.co
    Camilo Suaza Gallego - csuazag@eafit.edu.co

# Descripción del Algoritmo Serial

# 1. Análisis

## 1.2 ¿Cómo funciona el algoritmo?

1. Lectura de las imágenes
El algoritmo se encarga de leer dos imágenes BMP en escala de grises. Lo primero es entender cómo es la estructura de una imagen BMP y saber cómo capturar los datos necesarios para la solución.

Estructura de una imagen BMP:

Para nuestro ejercicio, los campos que debemos capturar de la imagen son el width, height, bitCount, data.
Creamos una estructura que contendrá la información de la imagen ya mencionada. Al leer la data de la imagen, esta es retornada en un arreglo, por ello debemos usar un función que mapea de la lógica matricial al arreglo, además, existe un control de que la imagen si sea de 8 bits por pixel tomando este dado del header de la imagen.

Para el procesamiento del algoritmo tenemos 6 ciclos anidados, dándonos un orden de O(n^6).

Los primeros dos ciclos, iteran sobre el frame 1 de macro-bloque en macro-bloque, es decir, por cada iteración se incrementa en 16 en 16 tanto para las filas como para las columnas de la matriz. Por cada iteración en el frame 1, creamos una estructura que almacenará el resultado del valor mínimo encontrado en el frame 2 con sus respectivos índices.

Los siguientes dos ciclos realizan el recorrido en el frame 2 macro-bloque a macro-bloque pero este con un delta de un pixel, cuando por supuesto un desbordamiento de la matriz, por cada iteración de estos ciclos tendremos una variable summation que se encargará de saber cuánto ha sido el valor acumulado entre las restas de los dos macrobloques (procesamiento de los siguientes ciclos), esta variable summation es comparada con el valor minimo encontrado entre todos los macrobloques del frame2, realizamos la comparación de si ha encontrado uno menor del existente y guardamos en la estructura el valor mínimo y los índices en el frame 2.

Los ciclos más internos realizan el procesamiento de dado los índices que tienen el macrobloque del frame 1 y dado los índices que tienen el macrobloque del frame 2, realizará la resta píxel por píxel obteniendo la sumatoria de todas las restas efectuadas en la variable summation.

Por cada macro-bloque del frame 1 tenemos una estructura que contiene la información de dónde se encuentra el macro-bloque mas parecido en el frame 2, cada estructura se guardará en una matriz de resultados.



## 1.2 Definición de tecnología de desarrollo:

* Lenguaje de Programación: C++
* Técnica Aplicada: Serial

## 1.3 Tiempo de ejecución.

Como la ejecución de este algoritmo es en serial, solo se está usando un core. el tiempo que toma recorrer todas las dos matrices y operar es de 474.603 segundos con imágenes diferentes de 640 x 480 píxeles.


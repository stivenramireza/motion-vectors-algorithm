# Proyecto 2 - Motion Vectors Algorithm

    Stiven Ramírez Arango - sramir70@eafit.edu.co
    Sebastián Ospina Cabarcas - sospin26@eafit.edu.co
    Camilo Suaza Gallego - csuazag@eafit.edu.co

# Descripción del Algoritmo con Memoria Distribuida (MPI)

El **algoritmo MPI** que usa memoria distribuida en diferentes procesadores tiene 1 **MASTER** que envían los datos del macro-bloque repartido entre el número de SLAVES (en el cluster sólo tenemos 2). A su vez, también envía el tamaño del height y width, incluyendo su correspondiente matriz para el frame 1 y del frame 2.

Estos datos los recibe los **SLAVES** y los procesa hasta obtener una matriz de macro-bloques que contiene los
puntos mínimos (que significan la similitud del frame 1 con el frame 2). 

Finalmente, la matriz se almacena y es enviada al **MASTER** para mostrar sus resultados.

![Macro-bloques2](macro-bloques2.png?raw=true?style=centerme "Macro-bloques2")

Esta la función de similitud entre 2 macro-bloques:

![Sumatorias](sumatorias.png?raw=true?style=centerme "Sumatorias")

# 1. Análisis

## 1.2 ¿Cómo funciona el algoritmo?





## 1.2 Definición de tecnología de desarrollo para el algoritmo:

* Lenguaje de Programación: C++
* Framework Aplicado: MPI


## 1.3 Speedup



## 1.4 Eficiencia
# Proyecto 2 - Motion Vectors Algorithm

    Stiven Ramírez Arango - sramir70@eafit.edu.co
    Sebastián Ospina Cabarcas - sospin26@eafit.edu.co
    Camilo Suaza Gallego - csuazag@eafit.edu.co

# Descripción del Algoritmo con Memoria Distribuida (MPI)

La técnica consiste en predecir hacia donde se mueven los macro-bloques (16 x 16 pixeles) de un frame i en el frame i+1. Se realiza una búsqueda hacia delante (frames P) y además hacía atrás (frames B). Para este ejercicio solo se realizará búsqueda hacia delante.

![Macro-bloques1](macro-bloques1.png?raw=true?style=centerme "Macro-bloques1")

Cada macro-bloque se debe buscar en el frame siguiente en una región de búsqueda. Existen múltiples heurísticas para determinar la región de búsqueda. Para efectos de este problema, se tomará toda el frame i+1 como la región de búsqueda y se resolverá mediante fuerza bruta.

![Macro-bloques2](macro-bloques2.png?raw=true?style=centerme "Macro-bloques2")

Esta la función de similitud entre 2 macro-bloques:

![Sumatorias](sumatorias.png?raw=true?style=centerme "Sumatorias")

El **algoritmo MPI** que usa memoria distribuida en diferentes procesadores tiene 1 **MASTER** que envían los datos del macro-bloque repartido entre el número de SLAVES (en el cluster sólo tenemos 2). A su vez, también envía el tamaño del height y width, incluyendo su correspondiente matriz para el frame 1 y del frame 2.

Estos datos los recibe los **SLAVES** y los procesa hasta obtener una matriz de macro-bloques que contiene los
puntos mínimos (que significan la similitud del frame 1 con el frame 2). 

Finalmente, la matriz se almacena y es enviada al **MASTER** para mostrar sus resultados.

# 1. Análisis

## 1.2 ¿Cómo funciona el algoritmo?





## 1.2 Definición de tecnología de desarrollo para el algoritmo:

* Lenguaje de Programación: C++
* Framework Aplicado: MPI


## 1.3 Speedup



## 1.4 Eficiencia
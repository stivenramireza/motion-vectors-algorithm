# Proyecto 2 - Motion Vectors Algorithm

    Stiven Ramírez Arango - sramir70@eafit.edu.co
    Sebastián Ospina Cabarcas - sospin26@eafit.edu.co
    Camilo Suaza Gallego - csuazag@eafit.edu.co

# Descripción del Algoritmo Serial

La técnica consiste en predecir hacia donde se mueven los macro-bloques (16 x 16 pixeles) de un frame i en el frame i+1. Se realiza una búsqueda hacia delante (frames P) y además hacía atrás (frames B). Para este ejercicio solo se realizará búsqueda hacia delante.

![Macro-bloques1](macro-bloques1.png?raw=true "Macro-bloques1")

Cada macro-bloque se debe buscar en el frame siguiente en una región de búsqueda. Existen múltiples heurísticas para determinar la región de búsqueda. Para efectos de este problema, se tomará toda el frame i+1 como la región de búsqueda y se resolverá mediante fuerza bruta.

![Macro-bloques2](macro-bloques2.png?raw=true "Macro-bloques2")

Esta la función de similitud entre 2 macro-bloques:

![Sumatorias](sumatorias.png?raw=true "Sumatorias")

# 1. Análisis

## 1.2 ¿Cómo funciona el algoritmo?

![Monting Vectors Algorithm](algoritmo.jpeg?raw=true "Motion Vectors Algorithm")

## 1.2 Definición de tecnología de desarrollo para el algoritmo:

* Lenguaje de Programación: C++
* Técnica Aplicada: Serial
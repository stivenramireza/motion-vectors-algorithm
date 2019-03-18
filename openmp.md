# Proyecto 2 - Motion Vectors Algorithm

    Stiven Ramírez Arango - sramir70@eafit.edu.co
    Sebastián Ospina Cabarcas - sospin26@eafit.edu.co
    Camilo Suaza Gallego - csuazag@eafit.edu.co

# Descripción del Algoritmo con Memoria Compartida (OpenMP)





# 1. Análisis

## 1.2 ¿Cómo funciona el algoritmo?

Dada las dos matrices NxM que significan los frames de la imagen 1 y 2, decidimos que el mejor punto para aplicar el framework OpenMP es desde la parte mas externa, es decir, el primer ciclo mas externo de nuestro algoritmo.

Los primeros dos ciclos son los encargados de recorrer el primer frame de macro-bloque en macro-bloque, los siguientes dos ciclos son los encargados del recorrido con un delta de 1 pixel en la segunda matriz (frame 2) para obtener los macrobloques y los siguientes dos ciclos son los encargados de realizar las restas entre el macrobloque del frame 1 y el macrobloque del frame 2.

Cuando empezamos a analizar cuál sería una buena sección paralelizable de nuestro algoritmo decidimos empezar el análisis desde lo más interno hasta lo más externo para así definir qué sería más eficiente en términos de procesamiento, a continuación vamos a mostrar el análisis realizado en las 3 zonas paralelizables, el por qué tomamos la más externa y además expondremos los cálculos de la cantidad de ciclos.

1. La zona más interna.
Empezamos a analizar qué cuál sería el beneficio de paralelizar la zona más interna que es cuándo se restan los dos macrobloques y consideramos que no existe un beneficio significativo porque los cálculos son pequeños, es decir, el acceso a cada posición de los macrobloques (matrices) a restar es O(1) y la matriz es una matriz de 15x15, es decir que tenemos 255 iteraciones, a pesar de que son 255 iteraciones un solo hilo puede con esta tarea fácilmente y se demora muy poco tiempo. Entonces cuando consideramos esta opción concluímos que existiría un overhead debido que como se especifica el for mas externo para paralelizar, estaríamos indicando que vamos a repartir 15 iteraciones entre n hilos, teniendo en cuenta que cada hilo debe almacenar su valor de resta para luego hacer un parallel reduction con la variable summation, el incremento de la variable summation se vuelve una zona crítica porque debe ser una operación atómica para el incremento, por esto se complica más el algoritmo paralelo tratando de hacer un reducttion de un procesamiento pequeño sabiendo que los recorridos en sí de los dos frames son muchísimo más grandes y requieren mejor distribución de procesamiento. Por esta razón descartamos paralelizar la sección más interna.

2. La zona medio.
El gran recorrido que tiene nuestro algoritmo es en estos dos ciclos del medio debido que son los que se desplazan con un delta de un pixel, haciendo para el ejercicio de las imagenes de (480H x 640W) obtengamos (480 - 16) * (640 - 16) = 289536 iteraciones, siento paralelizado el for que recorre las filas (height) de 464 iteraciones. Estas 464 iteraciones son repartidas entre n hilos, sin embargo, el problema persiste debido que a pesar de que desaparece el reduction de la variable summation, aparece una zona crítica que es la comparación de cuál es el menor valor de todos, es decir, estaríamos diciendo que por cada macrobloque del frame 1, vamos a repartir entre los n hilos las restas de los macrobloques del frame 2, teniendo como zona crítica cuál es la menor resta entre todas las restas hechas por los hilos, haciendo que después de que terminase cada hilo, guardásemos la variable summation, en un arreglo compartido por los hilos para así al final de todos los hilos, recorrer este arreglo y mirar cual es el menor de todos y así almacenarlo en la matriz de resultados, suena atractivo el planteamiento pero haría que existiese un for extra que recorra todo el arreglo compartido del mismo número de iteraciones de los ciclos del medio, es decir un arreglo compartido de 289536 posiciones. Esta opción se descarta porque aumenta el orden de nuestro algoritmo a O(n^7).

3. La zona más externa
Tras haber descartado las dos zonas mas internas del algoritmo, nos quedamos con que la zona más externa es la más calificable para ser paralelizable, estos dos ciclos mas externos para estas imagenes nos arrojan una cantidad de (480 / 16) * (640 / 16) = 1200 iteraciones en el frame 1 para los macrobloques. Teniendo n hilos que se reparten 40 iteraciones que aunque suena poco, lo eficiente es que se hace que cada hilo haga el proceso completo de procesar todos los macrobloques del frame 2 y determinar cual de todos arroja la menor resta, de esta manera, desaparece el incremento de la complejidad y la zona crítica de la variable summation. Paralelizando nuestro algoritmo en esta zona nos proporciona mayor tranquilidad de no tener un overhead por agregar más código a nuestra solución serial.



## 1.2 Definición de tecnología de desarrollo para el algoritmo:

* Lenguaje de Programación: C++
* Framework Aplicado: OpenMP

## 1.3 Speedup



## 1.4 Eficiencia




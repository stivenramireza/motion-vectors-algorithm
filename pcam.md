# Proyecto 2 - Motion Vectors Algorithm

    Stiven Ramírez Arango - sramir70@eafit.edu.co
    Sebastián Ospina Cabarcas - sospin26@eafit.edu.co
    Camilo Suaza Gallego - csuazag@eafit.edu.co           Video: https://youtu.be/iPCX9sK8Uo4

# Descripción del Algoritmo con la Metodología PCAM (OpenMP y MPI)

# 1. Análisis

## 1.2 ¿Cómo funciona el algoritmo?

![pcam](https://user-images.githubusercontent.com/31974084/54576732-567f1780-49c7-11e9-923b-746ad0c84e20.png)

### 1.2.1 (P) Particionado

El **MASTER** comienza a ejecutarse y define los datos que se enviarán a los **SLAVES** para que éstos los procesen. El dato particionado es:

    - Tamaño del macro-bloque divido entre el número de tasks.

![particionado](https://user-images.githubusercontent.com/31974084/54577440-da3a0380-49c9-11e9-94e5-f87ed69d9cbd.png)

### 1.2.2 (C) Comunicaciones

Las comunicaciones que realizan son bloqueantes/no bloqueantes, pues el **MASTER** envía primero los datos, los **SLAVES** los procesan y devuelven una matriz con los mínimos al **MASTER**.

En otras palabras, el algoritmo envía diferentes datos con un **SCATTER** a todos los procesos y luego los reúne en un **GATHER** para enviar la matriz resultante al **MASTER**.

![comunicacion](https://user-images.githubusercontent.com/31974084/54577264-14ef6c00-49c9-11e9-882a-90a4f9750bf4.png)

### 1.2.3 (A) Aglomeración

En esta etapa se evalúa los duplicados en los procesos para agrupar tareas que comparten algún dato y de esta forma, con lo siguientes datos, procesarlos correctamente a través del paso de mensajes **(MPI)**:

    - Tamaño del macro-bloque divido entre el número de tasks
    - Height del frame 1
    - Width del frame 1
    - Height del frame 2
    - Width del frame 2
    - Matriz de pixeles del frame 1
    - Matriz de pixeles del frame 2

Y con esta información, los **SLAVES** trabajan en la ejecución del algoritmo con **memoria compartida** en diferentes **hilos de ejecución** y con **memoria distribuida** en los diferentes **procesadores (nodos)** de la siguiente manera:

#### 1.2.3.1 La zona más interna

En esta zona se realiza el procesamiento de dado los índices que tienen el macrobloque del frame 1 y dado los índices que tienen el macrobloque del frame 2, realizará la resta píxel por píxel obteniendo la sumatoria de todas las restas efectuadas en la variable summation. 

#### 1.2.3.2 La zona media

En esta zona se realiza el recorrido en el frame 2 macro-bloque a macro-bloque pero este con un delta de un pixel, cuando por supuesto un desbordamiento de la matriz, por cada iteración de estos ciclos tendremos una variable summation que se encargará de saber cuánto ha sido el valor acumulado entre las restas de los dos macrobloques (procesamiento de los siguientes ciclos), esta variable summation es comparada con el valor minimo encontrado entre todos los macrobloques del frame2, realizamos la comparación de si ha encontrado uno menor del existente y guardamos en la estructura el valor mínimo y los índices en el frame 2.

#### 1.2.3.3 La zona más externa

En esta zona se itera sobre el frame 1 de macro-bloque en macro-bloque, es decir, por cada iteración se incrementa en 16 en 16 tanto para las filas como para las columnas de la matriz. Por cada iteración en el frame 1, creamos una estructura que almacenará el resultado del valor mínimo encontrado en el frame 2 con sus respectivos índices.

Esta zona más externa es la más calificable para ser paralelizable con **OpenMP**, estos dos ciclos más externos para estas imagenes nos arrojan una cantidad de (480 / 16) * (640 / 16) = 1200 iteraciones en el frame 1 para los macrobloques. Teniendo n hilos que se reparten 40 iteraciones que aunque suena poco, lo eficiente es que se hace que cada hilo haga el proceso completo de procesar todos los macrobloques del frame 2 y determinar cual de todos arroja la menor resta, de esta manera, desaparece el incremento de la complejidad y la zona crítica de la variable **summation**. Paralelizando nuestro algoritmo en esta zona nos proporciona mayor tranquilidad de no tener un overhead por agregar más código a nuestra solución serial.

![aglo](https://user-images.githubusercontent.com/31974084/54578093-6fd69280-49cc-11e9-847c-63057738e8db.png)

### 1.2.4 (M) Mapeo

Para este proceso se tuvo en cuenta la implementación de un tipo de estructura en C++ que permite almacenar
el mínimo de todos los macro-bloques y de esta forma construir la matriz procesada.

Finalmente, la matriz de mínimos es enviada al **MASTER** y muestra sus resultados.

![mapeo](https://user-images.githubusercontent.com/31974084/54578029-153d3680-49cc-11e9-84f2-649fd1f14ccc.png)

## 1.2 Definición de tecnología de desarrollo:

* Lenguaje de Programación: C++
* Framework Aplicado: OpenMP y MPI

## 1.3 Speedup

## 1.4 Eficiencia
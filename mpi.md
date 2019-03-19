# Proyecto 2 - Motion Vectors Algorithm

    Stiven Ramírez Arango - sramir70@eafit.edu.co
    Sebastián Ospina Cabarcas - sospin26@eafit.edu.co
    Camilo Suaza Gallego - csuazag@eafit.edu.co           Video: https://youtu.be/iPCX9sK8Uo4

# Descripción del Algoritmo con Memoria Distribuida (MPI)

# 1. Análisis

## 1.2 ¿Cómo funciona el algoritmo?

Dada las dos matrices NxM que significan los frames de la imagen 1 y 2, decidimos aplicar el framework MPI usando el cluster proporcionado para el curso, el cual contiene 3 hosts, que son: **192.168.10.80**,
**192.168.10.81** y **192.168.10.82**.

Los primeros dos ciclos son los encargados de recorrer el primer frame de macro-bloque en macro-bloque, los siguientes dos ciclos son los encargados del recorrido con un delta de 1 pixel en la segunda matriz (frame 2) para obtener los macrobloques y los siguientes dos ciclos son los encargados de realizar las restas entre el macrobloque del frame 1 y el macrobloque del frame 2.

La función principal encargada de ejecutar el algoritmo se inicializa en primer lugar con la sentencia **MPI**
para indicarle que se comenzará a ejecutar un proceso con este framework, luego se referencia en el comunicador
universal el **idtask** (id del proceso) y el **numtasks** (número de procesos ó slaves que trabajaran en el procesamiento del algoritmo).

Posteriormente, el **MASTER** comienza a ejecutarse y define los datos que se enviarán a los **SLAVES** para que éstos los procesen. Entre los datos se encuentran:

    - Tamaño total del frame 1
    - Tamaño total del macro-bloque
    - Tamaño del macro-bloque divido entre el número de tasks

El **taskid 0** que es el **MASTER** envía los siguientes datos a sus esclavos:

    - Tamaño del macro-bloque divido entre el número de tasks
    - Height del frame 1
    - Width del frame 1
    - Height del frame 2
    - Width del frame 2
    - Matriz de pixeles del frame 1
    - Matriz de pixeles del frame 2

Y con estos datos los **SLAVES** trabajan en la ejecución del algoritmo con memoria distribuida en diferentes
procesadores de la siguiente manera:

### 1.2.1 La zona más interna

En esta zona se realiza el procesamiento de dado los índices que tienen el macrobloque del frame 1 y dado los índices que tienen el macrobloque del frame 2, realizará la resta píxel por píxel obteniendo la sumatoria de todas las restas efectuadas en la variable summation.

### 1.2.2 La zona media

En esta zona se realiza el recorrido en el frame 2 macro-bloque a macro-bloque pero este con un delta de un pixel, cuando por supuesto un desbordamiento de la matriz, por cada iteración de estos ciclos tendremos una variable summation que se encargará de saber cuánto ha sido el valor acumulado entre las restas de los dos macrobloques (procesamiento de los siguientes ciclos), esta variable summation es comparada con el valor minimo encontrado entre todos los macrobloques del frame2, realizamos la comparación de si ha encontrado uno menor del existente y guardamos en la estructura el valor mínimo y los índices en el frame 2.

### 1.2.3 La zona más externa

En esta zona se itera sobre el frame 1 de macro-bloque en macro-bloque, es decir, por cada iteración se incrementa en 16 en 16 tanto para las filas como para las columnas de la matriz. Por cada iteración en el frame 1, creamos una estructura que almacenará el resultado del valor mínimo encontrado en el frame 2 con sus respectivos índices.

## 1.2 Definición de tecnología de desarrollo

* Lenguaje de Programación: C++
* Framework Aplicado: MPI

## 1.3 Speedup

![speedup](https://user-images.githubusercontent.com/31974084/54575978-5b8e9780-49c4-11e9-8ebe-da02ab4af4ed.jpeg)

Lo que nos representa esta gráfica es que a medida que vamos incrementando el número de nodos (procesadores), el speedup que arroja el algoritmo es mejor, los tiempos de procesamiento bajan porque se reparte entre los n procesadores las iteraciones.

## 1.4 Eficiencia

![eficiencia](https://user-images.githubusercontent.com/31974084/54575882-f470e300-49c3-11e9-81ba-166c30b0fd16.jpeg)

Lo que nos representa esta gráfica es que a medida que incrementamos nuestro número de nodos, nuestro problema decrece en eficiencia, pero, ¿por qué?, la razón es muy simple, la pérdida de eficiencia se debe a una falta de paralelismo del problema/algoritmo dependiendo del número de iteraciones que se vayan a repartir entre los hilos. Como conclusión vemos que el punto más eficiente para nuestro algoritmo es dejando que el # de iteraciones que se reparten, sea igual a la cantidad de los cores disponibles para así hacer que cada proceso haga un procesamiento independiente y optimice su recurso.
# Proyecto 2 - Motion Vectors Algorithm

    Stiven Ramírez Arango - sramir70@eafit.edu.co
    Sebastián Ospina Cabarcas - sospin26@eafit.edu.co     Video Sustentación: https://bit.ly/2uz4NOR
    Camilo Suaza Gallego - csuazag@eafit.edu.co           Video Sustentación: https://youtu.be/twhuJ5_RIso

# ¿Qué es el problema de Motion Vectors (MPEG)?

El problema consiste en predecir hacia donde se mueven los macro-bloques (16 x 16 pixeles) de un frame i en el frame i+1. Se realiza una búsqueda hacia delante (frames P).

![Macro-bloques1](macro-bloques1.png?raw=true?style=centerme "Macro-bloques1")

Cada macro-bloque se debe buscar en el frame siguiente en una región de búsqueda. Existen múltiples heurísticas para determinar la región de búsqueda. Para efectos de este problema, se tomará toda el frame i+1 como la región de búsqueda y se resolverá mediante fuerza bruta.

El criterio para moverse dentro de la región de búsqueda, no es parte del estándar, para el caso de este ejercicio se utilizará búsqueda exhaustiva, es decir, en el peor de los casos (si no lo encuentra), se recorre toda la región de búsqueda con incrementos en dx y dy de un (1) pixel.

**Criterio de parada**: cuando la función anterior es igual a cero (0), indica que ha encontrado el macro-bloque en el destino (frame i+1) y debería parar allí, si la función no es igual a cero (0), deberá encontrar el valor mínimo. 

Es decir Min { val-función en región búsqueda }

El resultado de la localización del macro-bloque de referencia en la región de búsqueda en el siguiente frame, será un una pareja (xi, yi) donde inicia el macrobloque en el frame destino(i+1).

![Macro-bloques2](macro-bloques2.png?raw=true?style=centerme "Macro-bloques2")

Esta la función de similitud entre 2 macro-bloques:

![Sumatorias](sumatorias.png?raw=true?style=centerme "Sumatorias")

# 1. Algoritmo Serial

Para compilarlo en el cluster:

    $ cd serial/
    $ g++ src/bmp.cpp -o bmp-serial

Para ejecutarlo:

    $ time ./bmp-serial

El análisis y diseño del algoritmo en el siguiente link:

* [Algoritmo Serial](serial.md)

# 2. Algoritmo Paralelo con OpenMP

Para compilarlo y ejecutarlo en el cluster:

    $ cd openmp/
    $ g++ src/bmp.cpp -o bmp-openmp -fopenmp
    $ export OMP_NUM_THREADS=2                      // Para configurar el número de cores
    $ export OMP_DISPLAY_ENV=’true’                 // Para configurar la info del ambiente de ejecución
    $ time ./bmp-openmp

El análisis y diseño del algoritmo en el siguiente link:

* [Algoritmo Paralelo con OpenMP](openmp.md)

# 3. Algoritmo Paralelo con MPI

Para compilarlo y ejecutarlo en cluster:

    $ cd mpi/
    $ mpic++ -O2 -w src/bmp.cpp -o bmp-mpi
    $ time mpirun -f hosts_mpi -np 3 ./bmp-mpi      // Donde np es el número de procesadores

El análisis y diseño del algoritmo en el siguiente link:

* [Algoritmo Paralelo con MPI](mpi.md)

# 4. Algoritmo Paralelo con OpenMP y MPI (PCAM)

Para compilarlo y ejecutarlo en cluster:

    $ cd pcam/
    $ mpic++ -O2 -w src/bmp.cpp -o bmp-pcam
    $ export OMP_NUM_THREADS=2                      // Para configurar el número de cores
    $ export OMP_DISPLAY_ENV=’true’                 // Para configurar la info del ambiente de ejecución
    $ time mpirun -f hosts_mpi -np 3 ./bmp-pcam     // Donde np es el número de procesadores

El análisis y diseño del algoritmo en el siguiente link:

* [Algoritmo Paralelo con OpenMP y MPI (PCAM)](pcam.md)
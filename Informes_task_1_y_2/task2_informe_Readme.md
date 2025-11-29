
# Dgesv Benchmarking with GCC 
**Master HPC – Block 1, Task 2**

## 1. Metodología y caracteristicas de los entornos 

Se ha evaluado el rendimiento de la implementación propia de `dgesv()` desarrollada en la Task 1 utilizando tres versiones del compilador GCC disponibles en el entorno CESGA:

| version GCC | modulo       | Disponibilidad OpenBLAS/LAPACK                        |
| ----------- | ------------ | ----------------------------------------------------- |
| 8.4.1       | cesga/system | sin OpenBLAS, versión alternativa del main sin LAPACK |
| 10.1.0      | cesga/2020   | OpenBLAS disponible                                   |
| 11.4.0      | cesga/2025   | OpenBLAS disponible                                   |

Las pruebas se realizaron sobre matrices cuadradas de tamaño 1024², 2048² y 4096², ejecutando el binario 5 veces y tomando la mediana del tiempo medido dentro del main (en milisegundos). Para realizar esto de manera automatica se ha escrito un pequeño script denominado benchmark_dgesv.sh



## 2. Resultados

Viene en el modulo cesga/system. Este no tiene disponible el modulo openblas. Por ello no escribo el tiempo de referencia.

## GCC 8.4.1

| Matrix size | -O0      | -O2 (no-vec) | -O3-vec   | -Ofast-vec | Ref |
| ----------- | -------- | ------------ | --------- | ---------- | --- |
| 1024×1024   | 7394 ms  | 1396 ms      | 800 ms    | 775 ms     |     |
| 2048×2048   | 60190 ms | 14710 ms     | 12009 ms  | 11761 ms   |     |
| 4096×4096   | 484881   | 121544 ms    | 101420 ms | 98940 ms   |     |

---

## GCC 10.1.0

| Matrix size | -O0       | -O2 (no-vec) | -O3-vec   | -Ofast-vec | Ref     |
| ----------- | --------- | ------------ | --------- | ---------- | ------- |
| 1024×1024   | 7389 ms   | 1397 ms      | 787 ms    | 787 ms     | 61 ms   |
| 2048×2048   | 60231 ms  | 14530 ms     | 11765 ms  | 11650 ms   | 304 ms  |
| 4096×4096   | 484792 ms | 122265 ms    | 104507 ms | 100639 ms  | 1351 ms |

---

## GCC 11.4.0

| Matrix size | -O0       | -O2 (no-vec) | -O3-vec  | -Ofast-vec | Ref     |
| ----------- | --------- | ------------ | -------- | ---------- | ------- |
| 1024×1024   | 7714 ms   | 1179 ms      | 717 ms   | 708 ms     | 111 ms  | 
| 2048×2048   | 62680 ms  | 13872 ms     | 11529 ms | 11711 ms   | 293 ms  |
| 4096×4096   | 502126 ms | 115076 ms    | 98513 ms | 106975 ms  | 1391 ms |


---

## 3. Comentarios sobre el analisis

El salto de `-O0` a `-O2` reduce los tiempos en un factor 5 - 6 gracias a la eliminación de código redundante y la propagación de constantes.

El paso a `-O3` aporta una mejora adicional (≈10–15%), mientras que `-Ofast` es apenas imperceptible (1–2%).

Esto indica que las optimizaciones vectoriales y matemáticas agresivas tienen efecto limitado en este algoritmo, dominado por operaciones dependientes entre filas.

Por otro lado, los tiempos crecen de manera aproximadamente O(n³), coherente con el coste teórico del método de Gauss con pivoteo parcial.

La implementación de LAPACK/OpenBLAS es unas 60 veces más rápida que el algoritmo sin optimizar, lo esperado dado que emplea operaciones altamente optimizadas y vectorizadas.

## 4. Autovectorizacion

Se generó el informe con:
```bash
gcc -O3 -ftree-vectorize -fopt-info-vec-optimized -fopt-info-vec-missed dgesv.c -c
```

Comparando los informes de vectorización entre GCC 8.4.1, 10.1.0 y 11.4.0, se observa una evolución clara en la capacidad del compilador para identificar bucles autovectorizables.
En GCC 8.4.1, la mayoría de bucles fallan por “bad loop form” y “not enough data-refs”, mientras que a partir de GCC 10 la herramienta logra vectorizar correctamente los bucles internos más simples. No obstante, tampoco se ve un gran impacto en el tiempo de ejecución.

La versión 11.4.0 y 10.0.1 generan informes más limpios y una vectorización más estable, aunque el conjunto de bucles vectorizados es esencialmente el mismo. La ligera mejora de rendimiento observada probablemente entre ambas versiones no parece deberse en si a la optimización, quiza tiene que ver con con un manejo mas eficiente de la memoria y el scheduling.
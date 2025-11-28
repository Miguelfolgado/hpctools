#include "dgesv.h"
#include <math.h>
#include <stdio.h>

// Mi solución escala mucho peor que Lapack. Para sistemas de 1024x1024 unas 60 veces mas lento

int my_dgesv(int n, int nrhs, double *a, double *b) {
    for (int k = 0; k < n; k++)
    {
        // Buscamos pivote en columna k
        int maxRow = k;
	
        for (int i = k+1; i < n; i++)
	{
            if (fabs(a[i*n + k]) > fabs(a[maxRow*n + k]))
                maxRow = i;
        }


        // Intercambiamos filas si el pivote no esta en k
	// Si no lo hacemos el algoritmo se vuelve inestable
	
        if (maxRow != k)
	{
            for (int j = 0; j < n; j++)
	    {
                double tmp = a[k*n + j];
                a[k*n + j] = a[maxRow*n + j];
                a[maxRow*n + j] = tmp;
            }
            for (int j = 0; j < nrhs; j++)
	    {
                double tmp = b[k*nrhs + j];
                b[k*nrhs + j] = b[maxRow*nrhs + j];
                b[maxRow*nrhs + j] = tmp;
            }
        }
	
	
        // Dividimos la fila para normalizar a 1 el pivote
        double pivote = a[k*n + k];
        for (int j = 0; j < n; j++) a[k*n + j] = a[k*n + j] / pivote;
        for (int j = 0; j < nrhs; j++) b[k*nrhs + j] = b[k*nrhs + j] / pivote;

	
        // Eliminamos la columna k en todas las filas
        for (int i = 0; i < n; i++)
	    {
            if (i == k) continue;
            double factor = a[i*n + k];
            if (factor != 0.0)
	    {
                for (int j = 0; j < n; j++)
                    a[i*n + j] = a[i*n + j] - factor * a[k*n + j];
                for (int j = 0; j < nrhs; j++)
                    b[i*nrhs + j] = b[i*nrhs + j] - factor * b[k*nrhs + j];
            }
        }
    }

  return 0;
}
#!/bin/bash
# Nombre: benchmark_dgesv.sh
# Autor: Miguel Garcia Folgado
# Descripcion: Ejecuta ./dgesv SIZE N veces y calcula la mediana del tiempo de correspondiente a la funcion dgesv

EXEC="./dgesv"
SIZE=${1:-1024}   # Si no lo pasas, error
N=5              # Con 10 veces el resultado ya es estable. Y con 5 o 3 tambien

# Lo primero, tenemos size?
if [ $# -lt 1 ]; then
  echo "Error: No se indico SIZE de la matriz. Por favor indicalo."
  echo "Ejemplo (1024 x 1024): ./benchmark_dgesv.sh 1024."
  exit 1
fi


if [ ! -x "$EXEC" ]; then
  echo "Error: no se encuentra el ejecutable '$EXEC'"
  exit 1
fi

echo "Ejecutando $N veces $EXEC $SIZE ..."
TIMES=()

for i in $(seq 1 $N); do
  # Ejecutamos N veces
  T=$($EXEC $SIZE | grep "my dgesv solver" | grep -Eo '[0-9]+' | tail -1)
  echo "Ejecución $i: ${T} ms"
  TIMES+=($T)
done

# Calculamos mediana
SORTED=($(printf '%s\n' "${TIMES[@]}" | sort -n))
MID=$((N / 2))

if (( N % 2 == 1 )); then
  # Si son impares es el valor central. Creo que esta bien, bash redondea siempre hacia bajo
  MEDIAN=${SORTED[$MID]}
else
  # Si son pares, la media de los dos centrales.
  MEDIAN=$(echo "scale=4; (${SORTED[$MID-1]} + ${SORTED[$MID]}) / 2" | bc)
fi

echo "------------------------------------"
echo "Mediana del tiempo de de dgesv: ${MEDIAN} ms"


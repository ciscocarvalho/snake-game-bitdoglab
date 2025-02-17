#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <constants.h>
#include "../inc/types.h"
#include "pico/time.h"

// =============================================================
// UTILS
// funções úteis no geral
// =============================================================

// entrega uma mensagem de erro de alocação de memória e encerra o programa
// imediatamente, útil para tratar de forma fácil os possíveis casos de falha
// de alocação dinâmica.
void memory_allocation_error() {
  fprintf(stderr, "Failed to allocate memory.\n");
  exit(EXIT_FAILURE);
}

// copia os valores de um array de posição para outro
void copy_position(Position source, Position target) {
  target[0] = source[0];
  target[1] = source[1];
}

// pega a direção oposta a outra direção
Direction get_opposite_direction(Direction direction) {
  switch (direction) {
    case DIRECTION_NORTH: return DIRECTION_SOUTH;
    case DIRECTION_EAST: return DIRECTION_WEST;
    case DIRECTION_SOUTH: return DIRECTION_NORTH;
    case DIRECTION_WEST: return DIRECTION_EAST;
    default: return -1;
  }
}

// checa se duas posições colidem
bool positions_collide(Position position1, Position position2) {
  int row1 = position1[0], col1 = position1[1];
  int row2 = position2[0], col2 = position2[1];

  return row1 == row2 && col1 == col2;
}

// circularidade de um inteiro dentro de um intervalo, útil para fazer um valor
// "teleportar" para o outro lado se ele saiu do limite. Especialmente útil
// para fazer a cobrinha teleportar para outro lado do canvas.
int wrap(int n, int min, int max) {
  if (n < min) {
    n = max;
  } else if (n > max) {
    n = min;
  }

  return n;
}

// pega um valor inteiro pseudo aleatório (um algoritmo verdadeiramente
// aleatório não vale a pena devido à complexidade) no intervalo inclusivo
// [min, max], útil para escolhas aleatórias.
int randint(int min, int max) {
  if (min > max) {
    fprintf(stderr, "randint called with min (%i) greater than max (%i).\n", min, max);
    exit(EXIT_FAILURE);
  }

  static bool initilized = false;

  if (!initilized) {
    srand(time(NULL));
  }

  return min + rand() % (max - min + 1);
}

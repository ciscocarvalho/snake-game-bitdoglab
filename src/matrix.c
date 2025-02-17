#include <malloc.h>
#include "../inc/matrix.h"
#include "../inc/utils.h"

// ===========================================================================
// MATRIX
// Uma abstração da estrutura de dados matriz.
// ===========================================================================

// inicia a matriz
Matrix* matrix_init(int n_rows, int n_cols) {
  Matrix* matrix = malloc(sizeof(Matrix));

  if (matrix == NULL) {
    memory_allocation_error();
  }

  matrix->rows = n_rows;
  matrix->cols = n_cols;

  MatrixDataType** data = malloc(sizeof(MatrixDataType*) * n_rows);

  if (data == NULL) {
    memory_allocation_error();
  }

  for (int i = 0; i < n_rows; i++) {
    data[i] = calloc(n_cols, sizeof(MatrixDataType));

    if (data[i] == NULL) {
      memory_allocation_error();
    }
  }

  matrix->data = data;

  return matrix;
};

// pega o valor de uma certa posição da matriz
MatrixDataType matrix_get(Matrix* matrix, MatrixPosition position) {
  int row = position[0], col = position[1];
  return matrix->data[row][col];
}

// preenche uma posição da matriz
void matrix_put(Matrix* matrix, MatrixPosition position, MatrixDataType val) {
  int row = position[0], col = position[1];
  matrix->data[row][col] = val;
}

// libera a memória alocada para a matriz
void matrix_free(Matrix* matrix) {
  if (matrix == NULL) {
    return;
  }

  for (int i = 0; i < matrix->rows; i++) {
      free(matrix->data[i]);
  }

  free(matrix->data);
  free(matrix);
}

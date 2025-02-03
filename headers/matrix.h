#pragma once

typedef int MatrixDataType;

typedef struct Matrix {
  int rows;
  int cols;
  MatrixDataType** data;
} Matrix;

typedef int MatrixPosition[2];

Matrix* matrix_init(int n_rows, int n_cols);

MatrixDataType matrix_get(Matrix* matrix, MatrixPosition position);

void matrix_put(Matrix* matrix, MatrixPosition position, MatrixDataType val);

void matrix_free(Matrix* matrix);

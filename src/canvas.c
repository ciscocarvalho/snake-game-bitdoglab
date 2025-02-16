#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include "../inc/types.h"
#include "../inc/utils.h"
#include "../inc/constants.h"
#include "../inc/matrix.h"
#include "../inc/neopixel.h"

typedef Matrix Canvas;
typedef MatrixDataType CanvasCell;
typedef MatrixPosition CanvasPosition;

static bool is_position_free(Canvas* canvas, Position position) {
  int row = position[0], col = position[1];
  return (canvas->data[row][col] == CELL_UNUSED);
}

Position* canvas_get_free_positions(Canvas* canvas, size_t* size) {
  *size = 0;
  Position* positions = NULL;

  for (int row = 0; row < canvas->rows; row++) {
    for (int col = 0; col < canvas->cols; col++) {
      if (is_position_free(canvas, (int [2]){ row, col })) {
        (*size)++;
        positions = realloc(positions, (*size) * sizeof(Position));

        if (positions == NULL) {
          memory_allocation_error();
        }

        copy_position((int [2]){ row, col }, positions[*size - 1]);
      }
    }
  }

  return positions;
}

void canvas_get_random_free_position(Canvas* canvas, Position position) {
  size_t positions_size;

  Position* free_positions = canvas_get_free_positions(canvas, &positions_size);

  if (positions_size > 0 && free_positions != NULL) {
    int position_index = randint(0, (int) positions_size - 1);
    copy_position(free_positions[position_index], position);
    free(free_positions);
  } else {
    fprintf(stderr, "Function canvas_get_random_free_position() called but there is no free position.\n");
    exit(EXIT_FAILURE);
  }
}

int canvas_count_free_positions(Canvas* canvas) {
  int count = 0;

  for (int row = 0; row < canvas->rows; row++) {
    for (int col = 0; col < canvas->cols; col++) {
      if (is_position_free(canvas, (int [2]){ row, col })) {
        count++;
      }
    }
  }

  return count;
}

static void gen_sprite(Canvas* canvas, int sprite[5][5][3]) {
  for (int row = 0; row < canvas->rows; row++) {
    for (int col = 0; col < canvas->cols; col++) {
      int cell = canvas->data[row][col];

      switch (cell) {
        case CELL_UNUSED: {
          copy_color((int [3]){ 0, 0, 0 }, sprite[row][col]);
          break;
        } case CELL_SNAKE_HEAD: {
          copy_color((int [3]){ 1, 1, 1 }, sprite[row][col]);
          break;
        } case CELL_FOOD: {
          copy_color((int [3]){ 1, 0, 0 }, sprite[row][col]);
          break;
        } case CELL_SNAKE_BODY: {
          copy_color((int [3]){ 0, 0, 1 }, sprite[row][col]);
          break;
        } default: {
          copy_color((int [3]){ 0, 1, 0 }, sprite[row][col]);
          break;
        }
      }
    }
  }
}

void canvas_render(Canvas* canvas) {
  npClear();

  int sprite[5][5][3];
  gen_sprite(canvas, sprite);
  setSpriteLEDs(sprite);

  npWrite();
}

CanvasCell canvas_get(Canvas *canvas, CanvasPosition position) {
  int row = position[0];
  int col = position[1];
  return canvas->data[row][col];
};

void canvas_put(Canvas *canvas, CanvasCell cell, CanvasPosition position) {
  matrix_put(canvas, position, cell);
};

void canvas_clear(Canvas *canvas) {
  for (int i = 0; i < canvas->rows; i++) {
    for (int j = 0; j < canvas->cols; j++) {
      canvas_put(canvas, CELL_UNUSED, (int[2]){ i, j });
    };
  };
}

Canvas* canvas_init(int n_rows, int n_cols) {
  Canvas* canvas = matrix_init(n_rows, n_cols);
  canvas_clear(canvas);
  return canvas;
}

void canvas_free(Canvas* canvas) {
  matrix_free(canvas);
}

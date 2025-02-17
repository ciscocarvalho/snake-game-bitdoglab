#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include "../inc/utils.h"
#include "../inc/food.h"
#include "../inc/canvas.h"

// =============================================================
// FOOD
// Representa a comida que a cobrinha precisa comer para crescer
// =============================================================

// pega a próxima posição em que a comida vai aparecer
static void get_next_position(Food* food, Canvas* canvas, Position next_position) {
  canvas_get_random_free_position(canvas, next_position);
};

// inicia o struct da comida
Food* food_init(Canvas* canvas) {
  Food* food = malloc(sizeof(Food));

  if (food == NULL) {
    memory_allocation_error();
  }

  Position next_position;
  get_next_position(food, canvas, next_position);
  copy_position(next_position, food->position);
  food_put(food, canvas);

  return food;
}

// remove a comida do canvas
void food_remove(Food* food, Canvas* canvas) {
  if (canvas_get(canvas, food->position) == CELL_FOOD) {
    canvas_put(canvas, CELL_UNUSED, food->position);
  }

  food->in_canvas = false;
}

// move a comida para outra parte do canvas
void food_move(Food* food, Canvas* canvas) {
  Position next_position;
  get_next_position(food, canvas, next_position);

  food_remove(food, canvas);
  copy_position(next_position, food->position);
  food_put(food, canvas);
}

// coloca a comida no canvas
void food_put(Food* food, Canvas* canvas) {
  canvas_put(canvas, CELL_FOOD, food->position);
  food->in_canvas = true;
};

// libera a memória alocada para a comida
void food_free(Food* food) {
  free(food);
}

#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include "../headers/utils.h"
#include "../headers/apple.h"
#include "../headers/canvas.h"

static void get_next_position(Apple* apple, Canvas* canvas, Position next_position) {
  canvas_get_random_free_position(canvas, next_position);
};

Apple* apple_init(Canvas* canvas) {
  Apple* apple = malloc(sizeof(Apple));

  if (apple == NULL) {
    memory_allocation_error();
  }

  Position next_position;
  get_next_position(apple, canvas, next_position);
  copy_position(next_position, apple->position);
  apple_put(apple, canvas);

  return apple;
}

void apple_remove(Apple* apple, Canvas* canvas) {
  if (canvas_get(canvas, apple->position) == CELL_APPLE) {
    canvas_put(canvas, CELL_UNUSED, apple->position);
  }

  apple->in_canvas = false;
}

void apple_move(Apple* apple, Canvas* canvas) {
  Position next_position;
  get_next_position(apple, canvas, next_position);

  apple_remove(apple, canvas);
  copy_position(next_position, apple->position);
  apple_put(apple, canvas);
}

void apple_put(Apple* apple, Canvas* canvas) {
  canvas_put(canvas, CELL_APPLE, apple->position);
  apple->in_canvas = true;
};

void apple_free(Apple* apple) {
  free(apple);
}

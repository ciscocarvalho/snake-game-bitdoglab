#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include "../headers/utils.h"
#include "../headers/snake.h"
#include "../headers/constants.h"
#include "../headers/canvas.h"

static void snake_remove(Snake* snake, Canvas* canvas) {
  for (int i = 0; i < snake->size; i++) {
    canvas_put(canvas, CELL_UNUSED, snake->node_positions[i]);
  }
}

bool snake_node_is_head(Snake* snake, int node_index) {
  return node_index == 0;
}

static void snake_put(Snake* snake, Canvas* canvas) {
  // By filling the cells from the last node to the first (head), the snake's
  // head takes priority over the body in case of a collision, making the head
  // appear to pass "above" the body when collisions are allowed (think of this
  // as, for example, a way to implement an easier game mode).
  for (int i = snake->size - 1; i >= 0; i--) {
    CanvasCell cell = snake_node_is_head(snake, i) ? CELL_SNAKE_HEAD : CELL_SNAKE_BODY;
    canvas_put(canvas, cell, snake->node_positions[i]);
  }
}

static void snake_get_previous_node_position(Snake* snake, int node_index, Position previous_node_position) {
  copy_position(snake->node_positions[node_index - 1], previous_node_position);
}

static void get_relative_position(Canvas* canvas, Position node_position, Direction direction, Position relative_position) {
  int row = node_position[0], col = node_position[1];

  switch (direction) {
    case DIRECTION_NORTH: row = wrap(row - 1, 0, canvas->rows - 1); break;
    case DIRECTION_EAST: col = wrap(col + 1, 0, canvas->cols - 1); break;
    case DIRECTION_SOUTH: row = wrap(row + 1, 0, canvas->rows - 1); break;
    case DIRECTION_WEST: col = wrap(col - 1, 0, canvas->cols - 1); break;
    default: break;
  }

  copy_position((int [2]){ row, col }, relative_position);
}

static Direction get_node_direction(Snake* snake, int node_index) {
  Position node_position;
  copy_position(snake->node_positions[node_index], node_position);

  if (snake_node_is_head(snake, node_index)) {
    return snake->direction;
  } else {
    Position previous_node_position;
    snake_get_previous_node_position(snake, node_index, previous_node_position);

    if (previous_node_position[0] < node_position[0]) {
      return DIRECTION_NORTH;
    } else if (previous_node_position[0] > node_position[0]) {
      return DIRECTION_SOUTH;
    } else if (previous_node_position[1] < node_position[1]) {
      return DIRECTION_WEST;
    } else if (previous_node_position[1] > node_position[1]) {
      return DIRECTION_EAST;
    } else {
      fprintf(stderr, "Function get_node_direction() found overlapping nodes.\n");
      exit(EXIT_FAILURE);
    }
  }
};

static Direction get_opposite_direction(Direction direction) {
  switch (direction) {
    case DIRECTION_NORTH: return DIRECTION_SOUTH;
    case DIRECTION_EAST: return DIRECTION_WEST;
    case DIRECTION_SOUTH: return DIRECTION_NORTH;
    case DIRECTION_WEST: return DIRECTION_EAST;
    default: return -1;
  }
}

void snake_get_head_position(Snake* snake, Position head_position) {
  copy_position(snake->node_positions[0], head_position);
}

void snake_get_new_node_position(Snake* snake, Canvas* canvas, Position node_position) {
  Direction last_node_direction = get_node_direction(snake, snake->size - 1);
  get_relative_position(
    canvas,
    snake->node_positions[snake->size - 1],
    get_opposite_direction(last_node_direction),
    node_position
  );
}

void snake_grow(Snake *snake, Canvas* canvas) {
  snake_remove(snake, canvas);

  Position new_node_position;
  snake_get_new_node_position(snake, canvas, new_node_position);

  snake->size++;
  snake->node_positions = realloc(snake->node_positions, (snake->size) * sizeof(Position));

  if (snake->node_positions == NULL) {
    memory_allocation_error();
  }

  copy_position(new_node_position, snake->node_positions[snake->size - 1]);

  snake_put(snake, canvas);
}

static void snake_get_initial_position(Canvas* canvas, Position position) {
  return canvas_get_random_free_position(canvas, position);
}

void snake_free(Snake* snake) {
  if (snake == NULL) {
    return;
  }

  free(snake->node_positions);
  free(snake);
}

void get_next_node_position(Snake* snake, Canvas* canvas, int node_index, Position next_node_position) {
  if (snake_node_is_head(snake, node_index)) {
    Direction direction = get_node_direction(snake, node_index);
    get_relative_position(canvas, snake->node_positions[node_index], direction, next_node_position);
  } else {
    copy_position(snake->node_positions[node_index - 1], next_node_position);
  }
}

static void snake_move_node(Snake* snake, Canvas* canvas, int node_index) {
  get_next_node_position(snake, canvas, node_index, snake->node_positions[node_index]);
}

void snake_move(Snake* snake, Canvas* canvas) {
  snake_remove(snake, canvas);

  for (int i = snake->size - 1; i >= 0; i--) {
    snake_move_node(snake, canvas, i);
  }

  snake_put(snake, canvas);
}

static bool snake_collides(Snake* snake, Position position) {
  for (int i = 0; i < snake->size; i++) {
    if (positions_collide(position, snake->node_positions[i])) {
      return true;
    }
  }

  return false;
}

bool snake_self_collides(Snake* snake) {
  for (int i = 1; i < snake->size; i++) {
    if (positions_collide(snake->node_positions[0], snake->node_positions[i])) {
      return true;
    }
  }

  return false;
}

Snake* snake_init(Canvas* canvas, Position position, Direction direction, int initial_size) {
  if (
      initial_size < 1 ||
      (initial_size > canvas->rows && (direction == DIRECTION_NORTH || direction == DIRECTION_SOUTH)) ||
      (initial_size > canvas->cols && (direction == DIRECTION_EAST || direction == DIRECTION_WEST))
  ) {
    fprintf(stderr, "Invalid initial snake initial_size: %i.\n", initial_size);
    exit(EXIT_FAILURE);
  }

  Snake* snake = malloc(sizeof(Snake));

  if (snake == NULL) {
    memory_allocation_error();
  }

  snake->size = 1;
  snake->node_positions = malloc(snake->size * sizeof(Position));

  if (snake->node_positions == NULL) {
    memory_allocation_error();
  }

  copy_position(position, snake->node_positions[0]);
  snake->direction = direction;

  while (snake->size < initial_size) {
    snake_grow(snake, canvas);
  }

  snake_put(snake, canvas);

  return snake;
}

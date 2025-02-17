#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include "../inc/utils.h"
#include "../inc/snake.h"
#include "../inc/constants.h"
#include "../inc/canvas.h"

// =================================================================================
// SNAKE
// Representação da cobra, contém várias funções análogas as de ./food.c.
// "node" neste arquivo significa uma parte da cobrinha, podendo ser a cabeça
// ou uma parte do corpo.
// =================================================================================

// remove a cobra do canvas
void snake_remove(Snake* snake, Canvas* canvas) {
  for (int i = 0; i < snake->size; i++) {
    Position *node_position = &snake->node_positions[i];
    CanvasCell cell = canvas_get(canvas, *node_position);

    if (cell == CELL_SNAKE_BODY || cell == CELL_SNAKE_HEAD) {
      canvas_put(canvas, CELL_UNUSED, *node_position);
    }
  }

  snake->in_canvas = false;
}

// diz se o node na posição node_index é a cabeça da cobra
bool snake_node_is_head(Snake* snake, int node_index) {
  return node_index == 0;
}

// coloca a cobra no canvas
static void snake_put(Snake* snake, Canvas* canvas) {
  // By filling the cells from the last node to the first (head), the snake's
  // head takes priority over the body in case of a collision, making the head
  // appear to pass "above" the body when collisions are allowed (think of this
  // as, for example, a way to implement an easier game mode).
  for (int i = snake->size - 1; i >= 0; i--) {
    CanvasCell cell = snake_node_is_head(snake, i) ? CELL_SNAKE_HEAD : CELL_SNAKE_BODY;
    canvas_put(canvas, cell, snake->node_positions[i]);
  }

  snake->in_canvas = true;
}

// pega a posição do node anterior a outro node
static void snake_get_previous_node_position(Snake* snake, int node_index, Position previous_node_position) {
  copy_position(snake->node_positions[node_index - 1], previous_node_position);
}

// pega uma posição relativa à posição de outro node, usando uma direção como
// referência, e coloca a posição no array relative_position.
// Por exemplo: se node_position é (1, 1) e direction é DIRECTION_NORTH, a
// posição relativa é a posição norte à (1, 1), ou seja, (0, 1).
// Quando o valor de uma linha ou coluna sai do intervalo válido do canvas, é
// feita uma circularidade, por exemplo: em vez de entregar a posição (-1, 1) a
// função entregará a posição (4, 1) (assumindo um canvas de tamanho 5x5).
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

// pega a direção de um node
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

// pega a posição da cabeça da cobra
void snake_get_head_position(Snake* snake, Position head_position) {
  copy_position(snake->node_positions[0], head_position);
}

// pega a posição em que será inserido um novo node, assume-se que pelo a
// cabeça da cobra já exista.
void snake_get_new_node_position(Snake* snake, Canvas* canvas, Position node_position) {
  Direction last_node_direction = get_node_direction(snake, snake->size - 1);
  get_relative_position(
    canvas,
    snake->node_positions[snake->size - 1],
    get_opposite_direction(last_node_direction),
    node_position
  );
}

// faz a cobra crescer
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

// pega a posição inicial da cobra
static void snake_get_initial_position(Canvas* canvas, Position position) {
  return canvas_get_random_free_position(canvas, position);
}

// libera a memória alocada para a cobra
void snake_free(Snake* snake) {
  if (snake == NULL) {
    return;
  }

  free(snake->node_positions);
  free(snake);
}

// pega a posição do próximo node
void get_next_node_position(Snake* snake, Canvas* canvas, int node_index, Position next_node_position) {
  if (snake_node_is_head(snake, node_index)) {
    Direction direction = get_node_direction(snake, node_index);
    get_relative_position(canvas, snake->node_positions[node_index], direction, next_node_position);
  } else {
    copy_position(snake->node_positions[node_index - 1], next_node_position);
  }
}

// move um node na direção que a cobra está indo
static void snake_move_node(Snake* snake, Canvas* canvas, int node_index) {
  get_next_node_position(snake, canvas, node_index, snake->node_positions[node_index]);
}

// move a cobra inteira
void snake_move(Snake* snake, Canvas* canvas) {
  snake_remove(snake, canvas);

  for (int i = snake->size - 1; i >= 0; i--) {
    snake_move_node(snake, canvas, i);
  }

  snake_put(snake, canvas);
}

// checa se a cobra colide com uma posição específica
static bool snake_collides(Snake* snake, Position position) {
  for (int i = 0; i < snake->size; i++) {
    if (positions_collide(position, snake->node_positions[i])) {
      return true;
    }
  }

  return false;
}

// checa se a cobra colide consigo mesma
bool snake_self_collides(Snake* snake) {
  for (int i = 1; i < snake->size; i++) {
    if (positions_collide(snake->node_positions[0], snake->node_positions[i])) {
      return true;
    }
  }

  return false;
}

// inicia a cobra
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

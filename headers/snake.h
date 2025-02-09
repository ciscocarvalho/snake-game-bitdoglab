#pragma once

#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "../headers/types.h"
#include "../headers/constants.h"
#include "../headers/canvas.h"

typedef struct Snake {
  Position* node_positions;
  Direction direction;
  bool in_canvas;
  int size;
} Snake;

bool snake_node_is_head(Snake* snake, int node_index);

void snake_get_head_position(Snake* snake, Position head_position);

void get_next_node_position(Snake* snake, Canvas* canvas, int node_index, Position next_node_position);

void snake_get_new_node_position(Snake* snake, Canvas* canvas, Position node_position);

void snake_grow(Snake *snake, Canvas* canvas);

void snake_free(Snake* snake);

void snake_remove(Snake* snake, Canvas* canvas);

void snake_move(Snake* snake, Canvas* canvas);

bool snake_self_collides(Snake* snake);

Snake* snake_init(Canvas* canvas, Position position, Direction direction, int size);

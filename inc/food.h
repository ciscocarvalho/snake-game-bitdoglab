#pragma once

#include "./types.h"
#include "./canvas.h"

typedef struct Food {
  Position position;
  bool in_canvas;
} Food;

Food* food_init(Canvas* canvas);

void food_put(Food* food, Canvas* canvas);

void food_remove(Food* food, Canvas* canvas);

void food_move(Food* food, Canvas* canvas);

void food_free(Food* food);

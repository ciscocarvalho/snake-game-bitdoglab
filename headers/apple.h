#pragma once

#include "../headers/types.h"
#include "../headers/canvas.h"

typedef struct Apple {
  Position position;
  bool in_canvas;
} Apple;

Apple* apple_init(Canvas* canvas);

void apple_put(Apple* apple, Canvas* canvas);

void apple_remove(Apple* apple, Canvas* canvas);

void apple_move(Apple* apple, Canvas* canvas);

void apple_free(Apple* apple);

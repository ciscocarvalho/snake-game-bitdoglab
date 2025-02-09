#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "../headers/types.h"
#include "../headers/constants.h"
#include "../headers/matrix.h"

typedef Matrix Canvas;
typedef MatrixDataType CanvasCell;
typedef MatrixPosition CanvasPosition;

void canvas_render(Canvas* canvas);

CanvasCell canvas_get(Canvas *canvas, CanvasPosition position);

void canvas_put(Canvas *canvas, CanvasCell cell, CanvasPosition position);

void canvas_clear(Canvas *canvas);

Canvas* canvas_init(int n_rows, int n_cols);

void canvas_free(Canvas* canvas);

Position* canvas_get_free_positions(Canvas* canvas, size_t* size);

void canvas_get_random_free_position(Canvas* canvas, Position position);

int canvas_count_free_positions(Canvas* canvas);

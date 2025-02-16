#pragma once

#include <stdbool.h>
#include "types.h"

void memory_allocation_error();

void copy_position(Position source, Position target);

Direction get_opposite_direction(Direction direction);

int wrap(int n, int min, int max);

int randint(int min, int max);

bool positions_collide(Position position1, Position position2);

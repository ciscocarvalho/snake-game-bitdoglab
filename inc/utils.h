#pragma once

#include <stdbool.h>
#include "types.h"
#include "../inc/display_oled/ssd1306.h"
#include "../inc/menu_text.h"

void memory_allocation_error();

void copy_position(Position source, Position target);

Direction get_opposite_direction(Direction direction);

int wrap(int n, int min, int max);

int randint(int min, int max);

bool positions_collide(Position position1, Position position2);

void display_show_lines(uint8_t *ssd, uint8_t ssd_size, char* lines[], uint8_t lines_size, RenderArea frame_area);
void display_show_line(uint8_t *ssd, uint8_t ssd_size, char* line, RenderArea frame_area);

void display_menu_text(MenuText menu_text, uint8_t *ssd, RenderArea frame_area);

bool is_button_down(uint8_t button);

int wait_button_a_or_b();

void pwm_init_buzzer(uint pin);

uint wait_menu_text_choice(MenuText* menu_text, uint8_t* ssd, RenderArea render_area);
#pragma once

#include "pico/types.h"

typedef struct {
    char* label;
    uint action;
    bool selected;
} MenuOption;

typedef struct {
    char** header;
    size_t header_size;
} MenuTextHeader;

typedef struct {
    char** footer;
    size_t footer_size;
} MenuTextFooter;

typedef struct {
    MenuTextHeader header;
    MenuTextFooter footer;
    MenuOption* options;
    size_t options_size;
} MenuText;

typedef struct {
    char** lines;
    size_t lines_size;
} MenuTextView;

MenuText* menu_text_create(MenuOption* options, size_t options_size);
void menu_text_free(MenuText* menu_text);

MenuTextView* menu_text_view_create(MenuText menu_text);
void menu_text_view_free(MenuTextView* menu_text_view);

MenuOption menu_text_get_selected_option(MenuText menu_text);

void menu_text_move_selection_down(MenuText* menu_text);
void menu_text_move_selection_up(MenuText* menu_text);
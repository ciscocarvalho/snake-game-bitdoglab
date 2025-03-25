#include "pico/stdlib.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "pico/stdio.h"
#include "../inc/menu_text.h"
#include "../inc/utils.h"

MenuText* menu_text_create(MenuOption* options, size_t options_size) {
    MenuText* menu_text = malloc(sizeof(MenuText));

    char** header_lines = NULL;

    menu_text->header = (MenuTextHeader) {
        .header = header_lines,
        .header_size = 0,
    };

    char** footer_lines = NULL;

    menu_text->footer = (MenuTextFooter) {
        .footer = footer_lines,
        .footer_size = 0,
    };

    menu_text->options = options;
    menu_text->options_size = options_size;

    return menu_text;
}

void menu_text_view_free(MenuTextView* menu_text_view) {
    if (menu_text_view == NULL) {
        return;
    }

    free(menu_text_view);
};

void menu_text_free(MenuText* menu_text) {
    if (menu_text == NULL) {
        return;
    }

    if (menu_text->header.header != NULL) {
        free(menu_text->header.header);
    }

    if (menu_text->options != NULL) {
        free(menu_text->options);
    }

    if (menu_text->footer.footer != NULL) {
        free(menu_text->footer.footer);
    }

    free(menu_text);
}

static char* format_selected_option_label(char* label) {
    char* preffix = "> ";
    char* suffix = " <";
    size_t new_str_len = strlen(preffix) + strlen(label) + strlen(suffix);
    char* new_str = malloc(new_str_len * sizeof(char));
    new_str = asnprintf(new_str, &new_str_len, "%s%s%s", preffix, label, suffix);
    return new_str;
}

MenuTextView* menu_text_view_create(MenuText menu_text) {
    char** lines = NULL;
    uint lines_size = 0;

    for (int i = 0; i < menu_text.header.header_size; i++) {
        lines_size++;
        lines = realloc(lines, lines_size * sizeof(char *));
        if (lines == NULL) {
            memory_allocation_error();
        }
        lines[lines_size - 1] = menu_text.header.header[i];
    }

    for (int i = 0; i < menu_text.options_size; i++) {
        lines_size++;
        lines = realloc(lines, lines_size * sizeof(char *));
        if (lines == NULL) {
            memory_allocation_error();
        }

        MenuOption option = menu_text.options[i];
        char* label = option.label;

        if (option.selected) {
            lines[lines_size - 1] = format_selected_option_label(label);
        } else {
            lines[lines_size - 1] = label;
        }
    }

    for (int i = 0; i < menu_text.footer.footer_size; i++) {
        lines_size++;
        lines = realloc(lines, lines_size * sizeof(char *));
        if (lines == NULL) {
            memory_allocation_error();
        }
        lines[lines_size - 1] = menu_text.footer.footer[i];
    }

    MenuTextView* mtv = malloc(sizeof(MenuTextView));

    mtv->lines = lines;
    mtv->lines_size = lines_size;

    return mtv;
};

MenuOption menu_text_get_selected_option(MenuText menu_text) {
    for (int i = 0; i < menu_text.options_size; i++) {
        MenuOption option = menu_text.options[i];

        if (option.selected) {
            return option;
        }
    }

    fprintf(stderr, "No option selected");
    exit(EXIT_FAILURE);
}

static void menu_text_move_selection(MenuText* menu_text, bool up) {
    int selected_option_index = -1;
    MenuOption* option;

    for (int i = 0; i < menu_text->options_size; i++) {
        option = &menu_text->options[i];

        if (option->selected) {
            selected_option_index = i;
            break;
        }
    }

    if (selected_option_index == -1) {
        fprintf(stderr, "Can't move selection %s because there is no option currently selected\n", up ? "up" : "down");
        exit(EXIT_FAILURE);
    }

    menu_text->options[selected_option_index].selected = false;

    uint next_option_index = wrap(selected_option_index + (up ? -1 : 1), 0, menu_text->options_size - 1);
    menu_text->options[next_option_index].selected = true;
}

void menu_text_move_selection_down(MenuText* menu_text) {
    menu_text_move_selection(menu_text, false);
}

void menu_text_move_selection_up(MenuText* menu_text) {
    menu_text_move_selection(menu_text, true);
}
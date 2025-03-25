#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <constants.h>
#include "../inc/types.h"
#include "pico/time.h"
#include "../inc/display_oled/ssd1306.h"
#include "hardware/pwm.h"
#include "../inc/menu_text.h"
#include <string.h>

// =============================================================
// UTILS
// funções úteis no geral
// =============================================================

// entrega uma mensagem de erro de alocação de memória e encerra o programa
// imediatamente, útil para tratar de forma fácil os possíveis casos de falha
// de alocação dinâmica.
void memory_allocation_error() {
  fprintf(stderr, "Failed to allocate memory.\n");
  printf("Failed to allocate memory.\n");
  exit(EXIT_FAILURE);
}

// copia os valores de um array de posição para outro
void copy_position(Position source, Position target) {
  target[0] = source[0];
  target[1] = source[1];
}

// pega a direção oposta a outra direção
Direction get_opposite_direction(Direction direction) {
  switch (direction) {
    case DIRECTION_NORTH: return DIRECTION_SOUTH;
    case DIRECTION_EAST: return DIRECTION_WEST;
    case DIRECTION_SOUTH: return DIRECTION_NORTH;
    case DIRECTION_WEST: return DIRECTION_EAST;
    default: return -1;
  }
}

// checa se duas posições colidem
bool positions_collide(Position position1, Position position2) {
  int row1 = position1[0], col1 = position1[1];
  int row2 = position2[0], col2 = position2[1];

  return row1 == row2 && col1 == col2;
}

// circularidade de um inteiro dentro de um intervalo, útil para fazer um valor
// "teleportar" para o outro lado se ele saiu do limite. Especialmente útil
// para fazer a cobrinha teleportar para outro lado do canvas.
int wrap(int n, int min, int max) {
  if (n < min) {
    n = max;
  } else if (n > max) {
    n = min;
  }

  return n;
}

// pega um valor inteiro pseudo aleatório (um algoritmo verdadeiramente
// aleatório não vale a pena devido à complexidade) no intervalo inclusivo
// [min, max], útil para escolhas aleatórias.
int randint(int min, int max) {
  if (min > max) {
    fprintf(stderr, "randint called with min (%i) greater than max (%i).\n", min, max);
    exit(EXIT_FAILURE);
  }

  static bool initilized = false;

  if (!initilized) {
    srand(time(NULL));
  }

  return min + rand() % (max - min + 1);
}

void display_show_lines(uint8_t *ssd, uint8_t ssd_size, char* lines[], uint8_t lines_size, RenderArea frame_area) {
    ssd1306_clear(ssd, ssd_size, frame_area);

    for (uint i = 0; i < lines_size; i++) {
        uint font_size = 8;
        uint x = (ssd1306_width - strlen(lines[i]) * font_size) / 2 - 1;
        uint y = (ssd1306_height - lines_size * font_size) / 2 + i * font_size;

        ssd1306_draw_string(ssd, x, y, lines[i]);
    }

    render_on_display(ssd, &frame_area);
}

void display_show_line(uint8_t *ssd, uint8_t ssd_size, char* line, RenderArea frame_area) {
    display_show_lines(ssd, ssd_size, (char* [1]){ line }, 1, frame_area);
}

void display_menu_text(MenuText menu_text, uint8_t *ssd, RenderArea frame_area) {
    MenuTextView* mtv_start = menu_text_view_create(menu_text);
    display_show_lines(ssd, count_of(ssd), mtv_start->lines, mtv_start->lines_size, frame_area);
    menu_text_view_free(mtv_start);
}

bool is_button_down(uint8_t button) {
    return gpio_get(button) == 0;
}

int wait_button_a_or_b() {
    while (true) {
        bool button_a_down = is_button_down(BUTTON_A);
        bool button_b_down = is_button_down(BUTTON_B);

        if (button_a_down) {
            return BUTTON_A;
        } else if (button_b_down) {
            return BUTTON_B;
        }

        sleep_ms(50);
    }

    return -1;
}

void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
}
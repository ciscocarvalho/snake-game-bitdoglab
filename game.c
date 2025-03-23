#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "./inc/constants.h"
#include "./inc/canvas.h"
#include "./inc/snake.h"
#include "./inc/food.h"
#include "./inc/utils.h"
#include "./inc/joystick.h"
#include "./inc/melody.h"
#include "./inc/neopixel.h"
#include "./inc/display_oled/ssd1306.h"

int game_loop() {
    char *controls_text_in_game[] = {
        "Controls",
        "",
        "Joystick move",
        "A Quit",
        "B Restart",
    };

    char *controls_text_on_win[] = {
        "You win",
        "",
        "A Quit",
        "B Play again",
    };

    char *controls_text_on_loss[] = {
        "You lose",
        "",
        "A Quit",
        "B Try again",
    };

    RenderArea text_area = ssd1306_init();
    uint8_t ssd[ssd1306_buffer_length];
    ssd1306_clear(ssd, ssd1306_buffer_length, text_area);

    Canvas* canvas = canvas_init(5, 5);

    Position snake_position;
    // BUG: canvas_get_random_free_position doesn't work at the beginning
    // because raspberry pi's time always starts at 0.
    // canvas_get_random_free_position(canvas, snake_position);
    copy_position((int [2]){ 2, 1 }, snake_position);
    Snake* snake = snake_init(canvas, snake_position, DIRECTION_EAST, 2);
    Food* food = food_init(canvas);

    canvas_render(canvas);

    bool going = true;
    bool allow_speeding = false;
    static displaying_text_in_game = false;
    int next_action;

    while (going) {
        if (!displaying_text_in_game) {
            display_show_lines(ssd, count_of(ssd), controls_text_in_game, count_of(controls_text_in_game), text_area);
            displaying_text_in_game = true;
        }

        int total_delay = 500;
        int step_delay = 10;
        int steps = total_delay / step_delay;

        for (int i = 0; i < steps; i++) {
            Direction current_direction = snake->direction;
            Direction previous_direction = current_direction;
            Direction new_direction = current_direction;
            bool skip_delay = false;

            JoystickInfo joystick_info = joystick_get_info();
            Direction joystick_direction = joystick_info.direction;

            if (joystick_direction != DIRECTION_NONE) {
                if (allow_speeding && current_direction == joystick_direction) {
                    skip_delay = true;
                } else if (current_direction != get_opposite_direction(joystick_direction)) {
                    new_direction = joystick_direction;
                }
            }

            bool button_a_down = is_button_down(BUTTON_A);
            bool button_b_down = is_button_down(BUTTON_B);

            if (button_a_down || button_b_down) {
                going = false;
                skip_delay = true;
                next_action = button_a_down ? ACTION_QUIT : ACTION_RESTART;

                while (is_button_down(button_a_down ? BUTTON_A : BUTTON_B)) {
                    sleep_ms(10);
                }
            }

            if (previous_direction != new_direction) {
                snake->direction = new_direction;
                skip_delay = true;
            }

            if (skip_delay) {
                break;
            }

            sleep_ms(step_delay);
        }

        if (!going) {
            break;
        }

        Position next_head_position;
        get_next_node_position(snake, canvas, 0, next_head_position);

        if (positions_collide(next_head_position, food->position)) {
            food_remove(food, canvas);
            play_bite(BUZZER_PIN);
            snake_grow(snake, canvas);
            snake_move(snake, canvas);

            if (canvas_count_free_positions(canvas) > 0) {
                food_move(food, canvas);
            }
        } else {
            snake_move(snake, canvas);
        }

        canvas_render(canvas);

        bool game_over = snake_self_collides(snake);
        bool game_won = canvas_count_free_positions(canvas) == 0 && !food->in_canvas;

        if (game_over || game_won) {
            if (game_over) {
                play_game_over(BUZZER_PIN);
                display_show_lines(ssd, count_of(ssd), controls_text_on_loss, count_of(controls_text_on_loss), text_area);
                displaying_text_in_game = false;
            } else {
                play_game_won(BUZZER_PIN);
                display_show_lines(ssd, count_of(ssd), controls_text_on_win, count_of(controls_text_on_win), text_area);
                displaying_text_in_game = false;
            }

            uint8_t button_down = wait_button_a_or_b();
            next_action = button_down == BUTTON_A ? ACTION_QUIT : ACTION_RESTART;

            while (is_button_down(button_down)) {
                sleep_ms(50);
            }

            going = false;
        }

        canvas_render(canvas);
    }

    food_free(food);
    snake_free(snake);
    canvas_clear(canvas);
    canvas_render(canvas);
    canvas_free(canvas);

    return next_action;
}

int main() {
    stdio_init_all();

    // inicia joystick
    joystick_init();

    // inicia neopixel (leds)
    npInit(LED_PIN);
    npClear();

    // inicia ssd1306 (display oled)
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // inicia botão a
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // inicia botão b
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // inicia buzzer
    pwm_init_buzzer(BUZZER_PIN);

    // texto a ser mostrado no display oled no início do programa
    char *controls_text_on_start[] = {
        "Snake",
        "",
        "A Quit",
        "B Play",
    };

    // limpa a matriz de leds
    npClear();
    npWrite();

    // limpa o display oled
    RenderArea text_area = ssd1306_init();
    uint8_t ssd[ssd1306_buffer_length];
    ssd1306_clear(ssd, ssd1306_buffer_length, text_area);
    display_show_lines(ssd, count_of(ssd), controls_text_on_start, count_of(controls_text_on_start), text_area);

    // espera o usuário fazer uma escolha pressionando algum dos botões
    int button_down = wait_button_a_or_b();

    if (button_down == BUTTON_A) {
        // usuário apertou A, então o programa limpa a mensagem do display oled e encerra
        ssd1306_clear(ssd, ssd1306_buffer_length, text_area);
        return 0;
    } else {
        // usuário apertou B, então o programa espera um pouco e inicia o jogo
        sleep_ms(200);
    }

    bool keep_playing = true;

    while (keep_playing) {
        int next_action = game_loop();

        switch (next_action) {
            case ACTION_QUIT: keep_playing = false; break;
            case ACTION_RESTART: break;
        }
    }

    // jogo encerrado, limpa display oled
    ssd1306_clear(ssd, ssd1306_buffer_length, text_area);

    // limpa matriz de leds (3 vezes para evitar bugs estranhos que ocorreram nos testes)
    for (int i = 0; i < 3; i++) {
        npClear();
        npWrite();
        sleep_ms(50);
    }

    return 0;
}

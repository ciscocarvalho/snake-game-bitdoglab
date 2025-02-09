#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "./headers/constants.h"
#include "./headers/canvas.h"
#include "./headers/snake.h"
#include "./headers/apple.h"
#include "./headers/utils.h"
#include "./headers/joystick.h"
#include "./headers/neopixel.h"
#include "./headers/display_oled/ssd1306.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

#define BUTTON_A 5
#define BUTTON_B 6
#define BUZZER_PIN 21

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

#define ACTION_RESTART 0
#define ACTION_QUIT 1

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

void play_tone(uint pin, uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    sleep_ms(duration_ms);

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
}

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
    Apple* apple = apple_init(canvas);

    canvas_render(canvas);

    bool going = true;
    bool allow_speeding = false;
    int next_action;

    while (going) {
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

        if (positions_collide(next_head_position, apple->position)) {
            snake_grow(snake, canvas);
            play_tone(BUZZER_PIN, 392, 50);
            apple_move(apple, canvas);
        }

        snake_move(snake, canvas);
        canvas_render(canvas);

        bool game_over = snake_self_collides(snake);
        bool game_won = canvas_count_free_positions(canvas) == 0;

        if (game_over || game_won) {
            if (game_over) {
                display_show_lines(ssd, count_of(ssd), controls_text_on_loss, count_of(controls_text_on_loss), text_area);
            } else {
                display_show_lines(ssd, count_of(ssd), controls_text_on_win, count_of(controls_text_on_win), text_area);
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

    apple_free(apple);
    snake_free(snake);
    canvas_clear(canvas);
    canvas_render(canvas);
    canvas_free(canvas);

    return next_action;
}

int main() {
    stdio_init_all();

    // joystick
    joystick_init();
    
    // neopixel (leds)
    npInit(LED_PIN);
    npClear();

    // ssd1306 (display oled)
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // (button a)
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // (button b)
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // (buzzer)
    pwm_init_buzzer(BUZZER_PIN);

    char *controls_text_on_start[] = {
        "Snake",
        "",
        "A Quit",
        "B Play",
    };

    npClear();
    npWrite();
    RenderArea text_area = ssd1306_init();
    uint8_t ssd[ssd1306_buffer_length];
    ssd1306_clear(ssd, ssd1306_buffer_length, text_area);
    display_show_lines(ssd, count_of(ssd), controls_text_on_start, count_of(controls_text_on_start), text_area);

    int button_down = wait_button_a_or_b();

    if (button_down == BUTTON_A) {
        ssd1306_clear(ssd, ssd1306_buffer_length, text_area);
        return 0;
    } else {
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

    ssd1306_clear(ssd, ssd1306_buffer_length, text_area);

    // Weird led bugs made me paranoid
    for (int i = 0; i < 3; i++) {
        npClear();
        npWrite();
        sleep_ms(50);
    }

    return 0;
}

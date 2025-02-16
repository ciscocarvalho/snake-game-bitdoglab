#include "../inc/melody.h"
#include "pico/types.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

static void play_tone(uint pin, uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    sleep_ms(duration_ms);

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
}

void play_melody(uint pin, Melody melody, uint melody_length) {
    for (uint i = 0; i < melody_length; i++) {
        uint note = melody[i][0];
        uint duration = melody[i][1];
        play_tone(pin, note, duration);
    }
}

void play_game_won(uint pin) {
    uint melody[][2] = {
        { NOTE_C5, 300 },
        { NOTE_D5, 300 },
        { NOTE_G5, 300 },
        { NOTE_C6, 600 },
        { NOTE_D5, 300 },
        { NOTE_C5, 600 },
    };
    play_melody(pin, melody, count_of(melody));
}

void play_game_over(uint pin) {
    uint melody[][2] = {
        { NOTE_A4, 300 },
        { NOTE_G4, 300 },
        { NOTE_F4, 400 },
        { NOTE_E4, 600 },
    };
    play_melody(pin, melody, count_of(melody));
}

void play_bite(uint pin) {
    Melody melody = { { 392, 50 } };
    play_melody(pin, melody, count_of(melody));
}
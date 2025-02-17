#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "math.h"
#include "../inc/joystick.h"
#include "../inc/constants.h"

// ===========================================================================
// JOYSTICK
// Facilita o uso do joystick no código principal ao encapsular a lógica da
// captura de input e os cálculos necessários para determinar informações como
// a direção em que ele está inclinado
// ===========================================================================

// inicia o joystick
void joystick_init() {
    adc_init();

    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    adc_gpio_init(27);
}

// pega informações sobre o joystick.
// y_raw, x_raw, max, x_normalized e y_normalized foram copiados do exemplo em
// https://github.com/BitDogLab/BitDogLab-C/blob/main/joystick/joystick.c.
// as demais informações foram descobertas usando matemática básica.
// "normalized" aqui significa um valor entre 0 e 1, para representar
// porcentagem e deixar parte do código mais intuitivo.
// uma direção é assumida somente se o joystick está inclinado mais que 50% do
// raio da volta total que ele é capaz de fazer, caso contrário a direção é
// DIRECTION_NONE.
JoystickInfo joystick_get_info() {
    JoystickInfo info = {};

    adc_select_input(0);
    info.y_raw = adc_read();

    adc_select_input(1);
    info.x_raw = adc_read();

    info.max = (1 << 12) - 1;

    info.x_normalized = info.x_raw / (float)info.max;
    info.y_normalized = info.y_raw / (float)info.max;

    info.center = info.max / 2;

    info.x_raw_distance = abs(info.x_raw - info.center);
    info.y_raw_distance = abs(info.y_raw - info.center);

    info.raw_distance = sqrt(pow(info.x_raw_distance, 2) + pow(info.y_raw_distance, 2));
    info.normalized_distance = info.raw_distance / (float)info.center;

    info.x_normalized_distance = info.x_raw_distance / (float)info.max;
    info.y_normalized_distance = info.y_raw_distance / (float)info.max;

    info.direction = DIRECTION_NONE;

    if (info.normalized_distance > 0.5) {
        if (info.x_raw_distance > info.y_raw_distance) {
            info.direction = info.x_raw < info.center ? DIRECTION_WEST : DIRECTION_EAST;
        } else {
            // Regular cartesian plane y logic applies here
            info.direction = info.y_raw > info.center ? DIRECTION_NORTH : DIRECTION_SOUTH;
        }
    }


    return info;
}
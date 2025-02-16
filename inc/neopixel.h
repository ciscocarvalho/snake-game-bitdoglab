#pragma once

#include "pico/types.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

typedef int LedColor[3];

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};

typedef struct pixel_t pixel_t;

typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

void npInit(uint pin);

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);

void npClear();

void npWrite();

int getIndex(int x, int y);

void setSpriteLEDs(int sprite[5][5][3]);

void copy_color(LedColor source, LedColor target);
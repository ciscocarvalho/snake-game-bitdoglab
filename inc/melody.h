#include "pico/types.h"

typedef uint Melody[][2];

#define NOTE_C4 29886
#define NOTE_Cs4 28294
#define NOTE_C5 14929
#define NOTE_C6 7465
#define NOTE_D4 26795
#define NOTE_D5 13298
#define NOTE_DS4 25386
#define NOTE_E4 24063
#define NOTE_F4 22823
#define NOTE_FS5 9561
#define NOTE_Fs4 21663
#define NOTE_G4 20579
#define NOTE_G5 8966
#define NOTE_GS5 9409
#define NOTE_GS4 19569
#define NOTE_A4 18630
#define NOTE_AS4 177758
#define NOTE_AS5 8380
#define NOTE_AS3 33498
#define NOTE_B4 16952

void play_melody(uint pin, Melody melody, uint melody_length);
void play_game_won(uint pin);
void play_game_over(uint pin);
void play_bite(uint pin);
void play_selection_move(uint pin);
#pragma once

#include <stdbool.h>

typedef struct {
    bool mute;
} GameSettingsSoundEffects;

typedef struct {
    bool mute;
} GameSettingsSoundMusic;

typedef struct {
    GameSettingsSoundEffects sound_effects;
    GameSettingsSoundMusic music;
} GameSettingsSound;

typedef struct {
    GameSettingsSound sound;
} GameSettings;

extern GameSettings settings;

GameSettings game_settings_get_default();
GameSettings* game_settings_get();
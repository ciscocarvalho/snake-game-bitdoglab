#include "../inc/settings.h"

static bool initialized = false;

static GameSettings default_settings = {
    .sound = (GameSettingsSound) {
        (GameSettingsSoundEffects) {
            .mute = false,
        },
        (GameSettingsSoundMusic) {
            .mute = false,
        },
    },
};

GameSettings settings;

static void initialize_settings() {
    settings = default_settings;
    initialized = true;
}

GameSettings game_settings_get_default() {
    return default_settings;
}

GameSettings* game_settings_get() {
    if (!initialized) {
        initialize_settings();
    }

    return &settings;
}

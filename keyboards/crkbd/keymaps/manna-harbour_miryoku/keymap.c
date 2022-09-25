// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.


#include QMK_KEYBOARD_H
#include <stdio.h>

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Default"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case L_RAISE:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
    }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

static void oled_render_logo(void) {
    static const unsigned char PROGMEM raw_logo[] = {
        0,  0,  0,  0,  0,  0,  0,  0,208, 32, 32, 32, 32, 32, 32, 32, 32, 32, 64,192,  0,  0,  0,  0,  0,  0,  0,224, 96,192,  0,  0,  0,  0,  0,  0,  4,  4,  4,  4,  4,244,  4,  4,  4, 12,  8,  8,  0,  0,  0,128,192, 64, 64, 64, 96, 32, 32, 32, 32, 32,  0,  0,  0,  0,  0,  0,224, 16, 16, 16, 16, 16, 16, 16, 16, 16,  0,  0,  0,  0,  0,  0, 24, 48, 96,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,240,  0,  0,  0,  0,  0,  0,  0,240, 24, 12,  4,  4, 28,240, 56, 12,  4,196,124,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,240, 31,  0,  0,  0,  0,  0,  0,  0,  0,128,192, 97, 63,  0,  0,  0,192,120,142,129,128,129,135,156,176, 96,128,  0,  0,  0,  0,  0,  0,255,  0,  0,  0,  0,  0,  0,  0,  0,  7, 13, 24, 16, 16, 16, 16, 48, 32, 32, 32, 96,192,  0,  0,  0,  0,  0,255,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  6, 12, 24,144,192, 96, 48, 12,  7,  1,  0,  0,  0,  0,  0,  0,  0,  0,  7, 24,112,192,  0,  0,192, 48, 28,  6,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  0, 31, 32, 48, 16, 24,  8, 12,  4,  6,  3,  1,  0,  0,  0,  0,  0, 32, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7, 12,  0, 16, 16, 16, 16, 31, 16, 16, 16, 16,  0,  0,  0,  0,  8, 24, 48, 32, 32, 32, 32, 32, 32, 32, 16, 24, 15,  0,  0,  0,  0,  0, 23,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 32, 48, 28,  6,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_write_P(raw_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
  }
  return true;
}
#endif // OLED_ENABLE

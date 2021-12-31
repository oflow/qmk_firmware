/* Copyright 2021 bdtc123
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H


#ifndef PERMISSIVE_HOLD
#define PERMISSIVE_HOLD
#endif
#ifndef TAPPING_TERM
#define TAPPING_TERM 150
#endif

#ifndef IME_TAPPING_TERM
#define IME_TAPPING_TERM 150
#endif

#define ___    KC_TRNS
#define ____   KC_TRNS
#define _____  KC_TRNS
#define ______ KC_TRNS

#define _BASE 0
#define _G    1  // for FPS Game (need LShift)
#define _L    2  // Left Mod
#define _R    3  // Right Mod

#define RGB_206 0x25,0x00,0x60  // purple
/* Ctrl+Alt, Ctrl+Shift, GUI+Alt, GUI+Shift */

enum custom_keycodes {
    KC_ESGR = SAFE_RANGE,  // Esc, Shift: ~, Ctrl: ~,  KC_ESGR != KC_GESC
    KC_BSDL,   // BS, Shift: Del
    IME_ON,    // Tap: Ctrl-Shift-Ins, Hold: LCtrl (when not using KC_HENK, KC_MHEN)
    IME_OFF,   // Tap: Ctrl-Ins,       Hold: LCtrl
    IME_TOG,   // Tap: Ctrl-Space      Hold: LCtrl
    RM_SOLID,  // RGB_MATRIX_SOLID_COLOR           from feature_rgb_matrix.md
    RM_WIDE,   // RGB_MATRIX_SOLID_REACTIVE_WIDE
    RM_MWIDE,  // RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    RM_SIMPL,  // RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    RM_MCROS,  // RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    RM_MNEXS,  // RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    RM_RAIND,  // RGB_MATRIX_RAINDROPS
    RM_JRAIN   // RGB_MATRIX_JELLYBEAN_RAINDROPS
};

static bool mods_bsdl[2] = {0, 0};       // lsft, rsft
static bool mods_esgr[4] = {0, 0, 0, 0}; // lsft, rsft, lctl, rctl

static bool ime_pressed = 0;
static uint16_t ime_timer = 0;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * 0: Default
     *
     * ,--------------------------------------------------------------------------------------.
     * | ESC |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |   BS   |
     * |--------------------------------------------------------------------------------------|
     * | TAB  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |   \   |
     * |--------------------------------------------------------------------------------------|
     * | MO(L) |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter   |
     * |--------------------------------------------------------------------------------------|
     * | IME OFF |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |     IME_ON     |
     * |--------------------------------------------------------------------------------------|
     * | LCTL | LGUI | LALT |           Shift & Space           | Space | RALT | RGUI | MO(R) |
     * `--------------------------------------------------------------------------------------'
     */
    [0] = LAYOUT_60_ansi(
        KC_ESGR, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSDL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        MO(_L),  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
        IME_OFF,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          IME_ON,
        KC_LCTL, KC_LGUI, KC_LALT,                        LSFT_T(KC_SPC),                         KC_SPC,  KC_RALT, KC_RGUI, MO(_R)
    ),
    /* 1: Game Layer
     *
     * Alt-F10:  Discord Mute Toggle
     * GUI-PSCR: Screenshot
     */
    [_G] = LAYOUT_60_ansi(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,   KC_BSDL,
        KC_TAB,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
        LT(_L,KC_M), ____, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,           _______,
        KC_LSFT,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,           IME_TOG,
        KC_LCTL, KC_K,    KC_L,                               KC_SPC,                             G(KC_PSCR), ____, A(KC_F10),TO(_BASE)
    ),
    /* 2: Left Funtion
     *
     * Shift-I: Shift-Insert
     * HJKL:    Vim Like
     * GUI-Tab: not Alt-Tab
     */
    [_L] = LAYOUT_60_ansi(
        RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        G(KC_TAB), ___,G(A(KC_PSCR)),G(S(KC_S)),___, _______, _______, _______, S(KC_INS), ___,   KC_PSCR, KC_HOME, KC_END,  _______,
        _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,          _______,
        _______,          _______, _______, _______, _______, _______, _______, _______, KC_PGUP, KC_PGDN, _______,          IME_OFF,
        _______, _______, _______,                            S(KC_SPC),                          _______, _______, _______, _______
    ),
    /* 3: Right Function
     *
     * GUI-K = Bluetooth Menu
     */
    [_R] = LAYOUT_60_ansi(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  C(A(KC_DEL)),
        _______, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______,          RM_SOLID,RM_WIDE, RM_MWIDE,RM_SIMPL,RM_MCROS,RM_MNEXS,RM_RAIND,RM_JRAIN,_______, _______,          _______,
        _______, _______, _______,                            _______,                            _______, G(KC_K), TO(_G),  _______
    ),

};
// rgb_matrix_set_color(index, color)
// rgb_matrix_set_color_all(color)
// rgb_matrix_mode(mode)
// rgb_matrix_mode_noeeprom(mode)

static uint8_t rgbm_game[16] = {0, 13, 14, 27, 28, 40, 41, 52, 53, 54, 55, 56, 57, 58, 59, 60};
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    switch(get_highest_layer(layer_state|default_layer_state)) {
        case _L:
            rgb_matrix_set_color(28, RGB_RED);
            break;
        case _R:
            rgb_matrix_set_color(60, RGB_RED);
            break;

        case _G:
            /* 56: space
             * 60: rctl
             * 40: enter */
            for (uint8_t i = 0; i < 16; i++) {
                rgb_matrix_set_color(rgbm_game[i], RGB_206);
            }
            break;
    }
}
/*
uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
        case 0:
            rgb_matrix_set_color_all(RGB_GREEN);
            break;
        case 1:
            rgb_matrix_set_color_all(RGB_PURPLE);
            break;
    }
    return state;
}*/
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool lsft_pressed = get_mods() & MOD_BIT(KC_LSFT);
    bool rsft_pressed = get_mods() & MOD_BIT(KC_RSFT);
    bool lctl_pressed = get_mods() & MOD_BIT(KC_LCTL);
    bool rctl_pressed = get_mods() & MOD_BIT(KC_RCTL);

    switch (keycode) {
        case RESET:
            if (record->event.pressed) {
            }
            return true;
        case KC_BSDL:
            // Shift-BS = DEL
            if (record->event.pressed) {
                if (lsft_pressed || rsft_pressed) {
                    mods_bsdl[0] = lsft_pressed;
                    mods_bsdl[1] = rsft_pressed;
                    unregister_mods(lsft_pressed ? MOD_LSFT : MOD_RSFT);
                }
                register_code((mods_bsdl[0] || mods_bsdl[1]) ? KC_DEL : KC_BSPC);
            } else {
                if (mods_bsdl[0]) register_mods(MOD_LSFT);
                if (mods_bsdl[1]) register_mods(MOD_RSFT);
                unregister_code((mods_bsdl[0] || mods_bsdl[1]) ? KC_DEL : KC_BSPC);
                mods_bsdl[0] = false;
                mods_bsdl[1] = false;
            }
            return false;
        case KC_ESGR:
            if (record->event.pressed) {
                // S-Esc = ~ TILD = S-GRV
                // C-Esc = ` GRV
                if (lctl_pressed || rctl_pressed) {
                    mods_esgr[2] = lctl_pressed;
                    mods_esgr[3] = rctl_pressed;
                    unregister_mods(lctl_pressed ? MOD_LCTL : MOD_RCTL);
                    register_code(KC_GRV);
                } else if (rsft_pressed || lsft_pressed) {
                    mods_esgr[0] = lsft_pressed;
                    mods_esgr[1] = rsft_pressed;
                    register_code(KC_GRV);
                } else {
                    register_code(KC_ESC);
                }
            } else {
                if (mods_esgr[2] || mods_esgr[3]) {
                    unregister_code(KC_GRV);
                    if (mods_esgr[2]) register_mods(MOD_LCTL);
                    if (mods_esgr[3]) register_mods(MOD_RCTL);
                } else if (mods_esgr[0] || mods_esgr[1]) {
                    unregister_code(KC_GRV);
                } else {
                    unregister_code(KC_ESC);
                }
                mods_esgr[0] = false;
                mods_esgr[1] = false;
                mods_esgr[2] = false;
                mods_esgr[3] = false;
            }
            return false;
        case IME_ON:
            // IME ON = C-S-Ins
            if (record->event.pressed) {
                ime_pressed = true;
                ime_timer = timer_read ();
                register_mods(MOD_LCTL);

            } else {
                if (ime_pressed && timer_elapsed(ime_timer) < IME_TAPPING_TERM) {
                    if (lsft_pressed || rsft_pressed) {
                        // IME OFF
                        unregister_mods(lsft_pressed ? MOD_LSFT : MOD_RSFT);
                        register_code(KC_INS);
                        unregister_code(KC_INS);
                        register_mods(lsft_pressed ? MOD_LSFT : MOD_RSFT);
                    } else {
                        // IME ON
                        register_mods(MOD_LSFT);
                        register_code(KC_INS);
                        unregister_code(KC_INS);
                        unregister_mods(MOD_LSFT);
                    }
                }
                unregister_mods(MOD_LCTL);
            }
            return false;
        case IME_OFF:
            // IME OFF = C-INs
            if (record->event.pressed) {
                ime_pressed = true;
                ime_timer = timer_read ();
                register_mods(MOD_LCTL);
            } else {
                if (ime_pressed && timer_elapsed(ime_timer) < IME_TAPPING_TERM) {
                    register_code(KC_INS);
                    unregister_code(KC_INS);
                }
                unregister_mods(MOD_LCTL);
            }
            return false;
        case IME_TOG:
            // IME Toggle = C-Space
            if (record->event.pressed) {
                ime_pressed = true;
                ime_timer = timer_read ();
                register_mods(MOD_LCTL);
            } else {
                if (ime_pressed && timer_elapsed(ime_timer) < IME_TAPPING_TERM) {
                    register_code(KC_SPC);
                    unregister_code(KC_SPC);
                }
                unregister_mods(MOD_LCTL);
            }
            return false;
        case RM_SOLID:
            if (record->event.pressed) rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
            return false;
        case RM_WIDE:
            if (record->event.pressed) rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_WIDE);
            return false;
        case RM_MWIDE:
            if (record->event.pressed) rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE);
            return false;
        case RM_SIMPL:
            if (record->event.pressed) rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_SIMPLE);
            return false;
        case RM_MCROS:
            if (record->event.pressed) rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_MULTICROSS);
            return false;
        case RM_MNEXS:
            if (record->event.pressed) rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS);
            return false;
        case RM_RAIND:
            if (record->event.pressed) rgb_matrix_mode(RGB_MATRIX_RAINDROPS);
            return false;
        case RM_JRAIN:
            if (record->event.pressed) rgb_matrix_mode(RGB_MATRIX_JELLYBEAN_RAINDROPS);
            return false;
        default:
            ime_pressed = false;

            return true;
    }
}

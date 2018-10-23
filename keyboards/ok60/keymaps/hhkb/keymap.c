#include "ok60.h"

#define XXX  KC_NO
#define ___  KC_TRNS

#define _BASE  0
#define _GAME  1  // for FPS Game (need LShift)
#define _L     2
#define _R     3

// short modifier: S(kc) = LSFT(kc) = (kc | QK_LSFT)
#define C(kc)      (kc | QK_LCTL)
#define A(kc)      (kc | QK_LALT)
#define G(kc)      (kc | QK_LGUI)
#define C_A(kc)    (kc | QK_LCTL | QK_LALT)
#define S_C(kc)    (kc | QK_LSFT | QK_LCTL)
#define S_A(kc)    (kc | QK_LSFT | QK_LALT)
#define S_G(kc)    (kc | QK_LSFT | QK_LGUI)

#define RGB_PLN RGB_MODE_PLAIN
#define RGB_BRT RGB_MODE_BREATHE

#ifndef IME_TAPPING_TERM
#define IME_TAPPING_TERM 250
#endif

enum custom_keycodes {
    KC_ESGR = SAFE_RANGE, // Esc, Shift: ~, Ctrl: `
    KC_BSDL,  // BS,  Shift: Del
    KC_MF12,  // F12, Shift: F11,     Ctrl: F13
    IME_ON,   // Tap: Ctrl+Shift+Ins, Hold: RCtrl
    IME_OFF,  // Tap: Ctrl+Ins,       Hold: LCtrl
    IME_TG    // Tap: Ctrl_Space      Hold: LCtrl
};

static bool mods_bsdl[2] = {0, 0};       // lsft, rsft
static bool mods_esgr[4] = {0, 0, 0, 0}; // lsft, rsft, lctl, rctl

static bool ctrl_interrupted[3] = {0, 0, 0};
static uint16_t scs_timer[3] = {0, 0, 0};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ,----------------------------------------------------------------------------------------.
     * | ESC |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  | F12 | BS |
     * |----------------------------------------------------------------------------------------|
     * | TAB  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \    |
     * |----------------------------------------------------------------------------------------|
     * | MO(L) |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |     Enter    |
     * |----------------------------------------------------------------------------------------|
     * | IME OFF |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |  IME ON  | MO(R) |
     * `----------------------------------------------------------------------------------------'
     *         | LGUI |  LAlt  |            Shift & Space              |  RAlt  | GUI |
     *         `----------------------------------------------------------------------'
     *
     */
    [_BASE] = LAYOUT_60_hhkb(
        KC_ESGR, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_MF12, KC_BSDL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        MO(_L),  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, XXX,     KC_ENT,
        IME_OFF, XXX,     KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, XXX,     IME_ON,  MO(_R),
        XXX,     KC_LGUI, KC_LALT,                   LSFT_T(KC_SPC),                     XXX,     KC_RALT, KC_RGUI, XXX),

    /*
     * ,----------------------------------------------------------------------------------------.
     * | ESC |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  | F12 | BS |
     * |----------------------------------------------------------------------------------------|
     * | TAB  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \    |
     * |----------------------------------------------------------------------------------------|
     * | Map   |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |     Enter    |
     * |----------------------------------------------------------------------------------------|
     * | LShift  |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |  IME TG  | TG(G) |
     * `----------------------------------------------------------------------------------------'
     *         | LCtl |  LAlt  |                Space                 |  RAlt  | RCtl |
     *         `----------------------------------------------------------------------'
     */
    [_GAME] = LAYOUT_60_hhkb( // TO(1) : for FPS Game
        KC_ESGR, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_MF12,  KC_BSDL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        LT(_L,KC_M),KC_A, KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, XXX,     KC_ENT,
        KC_LSFT, XXX,     KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, XXX,     KC_RSFT,  TO(_BASE),

        XXX,      KC_LCTL, KC_LALT,                       KC_SPC,                        XXX,     IME_TG,  KC_RCTL, XXX),

    /*
     * ,----------------------------------------------------------------------------------------.
     * | RSET|  F1 |  F2 |  F3 |  F4 |  F5 |  F6 |  F7 |  F8 |  F9 | F10 | F11 | F12 | Ins | Del|
     * |----------------------------------------------------------------------------------------|
     * | G-Tab|S-C-q| G-w |     |     |     |     |     |S-Ins|     | PrScr| Home | End | Pipe  |
     * |----------------------------------------------------------------------------------------|
     * |       |S-A-a|S-G-s|     |     |     |  ← |  ↓ |  ↑ |  → |     |S-C-'|     Enter    |
     * |----------------------------------------------------------------------------------------|
     * | LCtrl   |     |     |     |     |     | C-n |     | PgDn| PgUp|     |  LCtrl   |       |
     * `----------------------------------------------------------------------------------------'
     *         |     |       |              Shift & Space             | S-C-t | S-C-r |
     *         `----------------------------------------------------------------------'
     */
    [_L] = LAYOUT_60_hhkb( // MO(2) : left side modifer
        RESET,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,   KC_F11,   KC_F12,  KC_INS, KC_DEL,
        G(KC_TAB),S_C(KC_Q),G(KC_W),___,     ___,     ___,     ___,    ___,     S(KC_INS),___,      KC_PSCR,  KC_HOME,  KC_END,  KC_PIPE,
        ___,      S_A(KC_A),S_G(KC_S),___,   ___,     ___,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,  ___,      S_C(KC_QUOT),XXX,  ___,
        KC_LCTL,  XXX,     ___,     ___,     ___,     ___,     ___,     C(KC_N), ___,     KC_PGDN,  KC_PGUP,  ___,      XXX,     KC_LCTL, ___,
        XXX,      ___,     ___,                           ___,                            XXX,      S_C(KC_T),S_C(KC_R),XXX),

    /*  R2, R3 : use Nerd-Fonts
     * ,----------------------------------------------------------------------------------------.
     * | `   |  F1 |  F2 |  F3 |  F4 |  F5 |  F6 |  F7 |  F8 |  F9 | F10 | F11 | F12 |   |CA-Del|
     * |----------------------------------------------------------------------------------------|
     * | RGB> | TOG | MOD | HUI | HUD | SAI | SAD | VAI | VAD |     |     | PLN | BRT |         |
     * |----------------------------------------------------------------------------------------|
     * |     |   |   |   |     |     |     |     |     |     |     |     |              |
     * |----------------------------------------------------------------------------------------|
     * |         | - | + |     |     |     |     |     |     |     |     | TO(GAME)  |      |
     * `----------------------------------------------------------------------------------------'
     *         |     |        |                S-Space                 |        |     |
     *         `----------------------------------------------------------------------'
     */

    [_R] = LAYOUT_60_hhkb( // MO(3) : right side modifer
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  ___,     C_A(KC_DEL),
        ___,     RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, ___,     ___,     RGB_PLN, RGB_BRT, ___,
        KC_MSTP, KC_MPRV, KC_MPLY, KC_MNXT, ___,     ___,     ___,     ___,     ___,     ___,     ___,     ___,     XXX,     ___,
        ___,     XXX,     KC_VOLD, KC_VOLU, ___,     ___,     ___,     ___,     ___,     ___,     ___,     ___,     XXX,     TO(_GAME), ___,
        XXX,     KC_APP,  ___,                       S(KC_SPC),                          XXX,     ___,     KC_APP,  XXX)

};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}
uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    case _BASE:
    //case _L:
    //case _R:
        rgblight_setrgb (0xFF,  0x00, 0x00);
        break;
    case _GAME:
        rgblight_setrgb (0x7A,  0x00, 0xFF);
        break;
    }
    return state;

}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool lsft_pressed = get_mods() & MOD_BIT(KC_LSFT);
    bool rsft_pressed = get_mods() & MOD_BIT(KC_RSFT);
    bool lctl_pressed = get_mods() & MOD_BIT(KC_LCTL);
    bool rctl_pressed = get_mods() & MOD_BIT(KC_RCTL);
    switch (keycode) {
        case RESET:
            if (record->event.pressed) {
                rgblight_setrgb(0x00, 0x00, 0xFF);
            }
            return true;
        case KC_BSDL:
            // Shift-BS = DEL
            if (record->event.pressed) {
                mods_bsdl[0] = lsft_pressed;
                mods_bsdl[1] = rsft_pressed;
                if (lsft_pressed) unregister_mods(MOD_LSFT);
                if (rsft_pressed) unregister_mods(MOD_RSFT);
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
                // S-Esc = TILD(S-GRV)
                // C-Esc = GRV
                mods_esgr[0] = lsft_pressed;
                mods_esgr[1] = rsft_pressed;
                mods_esgr[2] = lctl_pressed;
                mods_esgr[3] = rctl_pressed;
                if (lctl_pressed || rctl_pressed) {
                    unregister_mods(lctl_pressed ? MOD_LCTL : MOD_RCTL);
                    register_code(KC_GRV);
                } else if (rsft_pressed || lsft_pressed) {
                    register_code(KC_GRV);
                } else {
                    register_code(KC_ESC);
                }
            } else {
                if (mods_esgr[2] || mods_esgr[3]) {
                    unregister_code(KC_GRV);
                    register_mods(mods_esgr[2] ? MOD_LCTL : MOD_RCTL);
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
        case KC_MF12:
            // S-F12 = F11
            // C-F12 = F13
            if (record->event.pressed) {
                if (rctl_pressed || lctl_pressed) {
                    unregister_mods(rctl_pressed ? MOD_RCTL : MOD_LCTL);
                    register_code(KC_F13);
                    unregister_code(KC_F13);
                    register_mods(rctl_pressed ? MOD_RCTL : MOD_LCTL);
                } else if (lsft_pressed || rsft_pressed) {
                    unregister_mods(lsft_pressed ? MOD_LSFT : MOD_RSFT);
                    register_code(KC_F11);
                    unregister_code(KC_F11);
                    register_mods(lsft_pressed ? MOD_LSFT : MOD_RSFT);
                } else {
                    register_code(KC_F12);
                    unregister_code(KC_F12);
                }
            }
            return false;
        case IME_ON:
            // IME ON = C-S-Ins
            if (record->event.pressed) {
                ctrl_interrupted[0] = false;
                scs_timer[0] = timer_read ();
                register_mods(MOD_LCTL);
            } else {
                if (!ctrl_interrupted[0] && timer_elapsed(scs_timer[0]) < IME_TAPPING_TERM) {
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
                ctrl_interrupted[1] = false;
                scs_timer[1] = timer_read ();
                register_mods(MOD_LCTL);
            } else {
                if (!ctrl_interrupted[1] && timer_elapsed(scs_timer[1]) < IME_TAPPING_TERM) {
                    register_code(KC_INS);
                    unregister_code(KC_INS);
                }
                unregister_mods(MOD_LCTL);
            }
            return false;
        case IME_TG:
            // IME Toggle = C-Space
            if (record->event.pressed) {
                ctrl_interrupted[2] = false;
                scs_timer[2] = timer_read ();
                register_mods(MOD_LCTL);
            } else {
                if (!ctrl_interrupted[2] && timer_elapsed(scs_timer[2]) < IME_TAPPING_TERM) {
                    register_code(KC_SPC);
                    unregister_code(KC_SPC);
                }
                unregister_mods(MOD_LCTL);
            }
            return false;

        default:
            ctrl_interrupted[0] = true;
            ctrl_interrupted[1] = true;
            ctrl_interrupted[2] = true;

            return true;
    }
    return true;
}

void led_set_user(uint8_t usb_led) {
    if (usb_led & (1 << USB_LED_NUM_LOCK)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_CAPS_LOCK)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_SCROLL_LOCK)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_COMPOSE)) {
    } else {
    }

    if (usb_led & (1 << USB_LED_KANA)) {
    } else {
    }
}


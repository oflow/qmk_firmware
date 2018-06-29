#include "ok60.h"

#define XXX  KC_NO
#define ___  KC_TRNS

#define _BASE  0
#define _L     1
#define _R     2
#define _GAME  3  // for FPS Game (need LShift)

#define RGB_PLN RGB_MODE_PLAIN
#define RGB_BRT RGB_MODE_BREATHE

enum custom_keycodes {
    KC_ESCGR,   // Esc, ~, `
    KC_BSDEL,   // BS, Del
    IME_ON,  // Tap: Ctrl+Shift+Ins, Hold: RCtrl
    IME_OFF  // Tap: Ctrl+Ins,       Hold: LCtrl
};
static bool bsdel_shifted = false;
static bool escgr_shifted = false;
static bool escgr_ctrled = false;

static bool ctrl_interrupted[2] = {0, 0};
static uint16_t scs_timer[2] = {0, 0};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ,----------------------------------------------------------------------------------------.
     * | ESC |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |  `  | BS |
     * |----------------------------------------------------------------------------------------|
     * | TAB  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \    |
     * |----------------------------------------------------------------------------------------|
     * | MO(L) |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |     Enter    |
     * |----------------------------------------------------------------------------------------|
     * | IME OFF |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |  IME ON  | MO(R) |
     * `----------------------------------------------------------------------------------------'
     *         | Alt |  GUI  |               Shift & Space               |  GUI  | Alt |
     *         `-----------------------------------------------------------------------'
     *
     */
    [_BASE] = KEYMAP(
        KC_ESCGR, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,  KC_GRV,  KC_BSDEL,
        KC_TAB,   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC, KC_BSLS,
        MO(_L),   KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN,  KC_QUOT,  XXX,     KC_ENT,
        IME_OFF,  XXX,    KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,    KC_COMM, KC_DOT,   KC_SLSH,  XXX,     IME_ON,  MO(_R),
        XXX,      KC_LALT, KC_LGUI,                SFT_T(KC_SPC),         XXX,     KC_RGUI, KC_RALT,  XXX),

    [_L] = KEYMAP( // MO(2) : left side modifer
        RESET,    KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,  KC_INS,  KC_DEL,
        LGUI(KC_TAB),  ___,  ___,  ___,   ___,    ___,    ___,    ___,   S(KC_INS), ___,    KC_PSCR,  KC_HOME,  KC_END,  ___,
        ___,    LCTL(S(KC_A)), ___, ___,  ___,    ___,   KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT, ___,      ___,      ___,     ___,
        ___,      ___,    ___,    ___,    ___,    ___,    ___,    LCTL(KC_N), ___, KC_PGDN, KC_PGUP,  ___,      ___,     IME_OFF, ___,
        ___,     KC_LGUI, ___,                      S(KC_SPC),            ___,     ___,     KC_RGUI,  ___),

    [_R] = KEYMAP( // MO(2) : right side modifer
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9, KC_F10, KC_F11, KC_F12, ___,   LCTL(LALT(KC_DEL)),
        ___,     RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, ___,   ___,    ___,    ___,    ___,
        KC_CAPS, RGB_PLN, RGB_BRT, ___,     ___,     ___,     ___,     ___,     ___,     ___,   ___,    ___,    ___,    ___,
        ___,     ___,     ___,     ___,     ___,     ___,     ___,     ___,     ___,     ___,   ___,    ___,    ___,    TG(_GAME), ___,
        ___,     KC_APP,  ___,                       KC_SPC,                    ___,     ___,   KC_APP, ___),

    [_GAME] = KEYMAP( // TG(3)
        KC_ESCGR, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,  KC_GRV,  KC_BSDEL,
        KC_TAB,   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC, KC_BSLS,
        MO(_L),   KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN,  KC_QUOT,  XXX,     KC_ENT,
        KC_LSFT,  XXX,    KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,    KC_COMM, KC_DOT,   KC_SLSH,  XXX,     IME_ON,  TG(_GAME),

        XXX,      KC_LCTL, KC_LALT,                SFT_T(KC_SPC),         XXX,     KC_RALT,  KC_RCTL,  XXX)


};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}
uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    case _BASE:
    case _L:
    case _R:
        rgblight_setrgb (0xFF,  0x00, 0x00);
        break;
    case _GAME:
        rgblight_setrgb (0x7A,  0x00, 0xFF);
        break;
    default:
        rgblight_setrgb (0xFF,  0x00, 0x00);
        break;
    }
    return state;

}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool shift_pressed = get_mods() & MOD_BIT(KC_LSFT);
    bool ctrl_pressed  = get_mods() & MOD_BIT(KC_RCTL);
    switch (keycode) {
        case RESET:
            if (record->event.pressed) {
                rgblight_setrgb(0x00, 0x00, 0xFF);
            }
            return true;
        case KC_BSDEL:
            // LShift BS = DEL
            if (record->event.pressed) {
                bsdel_shifted = shift_pressed;
                if (shift_pressed) unregister_mods(MOD_LSFT);
                register_code(shift_pressed ? KC_DEL : KC_BSPC);
            } else {
                if (bsdel_shifted) register_mods(MOD_LSFT);
                unregister_code(bsdel_shifted ? KC_DEL : KC_BSPC);
            }
            return false;
        case KC_ESCGR:
            if (record->event.pressed) {
                // LShift-Esc = TILD, RCtrl-Esc  = GRV
                escgr_shifted = shift_pressed;
                escgr_ctrled  = ctrl_pressed;
                if (ctrl_pressed) unregister_mods(MOD_RCTL);
                register_code((shift_pressed || ctrl_pressed) ? KC_GRV : KC_ESC);
            } else {
                unregister_code((escgr_shifted || escgr_ctrled) ? KC_GRV : KC_ESC);
                if (escgr_ctrled) register_mods(MOD_RCTL);
            }
            return false;
        case IME_ON:
            // Ctrl-Shift-Ins
            if (record->event.pressed) {
                ctrl_interrupted[0] = false;
                scs_timer[0] = timer_read ();
                register_mods(MOD_RCTL);
            } else {
                if (!ctrl_interrupted[0] && timer_elapsed(scs_timer[0]) < TAPPING_TERM) {
                    if (shift_pressed) {
                        unregister_mods(MOD_LSFT);
                        register_code(KC_INS);
                        unregister_code(KC_INS);
                        register_mods(MOD_LSFT);
                    } else {
                        register_mods(MOD_LSFT);
                        register_code(KC_INS);
                        unregister_code(KC_INS);
                        unregister_mods(MOD_LSFT);
                    }
                }
                unregister_mods(MOD_RCTL);
            }
            return false;
        case IME_OFF:
            // Ctrl-Ins, Shift-IME_OFF: Ctrl-Shift-Ins(IME_ON)
            if (record->event.pressed) {
                ctrl_interrupted[1] = false;
                scs_timer[1] = timer_read ();
                register_mods(MOD_LCTL);
            } else {
                if (!ctrl_interrupted[1] && timer_elapsed(scs_timer[1]) < TAPPING_TERM) {
                    register_code(KC_INS);
                    unregister_code(KC_INS);
                }
                unregister_mods(MOD_LCTL);
            }
            return false;

        default:
            ctrl_interrupted[0] = true;
            ctrl_interrupted[1] = true;

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


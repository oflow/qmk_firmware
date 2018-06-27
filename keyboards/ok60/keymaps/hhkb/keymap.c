#include "ok60.h"

#define XXXXX  KC_NO
#define _____  KC_TRNS
#define ___    KC_TRNS

#define RGB_PLN RGB_MODE_PLAIN
#define RGB_BRT RGB_MODE_BREATHE

enum custom_keycodes {
    KC_ESCGR,   // Esc, ~, `
    KC_BSDEL,   // BS, Del
    IME_ON,  // Ctrl + Shift + Ins
    IME_OFF  // ctrl + Ins
};
static bool bsdel_shifted = false;
static bool escgr_shifted = false;
static bool escgr_ctrled = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ,----------------------------------------------------------------------------------------.
     * | ESC |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |  `  | BS |
     * |----------------------------------------------------------------------------------------|
     * | TAB  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \    |
     * |----------------------------------------------------------------------------------------|
     * | MO(1) |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |     Enter    |
     * |----------------------------------------------------------------------------------------|
     * | IME OFF |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |  IME ON  | MO(2) |
     * `----------------------------------------------------------------------------------------'
     *         | Ctl |  Alt  |               Shift & Space               |  Alt  | Ctl |
     *         `-----------------------------------------------------------------------'
     *
     */
    KEYMAP(
        KC_ESCGR, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,  KC_GRV,  KC_BSDEL,
        KC_TAB,   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC, KC_BSLS,
        MO(1),    KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN,  KC_QUOT,  XXXXX,   KC_ENT,
        IME_OFF,  XXXXX,  KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,  KC_DOT,   KC_SLSH,  XXXXX,   IME_ON,  MO(2),
        XXXXX,   KC_LCTL, KC_LALT,                SFT_T(KC_SPC),          XXXXX,  KC_RALT,  KC_RCTL,  XXXXX),

    KEYMAP( // MO(1) : left side modifer
        RESET,    KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,   KC_F9,   KC_F10,   KC_F11,   KC_F12,  KC_INS,  KC_DEL,
        RGUI(KC_TAB), ___, ___,   _____,  _____,  _____,  _____,  _____, S(KC_INS), _____,  KC_PSCR,  KC_HOME,  KC_END,  _____,
        _____,    _____,  _____,  _____,  _____,  _____, KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT, _____,    _____,    _____,   _____,
        _____,    _____,  _____,  _____,  _____,  _____,  _____,  LCTL(KC_N), ___, KC_PGDN, KC_PGUP,  _____,    _____,   IME_OFF, _____,
        _____,   KC_LGUI, _____,                     KC_SPC,              _____,   _____,   KC_RGUI,  _____),

    KEYMAP( // MO(2) : right side modifer
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9, KC_F10, KC_F11, KC_F12, _____, LCTL(LALT(KC_DEL)),
        _____,   RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _____, _____,  _____,  _____,  _____,
        KC_CAPS, RGB_PLN, RGB_BRT, _____,   _____,   _____,   _____,   _____,   _____,   _____, _____,  _____,  _____,  _____,
        _____,   _____,   _____,   _____,   _____,   _____,   _____,   _____,   _____,   _____, _____,  _____,  _____,  TG(3), _____,
        _____,   KC_APP,  _____,                     KC_SPC,                    _____,   _____, KC_APP, _____),
    KEYMAP(
        KC_ESCGR, KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,     KC_MINS,  KC_EQL,  KC_GRV,  KC_BSDEL,
        KC_TAB,   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,     KC_LBRC,  KC_RBRC, KC_BSLS,
        MO(1),    KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN,  KC_QUOT,  XXXXX,   KC_ENT,
        KC_LSFT,  XXXXX,  KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,  KC_DOT,   KC_SLSH,  XXXXX,   IME_ON,  TG(3),

        XXXXX,   KC_LCTL, KC_LALT,                SFT_T(KC_SPC),          XXXXX,  KC_RALT,  KC_RCTL,  XXXXX)


};

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}
uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
    case 0:
    case 1:
    case 2:
        rgblight_setrgb (0xFF,  0x00, 0x00);
        break;
    case 3:
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
            // hold BS, DEL
            if (record->event.pressed) {
                bsdel_shifted = shift_pressed;
                if (shift_pressed) unregister_code(KC_LSFT);
                register_code(shift_pressed ? KC_DEL : KC_BSPC);
            } else {
                if (bsdel_shifted) register_code(KC_LSFT);
                unregister_code(bsdel_shifted ? KC_DEL : KC_BSPC);
            }
            return false;
        case KC_ESCGR:
            if (record->event.pressed) {
                // LShift-Esc = TILD, RCtrl-Esc  = GRV
                escgr_shifted = shift_pressed;
                escgr_ctrled  = ctrl_pressed;
                if (ctrl_pressed) unregister_code(KC_RCTL);
                register_code((shift_pressed || ctrl_pressed) ? KC_GRV : KC_ESC);
            } else {
                unregister_code((escgr_shifted || escgr_ctrled) ? KC_GRV : KC_ESC);
                if (escgr_ctrled) register_code(KC_RCTL);
            }
            return false;
        case IME_ON:
            // Ctrl-Shift-Ins
            if (record->event.pressed) {
                if (shift_pressed) {
                    unregister_code(KC_LSFT);
                    register_code(KC_LCTL);
                    register_code(KC_INS);
                    unregister_code(KC_INS);
                    unregister_code(KC_LCTL);
                    register_code(KC_LSFT);
                } else {
                    register_code(KC_LCTL);
                    register_code(KC_LSFT);
                    register_code(KC_INS);
                    unregister_code(KC_INS);
                    unregister_code(KC_LSFT);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
        case IME_OFF:
            // Ctrl-Ins, Shift-IME_OFF: Ctrl-Shift-Ins(IME_ON)
            if (record->event.pressed) {
                register_code(KC_LCTL);
                register_code(KC_INS);
                unregister_code(KC_INS);
                unregister_code(KC_LCTL);
            }
            return false;

        default:
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


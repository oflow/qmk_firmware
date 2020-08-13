#include QMK_KEYBOARD_H

#ifndef IME_TAPPING_TERM
#define IME_TAPPING_TERM 250
#endif


#define XXX  KC_NO
#define ___  KC_TRNS

#define RGB_PLN RGB_MODE_PLAIN
#define RGB_BRT RGB_MODE_BREATHE

#define _BASE  0
#define _GAME  1  // for FPS Game (need LShift)
#define _L     2
#define _R     3

// short modifier: S(kc) = LSFT(kc) = (kc | QK_LSFT)
/*
#define C(kc)      (kc | QK_LCTL)
#define S(kc)      (kc | QK_LSFT)
#define A(kc)      (kc | QK_LALT)
#define G(kc)      (kc | QK_LGUI)
*/
#define CA(kc)     (kc | QK_LCTL | QK_LALT)
#define CS(kc)     (kc | QK_LCTL | QK_LSFT)
#define CG(kc)     (kc | QK_LCTL | QK_LGUI)
#define SA(kc)     (kc | QK_LSFT | QK_LALT)
#define SG(kc)     (kc | QK_LSFT | QK_LGUI)

enum custom_keycodes {
    KC_ESGR = SAFE_RANGE, // Esc, Shift: ~, Ctrl: `
    KC_BSDL,  // BS,  Shift: Del
    KC_MF12,  // F12, Shift: F11,     Ctrl: F13
    KC_BASE,  // TO(_BASE) + Ctrl-Ins (IME_OFF)
    IME_ON,   // Tap: Ctrl-Shift-Ins, Hold: RCtrl
    IME_OFF,  // Tap: Ctrl-Ins,       Hold: LCtrl
    IME_TG    // Tap: Ctrl-Space      Hold: LCtrl
};

static bool mods_bsdl[2] = {0, 0};       // lsft, rsft
static bool mods_esgr[4] = {0, 0, 0, 0}; // lsft, rsft, lctl, rctl

static bool ctrl_interrupted[3] = {0, 0, 0};
static uint16_t scs_timer[3] = {0, 0, 0};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
     * ,----------------------------------------------------------------------------------------.
     * | ESC |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |    BS    |
     * |----------------------------------------------------------------------------------------|
     * | TAB  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \    |
     * |----------------------------------------------------------------------------------------|
     * | MO(L) |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |     Enter    |
     * |----------------------------------------------------------------------------------------|
     * | IME OFF |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |  IME ON  | MO(R) |
     * |----------------------------------------------------------------------------------------|
     * | LCtl | LGUI | LAlt |             Shift & Space              | RAlt | RGUI | APP | RCtl |
     * `----------------------------------------------------------------------------------------'
     */
  // 0: Base Layer
  [_BASE] = LAYOUT_60_iso_splitrshift(
      KC_ESGR, KC_1,    KC_2,    KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSDL,  \
      KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,  \
      MO(_L),  KC_A,    KC_S,    KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,   \
      IME_OFF,          KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, IME_ON,  MO(_R),   \
      KC_LCTL, KC_LGUI, KC_LALT,                 LSFT_T(KC_SPC),                          KC_RALT, KC_RGUI, KC_APP,  KC_RCTL),

    /*
     * ,----------------------------------------------------------------------------------------.
     * | ESC |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |    BS    |
     * |----------------------------------------------------------------------------------------|
     * | TAB  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \    |
     * |----------------------------------------------------------------------------------------|
     * | (1,M) |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |     Enter    |
     * |----------------------------------------------------------------------------------------|
     * | LShift  |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |    ↑    | TO(0) |
     * |----------------------------------------------------------------------------------------|
     * | LCtl |  K   |  L   |                Space                | IME TG |  ←  |  ↓  |  →  |
     * `----------------------------------------------------------------------------------------'
     */

  // 1: Game (base)
  [_GAME] = LAYOUT_60_iso_splitrshift(
      KC_ESC,    KC_1,  KC_2,    KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   \
      KC_TAB,    KC_Q,  KC_W,    KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,   \
      LT(2,KC_M),KC_A,  KC_S,    KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,    \
      KC_LSFT,          KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, IME_ON,  KC_BASE,   \
      KC_LCTL,   KC_K,  KC_L,                 KC_SPC,                                     KC_RALT, KC_RGUI, A(KC_PSCR),A(KC_F10)),


    /*
     * ,----------------------------------------------------------------------------------------.
     * | ESC |  F1 |  F2 |  F3 |  F4 |  F5 |  F6 |  F7 |  F8 |  F9 | F10 | F11 | F12 |   Del    |
     * |----------------------------------------------------------------------------------------|
     * | G-TAB|S-C-q| G-w |     |     |     |     |     |S-Ins|     | PrScr| Home | End | Pipe  |
     * |----------------------------------------------------------------------------------------|
     * |       |S-A-A|S-G-S|     |     |     |  ← |  ↓ |  ↑ |  → |  ;  |  '  |     Enter    |
     * |----------------------------------------------------------------------------------------|
     * | LShift  |     |     | G-k |     |     | C-n | A-m | PgUp| PgDn|     |          |       |
     * |----------------------------------------------------------------------------------------|
     * | LCtl | LGUI | LAlt |               Space                  |     |     | A-PsCr | C-F10 |
     * `----------------------------------------------------------------------------------------'
     */

  // 2: Left Function Layer
  // G-k: Bluetooth devices
  [_L] = LAYOUT_60_iso_splitrshift(
      RESET,   KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,   \
      G(KC_TAB),CS(KC_Q),G(KC_W),___,     ___,    ___,    ___,    ___,    S(KC_INS),___,   KC_PSCR, KC_HOME, KC_END,  KC_PIPE,  \
      MO(1),   SA(KC_A),SG(KC_S),___,     ___,    ___,    KC_LEFT,KC_DOWN,KC_UP,  KC_RGHT, ___,     CS(KC_QUOT),      ___,      \
      KC_LSFT,          ___,     ___,     G(KC_K),___,     ___,    C(KC_N),A(KC_M),KC_PGUP, KC_PGDN, ___,     IME_OFF, ___,      \
      ___,      ___,    ___,                      KC_SPC,                                  ___,     ___,    A(KC_PSCR),A(KC_F10)),

    /*
     * ,----------------------------------------------------------------------------------------.
     * |  `  |  F1 |  F2 |  F3 |  F4 |  F5 |  F6 |  F7 |  F8 |  F9 | F10 | F11 | F12 | C-A-del  |
     * |----------------------------------------------------------------------------------------|
     * | >RGB | TOG |     |     |     |     |     |     |     |     |      | PLN | BRT  |       |
     * |----------------------------------------------------------------------------------------|
     * | Stop  |Prev |Play |Next |     |     |     |     |     |     |     |     |              |
     * |----------------------------------------------------------------------------------------|
     * | LShift  |Vol- |Vol+ | G-k |     |     | C-n | A-m | PgUp| PgDn| BT  |  TO(1)   |       |
     * |----------------------------------------------------------------------------------------|
     * | S-G-< |     |      |            Shift+Space               |     |     |        | S-G-> |
     * `----------------------------------------------------------------------------------------'
     */


  // 3: Right Function Layer
  [_R] = LAYOUT_60_iso_splitrshift(
      KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,   KC_F10,  KC_F11,   KC_F12,  CA(KC_DEL),  \
      ___,     RGB_TOG, ___,     ___,     ___,    ___,    ___,    ___,    ___,    ___,     ___,     RGB_PLN,  RGB_BRT, ___,         \
      KC_MSTP, KC_MPRV, KC_MPLY, KC_MNXT, ___,    ___,    ___,    ___,    ___,    ___,     ___,     ___,               ___,         \
      KC_LSFT,          KC_VOLD, KC_VOLU, G(KC_K),___,    ___,    ___,    ___,    ___,     ___,     G(KC_K),  TO(1),   ___,         \
      SG(KC_LEFT),___,  ___,                      S(KC_SPC),                               ___,     ___,      ___,     SG(KC_RGHT)),


};

// Loop
void matrix_scan_user(void) {
};

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
            // C-F12 = F11
            if (record->event.pressed) {
                if (rctl_pressed || lctl_pressed) {
                    unregister_mods(rctl_pressed ? MOD_RCTL : MOD_LCTL);
                    register_code(KC_F11);
                    unregister_code(KC_F11);
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
        case KC_BASE:
            if (record->event.pressed) {
                layer_move(_BASE);
                register_mods(MOD_LCTL);
                register_code(KC_INS);
                unregister_code(KC_INS);
                unregister_mods(MOD_LCTL);
            } else {
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


/* Copyright 2021 Andreas Karlsvik
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
#include "keymap_norwegian.h"
#include <stdio.h>

#ifdef OLED_ENABLE
 #include "oled.c"
#endif

#ifdef ENCODER_ENABLE
    #include "gamepad_encoder.c"
#endif

enum layer {
    _QWERTY,
    _GAMING,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    KC_GAMING,
    KC_ADJUST,
    KC_PRVWD,
    KC_NXTWD,
    KC_LSTRT,
    KC_LEND,
    KC_DLINE,
    KC_APOSMFIX,
    KC_ATMFIX,
    KC_GRTRMFIX,
    KC_LESSMFIX,
    KC_LCBRMFIX,
    KC_RCBRMFIX,
    KC_PIPEMFIX,
    KC_BSLSMFIX
  };

#define KC_LOWER MO(_LOWER)
#define KC_RAISE MO(_RAISE)
#define NO_ACUT_MAC NO_BSLS       // ´
#define NO_APOS_MAC NO_LABK       // '
#define NO_AT_MAC   NO_QUOT       // @
#define NO_BSLS_MAC S(ALGR(NO_7)) // (backslash)
#define NO_DLR_MAC  S(NO_4)       // $
#define NO_GRV_MAC  ALGR(NO_BSLS) // `
#define NO_GRTR_MAC S(NO_PIPE)    // >
#define NO_LCBR_MAC S(ALGR(NO_8)) // {
#define NO_LESS_MAC NO_PIPE       // <
#define NO_PIPE_MAC ALGR(NO_7)    // |
#define NO_RCBR_MAC S(ALGR(NO_9)) // }

#define LTAPL LT(KC_LOWER, KC_BSPC)
#define LTAPR TG(KC_RAISE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
                 KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,     KC_5,    KC_A,
                 KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,     KC_T,    KC_B,
        	     KC_LGUI,  KC_A,    KC_S,    KC_D,    KC_F,     KC_G,
                 KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,     KC_B,    KC_MUTE,  LTAPR,
                                             KC_LCTRL,LTAPL,    KC_LALT, KC_SPC,   KC_ENT
    ),
    [_GAMING] = LAYOUT(
                 KC_ESC,  NO_ACUT,  KC_1,    KC_2,    KC_3,    KC_4,     KC_5,
                 XXXXXXX, KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,     KC_6,
        	     XXXXXXX, KC_LSFT,  KC_A,    KC_S,    KC_D,    KC_F,
                 XXXXXXX, KC_LCTRL, KC_Z,    KC_X,    KC_C,    KC_V,     KC_MUTE,  LTAPR,
                                             KC_LCTRL,KC_LOWER,KC_LALT,  KC_SPC,   KC_ENT
    ),
    [_LOWER] = LAYOUT(
                 _______, KC_F1,   KC_F2,     KC_F3,   KC_F4,   XXXXXXX,     _______,
                 _______, KC_F5,   KC_F6,     KC_F7,   KC_8,    XXXXXXX,     _______,
        	     _______, KC_F9,   KC_F10,    KC_F11,  KC_F12,  _______,
                 _______, _______, _______,   _______, _______, _______,     _______, _______,
                                              _______, _______, _______,     _______, _______
    ),
    [_RAISE] = LAYOUT(
                 _______, _______, _______, _______, _______,  _______, _______,
                 _______, _______, _______, _______, _______,  _______, _______,
        	     _______, _______, _______, _______, _______,  _______,
                 _______, _______, _______, _______, _______,  _______, _______, _______,
                                            _______, _______,  _______, _______, _______
    ),
    [_ADJUST] = LAYOUT(
                 XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX,
                 RESET,   XXXXXXX, KC_QWERTY, KC_GAMING, CG_TOGG, XXXXXXX, XXXXXXX,
        	     XXXXXXX, XXXXXXX, CG_TOGG,   XXXXXXX,   XXXXXXX, XXXXXXX,
                 XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, _______,
                                              _______,   _______, _______, KC_MS_BTN1, _______
    ),

};

#define LOWER_AND_RAISE ((1UL << _LOWER) | (1UL << _RAISE))

layer_state_t layer_state_set_kb(layer_state_t state) {
  if ((state & LOWER_AND_RAISE) == LOWER_AND_RAISE) {
    state |= 1UL << _ADJUST;
  } else {
    state &= ~(1UL << _ADJUST);
  }
  return state;
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(KC_LOWER, KC_BSPC):
            return true;
        default:
            return false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case KC_GAMING:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_GAMING);
            }
            return false;
        case KC_PRVWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_NXTWD:
             if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_LSTRT:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                     //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
            }
            break;
        case KC_LEND:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
            }
            break;
        case KC_DLINE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_BSPC);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_BSPC);
            }
            break;
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
            }
            return false;
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
            }
            return false;
        case KC_APOSMFIX:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    tap_code16(NO_APOS_MAC);
                } else {
                    tap_code16(NO_QUOT);
                }
            }
            return false;
            break;
        case KC_ATMFIX:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    tap_code16(NO_AT_MAC);
                } else {
                    tap_code16(NO_AT);
                }
            }
            return false;
            break;
        case KC_GRTRMFIX:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    tap_code16(NO_GRTR_MAC);
                } else {
                    tap_code16(NO_RABK);
                }
            }
            return false;
            break;
        case KC_LESSMFIX:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    tap_code16(NO_LESS_MAC);
                } else {
                    tap_code16(NO_LABK);
                }
            }
            return false;
            break;
        case KC_LCBRMFIX:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    tap_code16(NO_LCBR_MAC);
                } else {
                    tap_code16(NO_LCBR);
                }
            }
            return false;
            break;
        case KC_RCBRMFIX:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    tap_code16(NO_RCBR_MAC);
                } else {
                    tap_code16(NO_RCBR);
                }
            }
            return false;
            break;
        case KC_PIPEMFIX:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    tap_code16(NO_PIPE_MAC);
                } else {
                    tap_code16(NO_PIPE);
                }
            }
            return false;
            break;
        case KC_BSLSMFIX:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    tap_code16(NO_BSLS_MAC);
                } else {
                    tap_code16(NO_BSLS);
                }
            }
            return false;
            break;
        }
    return true;
}

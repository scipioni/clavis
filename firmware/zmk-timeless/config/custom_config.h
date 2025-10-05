// Custom configuration for ZMK firmware with Miryoku layout
// https://github.com/manna-harbour/miryoku

#pragma once

// Tapping term for non-HRM hold-tap behaviors (HRMs use optimized values in devicetree)
// Standard tapping term is only used for layer-taps and other non-HRM dual-role keys
#define U_TAPPING_TERM 175

// Bluetooth configuration
// Customize the keyboard name that appears when pairing via Bluetooth
// Uncomment and modify the line below to set your preferred keyboard name
// #define CONFIG_ZMK_KEYBOARD_NAME "My Custom Keyboard"

// Custom layer definitions
// These override the default Miryoku layers

// MIRYOKU_ALTERNATIVES_BASE_COLEMAKDH
// #define MIRYOKU_LAYER_BASE, \
// &kp Q,             &kp W,             &kp F,             &kp P,             &kp B,             &kp J,             &kp L,             &kp U,             &kp Y,             &kp SEMI, \
// U_MT(LGUI, A),     U_MT(LALT, R),     U_MT(LCTRL, S),    U_MT(LSHFT, T),    &kp G,             &kp M,             U_MT(LSHFT, N),    U_MT(LCTRL, E),    U_MT(LALT, I),     U_MT(LGUI, O), \
// U_LT(U_BUTTON, Z), U_MT(RALT, X),     &kp C,             &kp D,             &kp V,             &kp K,             &kp H,             &kp COMMA,         U_MT(RALT, DOT),   U_LT(U_BUTTON, SLASH), \
// U_NP,              U_NP,              U_LT(U_MEDIA, ESC),U_LT(U_NAV, SPACE),U_LT(U_MOUSE, TAB),U_LT(U_SYM, RET),  U_LT(U_NUM, BSPC), U_LT(U_FUN, DEL),  U_NP,              U_NP

// Optimized Miryoku layer with positional hold-tap behaviors for HRMs (as per tuning guide)
#define MIRYOKU_LAYER_BASE \
    &kp Q,                &kp W,                &kp F,                &kp P,                &kp B,                &kp J,               &kp L,                &kp U,                &kp Y,                &kp SEMI, \
    U_MTL(LGUI, A),       U_MTL(LALT, R),       U_MTL(LCTRL, S),      U_MTL(LSHFT, T),      &kp G,                &kp M,               U_MTR(LSHFT, N),      U_MTR(LCTRL, E),      U_MTR(LALT, I),       U_MTR(LGUI, O), \
    U_LT(U_BUTTON, Z),    U_MT(RALT, X),        &kp C,                &kp D,                &kp V,                &kp K,               &kp H,                &kp COMMA,            U_MT(RALT, DOT),      U_LT(U_BUTTON, SLASH), \
    U_NP,                 U_NP,                 U_LTL(U_MEDIA, ESC),  U_LTL(U_NAV, SPACE),  U_LTL(U_MOUSE, TAB),  U_LTR(U_SYM, RET),   U_LTR(U_NUM, BSPC),   U_LTR(U_FUN, DEL),    U_NP,                 U_NP

// Media layer with numpad and media controls
#define MIRYOKU_LAYER_MEDIA \
    U_NA,                 &kp KP_N7,            &kp KP_N8,            &kp KP_N9,            &kp KP_MINUS,         U_RGB_TOG,           U_RGB_EFF,            U_RGB_HUI,            U_RGB_SAI,            U_RGB_BRI, \
    U_NA,                 &kp KP_N4,            &kp KP_N5,            &kp KP_N6,            &kp KP_PLUS,          U_EP_TOG,            &kp C_PREV,           &kp C_VOL_DN,         &kp C_VOL_UP,         &kp C_NEXT, \
    &kp KP_N0,            &kp KP_N1,            &kp KP_N2,            &kp KP_N3,            &kp KP_DOT,           &u_out_tog,          &u_bt_sel_0,          &u_bt_sel_1,          &u_bt_sel_2,          &u_bt_sel_3, \
    U_NP,                 U_NP,                 U_NA,                 U_NA,                 U_NA,                 &kp C_STOP,          &kp C_PP,             &kp C_MUTE,           U_NP,                 U_NP

// Number layer with numbers and symbols
#define MIRYOKU_LAYER_NUM \
    &kp LBKT,             &kp N7,               &kp N8,               &kp N9,               &kp RBKT,             U_NA,                &u_to_U_BASE,         &u_to_U_EXTRA,        &u_to_U_TAP,          U_BOOT, \
    &kp SQT,              &kp N4,               &kp N5,               &kp N6,               &kp EQUAL,            U_NA,                &kp LSHFT,            &kp LCTRL,            &kp LALT,             &kp LGUI, \
    &kp GRAVE,            &kp N1,               &kp N2,               &kp N3,               &kp BSLH,             U_NA,                &kp RA(SQT),          &kp RA(DQT),          &kp RALT,             U_NA, \
    U_NP,                 U_NP,                 &kp DOT,              &kp N0,               &kp MINUS,            U_NA,                U_NA,                 U_NA,                 U_NP,                 U_NP

// Navigation layer with cursor keys and shortcuts
#define MIRYOKU_LAYER_NAV \
    U_BOOT,               &u_to_U_TAP,          &u_to_U_EXTRA,        &u_to_U_BASE,         U_NA,                U_RDO,                U_PST,                U_CPY,                U_CUT,                U_UND, \
    &kp LGUI,             &kp LALT,             &kp LCTRL,            &kp LSHFT,            U_NA,                &u_caps_word,         &kp LEFT,             &kp DOWN,             &kp UP,               &kp RIGHT, \
    U_NA,                 &kp LC(X),            &kp LC(C),            &kp LC(V),            U_NA,                &kp INS,              &kp HOME,             &kp PG_DN,            &kp PG_UP,            &kp END, \
    U_NP,                 U_NP,                 U_NA,                 U_NA,                 U_NA,                &kp RET,              &kp BSPC,             &kp DEL,              U_NP,                 U_NP

// Function layer with F-keys
#define MIRYOKU_LAYER_FUN \
    &kp F12,              &kp F7,               &kp F8,               &kp F9,               &kp PSCRN,            U_NA,                &u_to_U_BASE,         &u_to_U_EXTRA,        &u_to_U_TAP,          U_BOOT, \
    &kp F11,              &kp F4,               &kp F5,               &kp F6,               &kp SLCK,             U_NA,                &kp LSHFT,            &kp LCTRL,            &kp LALT,             &kp LGUI, \
    &kp F10,              &kp F1,               &kp F2,               &kp F3,               &kp PAUSE_BREAK,      U_NA,                &u_to_U_FUN,          &u_to_U_MEDIA,        &kp RALT,             U_NA, \
    U_NP,                 U_NP,                 &kp K_APP,            &kp SPACE,            &kp TAB,              U_NA,                U_NA,                 U_NA,                 U_NP,                 U_NP

// Button layer with mouse buttons
#define MIRYOKU_LAYER_BUTTON \
    U_UND,                U_CUT,                U_CPY,                U_PST,                U_RDO,                U_RDO,               U_PST,                U_CPY,                U_CUT,                U_UND, \
    &kp LGUI,             &kp LALT,             &kp LCTRL,            &kp LSHFT,            U_NU,                 U_NU,                &kp LSHFT,            &kp LCTRL,            &kp LALT,             &kp LGUI, \
    U_UND,                U_CUT,                U_CPY,                U_PST,                U_RDO,                U_RDO,               U_PST,                U_CPY,                U_CUT,                U_UND, \
    U_NP,                 U_NP,                 U_BTN3,               U_BTN1,               U_BTN2,               U_BTN2,              U_BTN1,               U_BTN3,               U_NP,                 U_NP

// Symbol layer with special characters
#define MIRYOKU_LAYER_SYM \
    &kp LBRC,             &kp AMPS,             &kp ASTRK,            &kp LPAR,             &kp RBRC,             U_NA,                &u_to_U_BASE,         &u_to_U_EXTRA,        &u_to_U_TAP,          U_BOOT, \
    &kp COLON,            &kp DLLR,             &kp PRCNT,            &kp CARET,            &kp PLUS,             U_NA,                &kp LSHFT,            &kp LCTRL,            &kp LALT,             &kp LGUI, \
    &kp TILDE,            &kp EXCL,             &kp AT,               &kp HASH,             &kp PIPE,             U_NA,                &u_to_U_SYM,          &u_to_U_MOUSE,        &kp RALT,             U_NA, \
    U_NP,                 U_NP,                 &kp LPAR,             &kp RPAR,             &kp UNDER,            U_NA,                U_NA,                 U_NA,                 U_NP,                 U_NP

// Mouse layer with mouse keys
#define MIRYOKU_LAYER_MOUSE \
    U_BOOT,               &u_to_U_TAP,          &u_to_U_EXTRA,        &u_to_U_BASE,         U_NA,                 U_RDO,               U_PST,                U_CPY,                U_CUT,                U_UND, \
    &kp LGUI,             &kp LALT,             &kp LCTRL,            &kp LSHFT,            U_NA,                 U_NU,                U_MS_L,               U_MS_D,               U_MS_U,               U_MS_R, \
    U_NA,                 &kp RALT,             &u_to_U_SYM,          &u_to_U_MOUSE,        U_NA,                 U_NU,                U_WH_L,               U_WH_D,               U_WH_U,               U_WH_R, \
    U_NP,                 U_NP,                 U_NA,                 U_NA,                 U_NA,                 U_BTN2,              U_BTN1,               U_BTN3,               U_NP,                 U_NP



#pragma once

#undef USE_I2C
#undef SSD1306OLED
#define USE_SERIAL_PD2

#define SPLIT_USB_DETECT
#define SPLIT_USB_TIMEOUT 5000
#define SPLIT_USB_TIMEOUT_POLL 25

/* Select hand configuration */
#define MASTER_LEFT
//#define MASTER_RIGHT

// sets the keyboard's handedness by setting a flag in the persistent storage (EEPROM)
// make crkbd:clavis:avrdude-split-left
// make crkbd:clavis:avrdude-split-right
//#define EE_HANDS

#define USE_SERIAL_PD2

// Prevent normal rollover on alphas from accidentally triggering mods.
#define IGNORE_MOD_TAP_INTERRUPT

#undef TAPPING_TERM
#define TAPPING_TERM 200

// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
#define TAPPING_FORCE_HOLD

// Auto Shift
#define NO_AUTO_SHIFT_ALPHA
#define AUTO_SHIFT_TIMEOUT TAPPING_TERM
#define AUTO_SHIFT_NO_SETUP

// Recommended for heavy chording.
#define QMK_KEYS_PER_SCAN 4

// Mouse key speed and acceleration.
#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY          0
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL       16
#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY    0
#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED      6
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX    64




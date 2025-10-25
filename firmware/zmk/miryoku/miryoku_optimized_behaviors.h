// Optimized ZMK Behavior Macros for Miryoku
// Balanced reliability and responsiveness for medium-speed typing

#pragma once

// Home Row Modifiers with positional logic
#define U_HML(MOD, TAP) &hml MOD TAP  // Home Row Mod - Left
#define U_HMR(MOD, TAP) &hmr MOD TAP  // Home Row Mod - Right

// Layer Tap behaviors
#define U_LTB(LAYER, TAP) &ltb LAYER TAP  // Layer Tap - Balanced

// Simple Mod Tap for non-home-row keys
#define U_MTS(MOD, TAP) &mts MOD TAP  // Mod Tap - Simple

// Fallback behaviors for compatibility
#define U_MT(MOD, TAP) &u_mt MOD TAP   // Universal Mod Tap
#define U_LT(LAYER, TAP) &u_lt LAYER TAP // Universal Layer Tap

// Convenience macros for common use cases
#define U_HRM_LEFT(MOD, TAP) U_HML(MOD, TAP)
#define U_HRM_RIGHT(MOD, TAP) U_HMR(MOD, TAP)

// For thumb keys and other layer switches
#define U_LAYER(LAYER, TAP) U_LTB(LAYER, TAP)

// For non-home-row modifier taps (like Alt on thumb keys)
#define U_MOD_TAP(MOD, TAP) U_MTS(MOD, TAP)
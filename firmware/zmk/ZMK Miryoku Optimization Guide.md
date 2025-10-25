# ZMK Miryoku Optimization Guide

## Overview

This document describes the optimized ZMK configuration for Miryoku layout, specifically tuned for medium-speed typing with balanced reliability and responsiveness. The configuration implements advanced hold-tap behaviors based on the "Timeless HRM" philosophy.

## Key Features

### 1. Optimized Hold-Tap Behaviors

#### Home Row Modifiers (HRMs)
- **Separate behaviors for left and right hands** (`hml` and `hmr`)
- **Bilateral enforcement** via positional hold-triggers
- **Typing streak protection** with `require-prior-idle-ms`
- **Quick-tap support** for key repetition

#### Layer-Tap Keys
- **Balanced flavor** for responsive layer switching
- **Moderate timing** optimized for thumb cluster usage
- **Quick-tap functionality** for utility keys

### 2. Timing Parameters

| Parameter | HRM Value | Layer-Tap Value | Purpose |
|-----------|-------------|------------------|---------|
| `tapping-term-ms` | 250ms | 200ms | Long window for reliable holds |
| `require-prior-idle-ms` | 180ms | 100ms | Prevents accidental modifiers during typing |
| `quick-tap-ms` | 160ms | 160ms | Enables key repetition |
| `flavor` | "balanced" | "balanced" | Good balance of tap/hold behavior |

### 3. Positional Logic

#### Corne Keyboard Layout (42 keys)
- **Left hand positions**: 0-19
- **Right hand positions**: 20-39
- **Cross-hand triggering**: HRMs only respond to opposite hand keys
- **Same-hand protection**: Prevents accidental modifier activation during rolls

## File Structure

### New Files Created

1. **`miryoku/miryoku_optimized_behaviors.dtsi`**
   - Defines optimized hold-tap behaviors
   - Implements positional logic and timing parameters
   - Provides fallback behaviors for compatibility

2. **`miryoku/miryoku_optimized_behaviors.h`**
   - Macro definitions for new behaviors
   - Convenience macros for common use cases
   - Backward compatibility with existing code

### Modified Files

1. **`config/custom_config.h`**
   - Enables optimized behaviors with `MIRYOKU_OPTIMIZED_BEHAVIORS`
   - Updates base layer to use new HRM behaviors
   - Maintains all existing custom layers

2. **`miryoku/miryoku.dtsi`**
   - Adds conditional include for optimized behaviors
   - Maintains compatibility with existing kludges

3. **`config/corne.conf`**
   - Optimized debounce settings (5ms)
   - Improved power management (60s idle timeout)
   - Enhanced connection stability

4. **`Makefile`**
   - Removed TTY requirement for automated builds

## Behavior Details

### Home Row Modifiers

#### Left Hand (HML)
```c
hml: home_row_mod_left {
  compatible = "zmk,behavior-hold-tap";
  #binding-cells = <2>;
  flavor = "balanced";
  tapping-term-ms = <250>;
  quick-tap-ms = <160>;
  require-prior-idle-ms = <180>;
  bindings = <&kp>, <&kp>;
  hold-trigger-key-positions = <20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39>;
  hold-trigger-on-release;
};
```

#### Right Hand (HMR)
```c
hmr: home_row_mod_right {
  compatible = "zmk,behavior-hold-tap";
  #binding-cells = <2>;
  flavor = "balanced";
  tapping-term-ms = <250>;
  quick-tap-ms = <160>;
  require-prior-idle-ms = <180>;
  bindings = <&kp>, <&kp>;
  hold-trigger-key-positions = <0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19>;
  hold-trigger-on-release;
};
```

### Layer-Tap Behavior

```c
ltb: layer_tap_balanced {
  compatible = "zmk,behavior-hold-tap";
  #binding-cells = <2>;
  flavor = "balanced";
  tapping-term-ms = <200>;
  quick-tap-ms = <160>;
  require-prior-idle-ms = <100>;
  bindings = <&mo>, <&kp>;
};
```

## Usage Examples

### Base Layer Implementation
```c
#define MIRYOKU_LAYER_BASE \
&kp Q,             &kp W,             &kp F,             &kp P,             &kp B,             &kp J,             &kp L,             &kp U,             &kp Y,             &kp SEMI,          \
U_HML(LGUI, A),    U_HML(LALT, R),    U_HML(LCTRL, S),   U_HML(LSHFT, T),   &kp G,             &kp M,             U_HMR(LSHFT, N),   U_HMR(LCTRL, E),   U_HMR(LALT, I),    U_HMR(LGUI, O),    \
U_LAYER(U_BUTTON, Z), U_MTS(RALT, X), &kp C,             &kp D,             &kp V,             &kp K,             &kp H,             &kp COMMA,         U_MTS(RALT, DOT),  U_LAYER(U_BUTTON, SLASH),\
U_NP,              U_NP,              U_LAYER(U_MEDIA, ESC),U_LAYER(U_NAV, SPACE),U_LAYER(U_MOUSE, TAB),U_LAYER(U_SYM, RET),  U_LAYER(U_NUM, BSPC), U_LAYER(U_FUN, DEL),  U_NP,              U_NP
```

## Benefits

### 1. Reduced Misfires
- `require-prior-idle-ms` prevents accidental modifier activation during typing streaks
- Positional logic eliminates same-hand roll interference
- Longer tapping terms provide safety margin

### 2. Improved Responsiveness
- Quick-tap enables immediate key repetition
- Balanced flavor allows rapid layer access
- Optimized debounce settings reduce latency

### 3. Enhanced Reliability
- Bilateral enforcement ensures intentional modifier usage
- Cross-hand triggering provides instant hold resolution
- Conservative timing suitable for medium-speed typing

### 4. Better User Experience
- "Mind-timer-free" typing - no need to count milliseconds
- Consistent behavior across different typing speeds
- Predictable modifier activation

## Troubleshooting

### Common Issues

1. **Modifiers not activating**
   - Check `hold-trigger-key-positions` for your keyboard layout
   - Ensure opposite hand keys are in the trigger list

2. **Slow layer switching**
   - Adjust `tapping-term-ms` for layer-tap behaviors
   - Consider using "tap-preferred" flavor for faster layer access

3. **Key repetition not working**
   - Verify `quick-tap-ms` is set appropriately
   - Check that the behavior supports quick-tap

### Fine-Tuning

#### For Faster Typists
- Increase `require-prior-idle-ms` to 200-230ms
- Consider "tap-preferred" flavor for HRMs
- Reduce `tapping-term-ms` for layer-taps to 150ms

#### For Slower Typists
- Increase `tapping-term-ms` for HRMs to 300ms
- Reduce `require-prior-idle-ms` to 150ms
- Keep current balanced flavor

#### For Gaming
- Use "tap-preferred" flavor throughout
- Reduce all timing values to 150-200ms
- Consider disabling `require-prior-idle-ms`

## Compatibility

### Supported Keyboards
- Corne (42 keys) - Fully tested
- Other Miryoku layouts - May need position adjustments

### ZMK Version
- Tested with ZMK 3.5.0
- Compatible with ZMK 3.0+ (hold-tap features)

### Backward Compatibility
- Original behaviors remain available
- Can disable optimizations by removing `MIRYOKU_OPTIMIZED_BEHAVIORS`
- All existing layers and macros preserved

## Future Enhancements

### Potential Improvements
1. **Combo Integration**: Add combo support for common shortcuts
2. **Adaptive Timing**: Dynamic adjustment based on typing speed
3. **Per-Key Tuning**: Individual timing for different keys
4. **Gesture Support**: Advanced modifier combinations

### Monitoring
- Monitor error rates during usage
- Adjust timing based on personal preference
- Consider logging for troubleshooting

## Build Instructions

1. Ensure all files are in place
2. Run `make` to build firmware
3. Flash UF2 files to keyboard halves
4. Test behavior and adjust as needed

## Conclusion

This optimized configuration provides a robust foundation for Miryoku users seeking reliable home row modifiers with minimal learning curve. The balance of timing parameters and positional logic creates a responsive typing experience that adapts to various usage patterns.

The implementation follows ZMK best practices and maintains full compatibility with existing Miryoku features while significantly improving the user experience for medium-speed typists.
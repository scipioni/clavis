# Clavis ZMK Firmware

This document provides instructions for building the ZMK firmware for the Clavis keyboard.

The keyboard layout is defined in the parent directory (`../`) using the Miryoku Babel format.

## Customizing Parameters

The firmware allows customization of various parameters to suit your typing preferences. To customize these parameters, edit the `config/custom_config.h` file.

### U_TAPPING_TERM

The `U_TAPPING_TERM` parameter controls the time window (in milliseconds) within which a key must be released to register as a tap rather than a hold. This affects all hold-tap behaviors such as:

- Mod-taps (e.g., `&mt` for modifier + key combinations)
- Layer-taps (e.g., `&lt` for switching to a layer when held, sending a key when tapped)
- Other hold-tap behaviors in the layout

The default value is 200ms. A lower value (e.g., 150ms) makes it easier to register taps but requires more precise timing to activate the hold function. A higher value gives more time to decide between tap and hold but can introduce slight delays in hold activation.

Current value: 150ms (overridden in `config/custom_config.h`)

## Building the Firmware

To build the firmware, run the following command:

```bash
make
```

## Output

The build process will generate the following files in the `firmware/` directory:

*   `clavis_left.uf2`: Firmware for the left half of the keyboard.
*   `clavis_right.uf2`: Firmware for the right half of the keyboard.
*   `settings_reset.uf2`: Firmware to reset the keyboard's settings.
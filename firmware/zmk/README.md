# Clavis ZMK Firmware

This document provides instructions for building the ZMK firmware for the Clavis keyboard.

The keyboard layout is defined in the parent directory (`../`) using the Miryoku Babel format.

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
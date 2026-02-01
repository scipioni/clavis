# Clavis ZMK Firmware

This document provides instructions for building the ZMK firmware for the Clavis keyboard.

The keyboard layout is defined in the parent directory (`../`) using the Miryoku Babel format.

## Customizing Parameters

The firmware allows customization of various parameters to suit your typing preferences. To customize these parameters, edit the `config/custom_config.h` file.

### U_TAPPING_TERM

The `U_TAPPING_TERM` (zmk tapping-term-ms) parameter controls the time window (in milliseconds) within which a key must be released to register as a tap rather than a hold. This affects all hold-tap behaviors such as:

- Mod-taps (e.g., `&mt` for modifier + key combinations)
- Layer-taps (e.g., `&lt` for switching to a layer when held, sending a key when tapped)
- Other hold-tap behaviors in the layout

The default value is 200ms. A lower value (e.g., 150ms) makes it easier to register taps but requires more precise timing to activate the hold function. A higher value gives more time to decide between tap and hold but can introduce slight delays in hold activation.

Current value: 150ms (overridden in `config/custom_config.h`)

### CONFIG_ZMK_KEYBOARD_NAME

The `CONFIG_ZMK_KEYBOARD_NAME` parameter sets the Bluetooth name of the device. This is the name that appears when pairing the keyboard via Bluetooth.

To customize it, edit the value in `config/corne.conf`.

Current value: "clavis"

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


## Bluetooth pairing

on keyboard reset bluetooth profile (0,1,2,3), for examble 0:
- press and hold U_MEDIA (thumb on left), then SHIFT and hold (j key) and then u_bt_sel_0 (h key)

on linux
```
bluetootctl

# find clavis mac
scan on
devices
--> Device D4:7E:55:3B:A2:96 clavis

# first trust mac, then pair mac
trust D4:7E:55:3B:A2:96
pair D4:7E:55:3B:A2:96

```

now change profile between PC, laptop, phone with:
- press and hold U_MEDIA (thumb on left) and then u_bt_sel_1 (, key) or u_bt_sel_0 (h key)


## Introspection

create keymap from current corne.keymap
```
uv pip install pcpp
pcpp --passthru-unfound-includes config/corne.keymap > /tmp/pcpp.keymap

```
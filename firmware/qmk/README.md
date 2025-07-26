# Clavis QMK Firmware

This document provides instructions for setting up, customizing, and flashing the QMK firmware for the Clavis keyboard.

The keyboard layout is defined in the [clavis/](clavis/) directory and is based on the Miryoku layout.

## Prerequisites

Before flashing the firmware, ensure your user has the necessary permissions to access the serial port. This can typically be achieved by adding your user to the `uucp` or `dialout` group.

```bash
# Example for adding user to the uucp group
sudo usermod -a -G uucp $USER
```

You may need to log out and log back in for the changes to take effect.

## Setup

To set up the QMK build environment, run the following command:

```bash
make setup
```

## Customization (Optional)

1.  **Modify Layout:** Upload `miryoku-intl.json` to the [QMK Configurator](https://config.qmk.fm) to modify the layout.
2.  **Download JSON:** Download the modified layout as `miryoku-intl.json`.
3.  **Convert to `layout.h`:** Run the following command to convert the JSON file to the `clavis/layout.h` format:
    ```bash
    make compile
    ```

## Compiling and Flashing

1.  **Compile:**
    ```bash
    make compile
    ```
2.  **Flash:**
    ```bash
    make flash
    ```

## Troubleshooting

*   **Master/Slave Detection:** The master and slave halves may not be detected on a cold boot. See [this QMK issue](https://github.com/qmk/qmk_firmware/issues/8990) for more details.

## Advanced `avrdude` Usage

### udev Rule

To ensure consistent device naming, you can create a udev rule. Create the file `/etc/udev/rules.d/55-caterina.rules` with the following content:

```
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0037", TAG+="uaccess", RUN{builtin}+="uaccess", SYMLINK+="caterina%n"
```

### Manual Flashing

The Pro Micro enters programming mode for 7 seconds after pressing the reset button twice.

*   **Erase Chip:**
    ```bash
    sudo avrdude -P /dev/caterina0 -p atmega32u4 -c avr109 -e
    ```
*   **Upload Firmware:**
    ```bash
    avrdude -P /dev/caterina0 -p atmega32u4 -c avr109 -U flash:w:crkbd_rev1_via.hex
    ```
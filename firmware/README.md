# Firmware with Miryoku Layout

This document explains how to generate the Miryoku layout files for various firmware, including ZMK and QMK.

## Generating Miryoku Files

The Miryoku layout is generated from the `miryoku.org` file using Emacs Org Mode.

### Prerequisites

*   **Emacs with Org Mode:** You'll need to have Emacs and Org Mode installed. You can typically install it using your system's package manager:
    ```bash
    # Example for Arch Linux
    yay -S emacs-org-mode
    ```

### 1. Customize the Layout (Optional)

You can edit `miryoku.org` to customize the layout to your preferences.

### 2. Generate Firmware Files

Run the `make` command to generate the firmware files from `miryoku.org`.

```bash
make
```

This will create a `tangled` directory containing the generated files for different firmware types:

```
tangled/
├── kmk/
├── kmonad/
├── qmk/
├── svg/
└── zmk/
```

Each directory will contain the following files:

*   `miryoku_layer_alternatives.h`
*   `miryoku_layer_list.h`
*   `miryoku_layer_selection.h`

### 3. Copy Files to Firmware Directory

After generating the files, copy them to the appropriate firmware directory (e.g., `zmk/miryoku` or `qmk/clavis`).
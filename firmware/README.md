# Firmware with miryoky layout

1. Generate miryoku ZMK/QMK/... files from miryoku.org (see below)
2. enter in zmk folder

## miryoku babel

Install emacs org mode
```
yay -S emacs-org-mode
```

```
make
```

Files generated
```
tangled/
├── kmk
│   ├── miryoku_layer_alternatives.h
│   ├── miryoku_layer_list.h
│   └── miryoku_layer_selection.h
├── kmonad
│   ├── miryoku_layer_alternatives.h
│   ├── miryoku_layer_list.h
│   └── miryoku_layer_selection.h
├── qmk
│   ├── miryoku_layer_alternatives.h
│   ├── miryoku_layer_list.h
│   └── miryoku_layer_selection.h
├── svg
│   ├── miryoku_layer_alternatives.h
│   ├── miryoku_layer_list.h
│   └── miryoku_layer_selection.h
└── zmk
    ├── miryoku_layer_alternatives.h
    ├── miryoku_layer_list.h
    └── miryoku_layer_selection.h
```

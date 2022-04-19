
SPLIT_KEYBOARD = yes
#RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = yes
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
VIA_ENABLE = no
LTO_ENABLE = yes # smaller size

MOUSEKEY_ENABLE = yes # Mouse keys
EXTRAKEY_ENABLE = yes # Audio control and System control
AUTO_SHIFT_ENABLE = yes # Auto Shift

# https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
EXTRAFLAGS += -flto


# If you want to change the display of OLED, you need to change here
        # SRC +=  ./lib/glcdfont.c \
        #         ./lib/rgb_state_reader.c \
        #         ./lib/layer_state_reader.c \
        #         ./lib/logo_reader.c \
        #         ./lib/keylogger.c \
        #         # ./lib/mode_icon_reader.c \
        #         # ./lib/host_led_state_reader.c \
#         # ./lib/timelogger.c \

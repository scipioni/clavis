# clavis qmk

My clavis qmk ![layout](images/) with 36 keys and miryoku directives

## setup

setup qmk

```
make setup
```

## customize (optional)

Upload miryoku-intl.json on https://config.qmk.fm and modify it

Download json on miryoku-intl.json and convert to clavis/layout.h

```
make compile
```

## compile

```
make compile
```

## flash firmware

```
make flash
```

## avrdude advanced

/etc/udev/ruled.d/55-caterina.rules

```
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="0037", TAG+="uaccess", RUN{builtin}+="uaccess", SYMLINK+="caterina%n"
```

device pro micro is visible for programming mode after 'two' reset button pressing and only for 7 seconds

```
# erase
sudo avrdude -P /dev/caterina0  -p atmega32u4 -c avr109 -e

# upload crkbd_rev1_via.hex
avrdude -P /dev/caterina0  -p atmega32u4 -c avr109 -U flash:w:crkbd_rev1_via.hex
```

# Clavis PCB

## Features

- Extra pins and a slider switch for battery for wireless Promicro clones (nice!nano, etc); the slider can be shorted with a jumper if `CONFIG_ZMK_SLEEP` is preferred.
- Wired operation with QMK: TRRS footprints are included and routed. The pinout is identical with a five-column Corne sans the RGBLEDs.
- Reset switches are easily accessible. The footprints used are the same as on the Corne.

## Bill of materials

- [TRRS audio cable](https://www.aliexpress.com/item/1005003613205581.html?spm=a2g0o.order_list.order_list_main.11.57841802uASasS) x 1
- [pin connector for micro](https://www.aliexpress.com/item/4000379224241.html?spm=a2g0o.order_list.order_list_main.17.57841802uASasS) qb
- [Pro Micro ATMEGA32U4](https://www.aliexpress.com/item/32888212119.html?spm=a2g0o.order_list.order_list_main.23.57841802uASasS) x 2
- [diode 1N4148 SOD-123]() x 36
- [TRRS jack](https://www.aliexpress.com/item/33029465106.html?spm=a2g0o.order_list.order_list_main.35.57841802uASasS) x 2
- [Kailh hot swapping pcb sockets for choc kailh low profile](https://www.aliexpress.com/item/32901654130.html?spm=a2g0o.order_list.order_list_main.41.57841802uASasS) x 36
- [kailh low profile choc switch](https://www.aliexpress.com/item/32838369089.html?spm=a2g0o.order_list.order_list_main.58.57841802uASasS) x 36
- [micro switch push button ](https://www.aliexpress.com/item/1068908059.html?spm=a2g0o.order_list.order_list_main.209.57841802uASasSs) x 2
- [MBK Choc Low Profile Keycaps 1U](https://keycapsss.com/keyboard-parts/keycaps/169/mbk-choc-low-profile-blank-keycaps?c=16) x 34
- [MBK Choc Low Profile Keycaps 1U homing](https://keycapsss.com/keyboard-parts/keycaps/169/mbk-choc-low-profile-blank-keycaps?number=KC10130_1U-H-WH&c=16) x 2

## How to

- prereq:
  - nodejs
  - java >= 11
- run `make setup clean`
- run `make unrouted` to make unrouted pcb ready to be checked in kicad
- go to step4 below

## Layout

Edit clavis.yaml and check on

- https://ergogen.cache.works/

## TODO

- import Specctra session with scripts/import_ses.py
- fix pcbdraw of images/board-front.png
-

## Introspection

Input file of entire process is clavis.yaml in ergogen format.

### step 1: ergogen - from yaml -> to kicad pcb

```
npm run gen
```

output: kicad pcb

```
output/pcbs/
├── board.kicad_pcb
├── bottom_plate.kicad_pcb
└── top_plate.kicad_pcb
output/outlines/*
```

open kicad pcb

```
pcbnew output/pcbs/board.kicad_pcb
```

![kicad](images/kicad.png)

### step 2: kicad - from kicad pcb -> to specctra DSN

with kicad gui:

- Export the PCB into Specctra DSN (File / Export... / Specctra DSN).

or with cli:

```
make output/pcbs/board.dsn
```

### step 3: autoroute with freerouting - from DSN -> to ses file

```
make output/routed_pcbs/board.ses
```

click with left mouse if loop

### step 4: kicad - merge routed session

with kicad gui:

- open unrouted pcb: pcbnew output/pcbs/board.kicad_pcb
- import output/routed_pcbs/board.ses file: Import -> Specctra session
- check DRC: Inspect -> DRC
- ...
- save output/routed_pcbs/board.kicad_pcb

or with cli: TODO

## step 5: kikit - create gerbers

create gerbers in output/gerbers

```
make gerbers
```

## Convert output dxf to svg

```
https://convertio.co/
```

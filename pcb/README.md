# clavis PCB

## Features

- Extra pins and a slider switch for battery for wireless Promicro clones (tested with nice!nano); the slider can be
  shorted with a jumper if `CONFIG_ZMK_SLEEP` is preferred.
- Wired operation with QMK should be possible but is not tested; TRRS footprints are included and routed. The pinout is
  identical with a five-column Corne sans the RGBLEDs.
- Reset switches are easily accessible. The footprints used are the same as on the Corne.

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

- import Specctre session with scripts/import_ses.py
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
- import ses file: Import -> Specctre session
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
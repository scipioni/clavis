# clavis
##  Split 36 key keyboard

left | right
-|-
![left](images/board-front.png) | ![right](images/board-back.png)

A 5x3 split keyboard with 3 thumbs. It's inspired by Corne and
has the exact number of keys needed for the [Miryoku](https://github.com/manna-harbour/miryoku) layout.

* Layout is declared using [Ergogen](https://github.com/mrzealot/ergogen/)
* The build system uses Ergogen to translate YAML to a KiCad PCB and plate files for FR-4 fab or laser cutting
* uses [FreeRouting](https://github.com/freerouting/freerouting) to **automatically route the traces on the PCB**
* uses pcbdraw to render PCB previews
* uses [KiKit](https://github.com/yaqwsx/KiKit) to render production-ready **Gerber files**

## Features

* Extra pins and a slider switch for battery for wireless Promicro clones (tested with nice!nano); the slider can be
  shorted with a jumper if `CONFIG_ZMK_SLEEP` is preferred.
* Wired operation with QMK should be possible but is not tested; TRRS footprints are included and routed. The pinout is
  identical with a five-column Corne sans the RGBLEDs.
* Reset switches are easily accessible. The footprints used are the same as on the Corne.

## How to

  * make sure to have Docker CLI and NodeJS installed
  * run `make setup clean all`
  * check the `output` folder for KiCad PCBs and Gerbers
  * optional: create png render with `make png` and check output/routed_pcbs/board-front.png

See the [workflow](.github/workflows/build.yml) or the [Makefile](Makefile) for more details.


## Layout

Edit clavis.yaml and check on

* https://ergogen.cache.works/

<!-- Upload samoklava.json except pcbs section. I print on paper with dxf format on qcad and:
* edit sheet properties -> printer offset -60,-60   -->


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
* Export the PCB into Specctra DSN (File / Export... / Specctra DSN).

or with cli:
```
make output/pcbs/board.dsn
```

### step 3: autoroute with freerouting - from DSN -> to ses file

```
make output/routed_pcbs/board.ses
```

click with left mouse if loop

### step 4: kicad: merge routed session

with kicad gui:
* open unrouted pcb: pcbnew output/pcbs/board.kicad_pcb
* import ses file: Import -> Specctre session
* check DRC: Inspect -> DRC
* ... 
* save output/routed_pcbs/board.kicad_pcb

or with cli: TODO

## Convert output dxf to svg

```
https://convertio.co/
```

FREEROUTING := 2.0.1

.PHONY: clean gerbers setup show unrouted images release kicad
.DEFAULT: gerber

setup: bin/freerouting-$(FREEROUTING).jar
	npm install

unrouted: output/routed_pcbs/board.ses

output/pcbs/board.kicad_pcb output/pcbs/top_plate.kicad_pcb output/pcbs/bottom_plate.kicad_pcb &: config.yaml
	npm run gen
	zip output/kicad.zip output/pcbs/*

kicad: output/kicad.zip config.yaml

output/kicad.zip: output/pcbs/board.kicad_pcb output/pcbs/top_plate.kicad_pcb output/pcbs/bottom_plate.kicad_pcb

output/pcbs/board.dsn: output/pcbs/board.kicad_pcb
	# file can not be present or the script will refuse to run
	if [ -f "$@" ] ; then rm $@ ; fi
	python scripts/export_dsn.py $< $@

output/routed_pcbs/board.ses: output/pcbs/board.dsn
	@mkdir -p $(shell dirname $@)
	java -jar bin/freerouting-$(FREEROUTING).jar -de $< -do $@ -ap 50 -fo -pp 50 -dr clavis.rules

output/routed_pcbs/%_plate.kicad_pcb: output/pcbs/%_plate.kicad_pcb
	@mkdir -p $(shell dirname $@)
	@cp $< $@
	
output/routed_pcbs/%-front.png: output/routed_pcbs/%.kicad_pcb
	@mkdir -p $(shell dirname $@)
	pcbdraw --style builtin:oshpark-afterdark.json $< $@

output/routed_pcbs/%-back.png: output/routed_pcbs/%.kicad_pcb
	@mkdir -p $(shell dirname $@)
	pcbdraw -b --style builtin:oshpark-afterdark.json $< $@

output/gerbers/%/gerbers.zip: output/routed_pcbs/%.kicad_pcb
	@mkdir -p $(shell dirname $@)
	kikit fab jlcpcb --no-assembly --no-drc $< $(shell dirname $@)

clean:
	sudo rm -rf output

images: \
	output/routed_pcbs/board-front.png \
	output/routed_pcbs/board-back.png \
	images/board-back.png \
	images/board-front.png

images/board-back.png: output/pcbs/board.kicad_pcb
	pcbdraw -b --style builtin:oshpark-afterdark.json output/pcbs/board.kicad_pcb $@

images/board-front.png: output/pcbs/board.kicad_pcb
	pcbdraw --style builtin:oshpark-afterdark.json output/pcbs/top_plate.kicad_pcb $@

images/qmk.png:
	convert /tmp/qmk.png -transparent white images/qmk.png

show: output/routed_pcbs/board-front.png output/routed_pcbs/board-back.png output/routed_pcbs/top_plate-front.png output/routed_pcbs/bottom_plate-front.png
	eog $<

bin/freerouting-$(FREEROUTING).jar:
	mkdir -p bin && cd bin && wget https://github.com/freerouting/freerouting/releases/download/v$(FREEROUTING)/freerouting-$(FREEROUTING).jar 

gerbers-board: output/gerbers/board/gerbers.zip

gerbers: \
	output/gerbers/top_plate/gerbers.zip \
	output/gerbers/bottom_plate/gerbers.zip \

release:
	zip -qr /tmp/clavis-1-2.zip output


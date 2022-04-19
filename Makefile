# .DELETE_ON_ERROR:
.SECONDARY:
.PHONY: clean gerbers setup png show unrouted
.DEFAULT: gerber

# container_cmd ?= docker
# container_args ?= -w /board -v $(shell pwd):/board --rm

setup: bin/freerouting-1.5.0.jar
	npm install


unrouted: output/routed_pcbs/board.ses

# outputs from 
output/pcbs/board.kicad_pcb output/pcbs/top_plate.kicad_pcb output/pcbs/bottom_plate.kicad_pcb &: clavis.yaml
	npm run gen

output/pcbs/board.dsn: output/pcbs/board.kicad_pcb
	# file can not be present or the script will refuse to run
	if [ -f "$@" ] ; then rm $@ ; fi
	python scripts/export_dsn.py $< $@

output/routed_pcbs/board.ses: output/pcbs/board.dsn
	@mkdir -p $(shell dirname $@)
	java -jar bin/freerouting-1.5.0.jar -de $< -do $@ -ap 50 -fo -pp 50 -dr clavis.rules

output/routed_pcbs/%_plate.kicad_pcb: output/pcbs/%_plate.kicad_pcb
	@mkdir -p $(shell dirname $@)
	@cp $< $@
	
# output/routed_pcbs/%.kicad_pcb: output/routed_pcbs/%.ses output/pcbs/%.kicad_pcb
# 	mkdir -p $(shell dirname $@)
# 	# file can not be present or the script will refuse to run
# 	if [ -f "$@" ] ; then rm $@ ; fi
# 	${container_cmd} run ${container_args} soundmonster/kicad-automation-scripts:latest /usr/lib/python2.7/dist-packages/kicad-automation/pcbnew_automation/import_ses.py output/pcbs/$*.kicad_pcb $< --output-file $@

# output/routed_pcbs/%-drc/: output/routed_pcbs/%.kicad_pcb
# 	mkdir -p $@
# 	${container_cmd} run ${container_args} soundmonster/kicad-automation-scripts:latest /usr/lib/python2.7/dist-packages/kicad-automation/pcbnew_automation/run_drc.py  $< $@

output/routed_pcbs/%-front.png: output/routed_pcbs/%.kicad_pcb
	@mkdir -p $(shell dirname $@)
	pcbdraw --style builtin:oshpark-afterdark.json $< $@

output/routed_pcbs/%-back.png: output/routed_pcbs/%.kicad_pcb
	@mkdir -p $(shell dirname $@)
	pcbdraw -b --style builtin:oshpark-afterdark.json $< $@

# output/pcbs/top_plate.png: output/pcbs/top_plate.kicad_pcb
# 	mkdir -p $(shell dirname $@)
# 	${container_cmd} run ${container_args} yaqwsx/kikit:v1.0.2 pcbdraw --style builtin:oshpark-afterdark.json $< $@


output/gerbers/%/gerbers.zip: output/routed_pcbs/%.kicad_pcb
	@mkdir -p $(shell dirname $@)
	kikit fab jlcpcb --no-assembly --no-drc $< $(shell dirname $@)

clean:
	sudo rm -rf output

png: \
	output/routed_pcbs/board-front.png \
	output/routed_pcbs/board-back.png

images/board-back.png: output/pcbs/board.kicad_pcb
	pcbdraw -b --style builtin:oshpark-afterdark.json output/pcbs/board.kicad_pcb $@

images/board-front.png: output/pcbs/board.kicad_pcb
	pcbdraw --style builtin:oshpark-afterdark.json output/pcbs/top_plate.kicad_pcb $@

show: output/routed_pcbs/board-front.png output/routed_pcbs/board-back.png output/routed_pcbs/top_plate-front.png output/routed_pcbs/bottom_plate-front.png
	eog $<

bin/freerouting-1.5.0.jar:
	mkdir -p bin && cd bin && wget https://github.com/freerouting/freerouting/releases/download/v1.5.0/freerouting-1.5.0.jar 

gerbers: \
	output/gerbers/top_plate/gerbers.zip \
	output/gerbers/bottom_plate/gerbers.zip \
	output/gerbers/board/gerbers.zip

.venv:
	python -m venv --system-site-packages .venv
	source .venv/bin/activate; pip install pcbdraw


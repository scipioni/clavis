VERSION := 0.19.12

.PHONY: compile setup flash
#.DEFAULT: all

setup: qmk_firmware qmk_firmware/keyboards/crkbd/keymaps/clavis 

compile: clavis/*
	make -C qmk_firmware/ crkbd:clavis
	
flash:
	make -C qmk_firmware/ crkbd:clavis:avrdude

qmk_firmware:
	git clone -b ${VERSION} https://github.com/qmk/qmk_firmware.git && \
	cd qmk_firmware && qmk setup --yes

qmk_firmware/keyboards/crkbd/keymaps/clavis:
	ln -s ../../../../clavis qmk_firmware/keyboards/crkbd/keymaps/clavis

clavis/layout.h: miryoku-intl.json
	cd qmk_firmware && qmk json2c ../miryoku-intl.json -o ../clavis/layout.h

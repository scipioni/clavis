VERSION := 0.23.3

.PHONY: compile setup flash
#.DEFAULT: all

setup: qmk_firmware qmk_firmware/keyboards/crkbd/keymaps/clavis 

compile: clavis/*
	#make -C qmk_firmware/ crkbd:clavis
	cd qmk_firmware && qmk compile -kb crkbd -km clavis
	
flash:
	#make -C qmk_firmware/ crkbd:clavis:avrdude
	cd qmk_firmware && qmk flash -kb crkbd -km clavis

qmk_firmware:
	git clone -b ${VERSION} https://github.com/qmk/qmk_firmware.git && \
	cd qmk_firmware && qmk setup --yes

qmk_firmware/keyboards/crkbd/keymaps/clavis:
	ln -s ../../../../clavis qmk_firmware/keyboards/crkbd/keymaps/clavis

clavis/layout.h: miryoku-intl.json
	cd qmk_firmware && qmk json2c ../miryoku-intl.json -o ../clavis/layout.h

.PHONY: cli clean update

all: zmk-clavis/zephyr/VERSION firmware/clavis_left.uf2

zmk-clavis/zephyr/VERSION:
	make update

update:
	docker compose run --rm --workdir=/zmk zmk west update

cli:
	docker compose run --rm --workdir=/zmk zmk bash

firmware/clavis_left.uf2: zmk-clavis/config/corne.h zmk-clavis/config/corne.conf
	docker compose run --rm --workdir=/zmk zmk bash -c ./build.sh

clean:
	rm -f firmware/*

raw-keymap:
	# pip install pcpp
	pcpp --passthru-unfound-includes zmk-clavis/config/corne.keymap

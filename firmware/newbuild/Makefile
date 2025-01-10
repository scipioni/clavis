.PHONY: all clean_firmware clean_image clean

all:
	docker build --tag zmk_keiler --file Dockerfile .
	docker run --rm -it --name zmk_keiler \
		-v $(PWD)/firmware:/app/firmware \
		-v $(PWD)/config:/app/config:ro \
		zmk_keiler

clean_firmware:
	rm -f firmware/*.uf2

clean_image:
	docker image rm zmk_keiler docker.io/zmkfirmware/zmk-build-arm:stable

clean:
	clean_firmware
	clean_image

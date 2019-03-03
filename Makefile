.PHONY: twi_test flash_test

all: twi_test flash_test

twi_test:
	make -C twi_test

flash_test:
	make -C flash_test

clean:
	make clean -C twi_test
	make clean -C flash_test

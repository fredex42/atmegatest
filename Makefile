.PHONY: twi_test

all: twi_test

twi_test:
	make -C twi_test

clean:
	make clean -C twi_test

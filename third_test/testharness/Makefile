CFLAGS = `pkg-config simavr simavrparts simavr-avr --cflags`
LIBS = `pkg-config simavr simavrparts --libs`

all: harness

harness.o: harness.c
	gcc -g $(CFLAGS) -c harness.c

harness: harness.o
	gcc -o harness harness.o $(LIBS)

clean:
	rm -f *.o harness

distclean: clean
	rm -f *.vcd

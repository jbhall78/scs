JOBS=16

all:
	make -j$(JOBS) -f Makefile.lib
	make -j$(JOBS) -f Makefile.client

clean:
	make -f Makefile.lib clean
	make -f Makefile.client clean

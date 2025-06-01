all:
	make -C src all
clean:
	make -C src clean

install:
	mkdir -p "$(DESTDIR)/app/bin"
	cp src/scs $(DESTDIR)/app/bin/scs
	mkdir -p "$(DESTDIR)/app/share/scs"
	(cd data && tar -cf - .) | (cd "$(DESTDIR)/app/share/scs" && tar -xf -)

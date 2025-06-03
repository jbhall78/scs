all:
	make -C src all
clean:
	make -C src clean

install:
	mkdir -p "$(DESTDIR)/bin"
	cp src/scs $(DESTDIR)/bin/scs
	mkdir -p "$(DESTDIR)/share/scs"
	(cd data && tar -cf - .) | (cd "$(DESTDIR)/share/scs" && tar -xf -)

flatpak:
	rm -rf build
	flatpak-builder --disable-cache build net.pixelops.scs.yaml build
	flatpak build-export /tmp/local-build-repo build
	flatpak build-bundle /tmp/local-build-repo net.pixelops.scs.flatpak net.pixelops.scs

ifdef DESTDIR
# DESTDIR is already defined, do nothing
else
DESTDIR = /usr
endif

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

flatpak-repo:
	mkdir -p $(HOME)/scs-flatpak-repo
	rm -rf $(HOME)/scs-flatpak-build
	mkdir -p $(HOME)/scs-flatpak-build
	flatpak-builder \
		--repo=$(HOME)/scs-flatpak-repo \
		--gpg-sign=jason@pixelops.net \
		$(HOME)/scs-flatpak-build \
		net.pixelops.scs.yaml
	cp pixelops.flatpakrepo $(HOME)/scs-flatpak-repo
	aws s3 sync $(HOME)/scs-flatpak-repo/ s3://$(S3_FLATPAK_BUCKET)/ --delete
	aws cloudfront create-invalidation --distribution-id $(CLOUDFRONT_DISTRIBUTION_ID) --paths "/summary*"

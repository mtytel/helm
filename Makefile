PROGRAM = twytch
BIN     = $(DESTDIR)/usr/bin
ICONS   = $(DESTDIR)/usr/share/$(PROGRAM)/icons
PATCHES = $(DESTDIR)/usr/share/$(PROGRAM)/patches

all:
	cd standalone/builds/linux && $(MAKE) CONFIG=$(CONFIG)

install: all
	install -d $(PATCHES) $(BIN) $(ICONS)
	install standalone/builds/linux/build/$(PROGRAM) $(BIN)
	install -m644 images/* $(ICONS)
	cp -rf patches/* $(PATCHES)

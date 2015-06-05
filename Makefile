
ifndef CONFIG
  CONFIG=Release
endif

PROGRAM = helm
BIN     = $(DESTDIR)/usr/bin
ICONS   = $(DESTDIR)/usr/share/$(PROGRAM)/icons
PATCHES = $(DESTDIR)/usr/share/$(PROGRAM)/patches

all: standalone lv2 vst

standalone:
	$(MAKE) -C standalone/builds/linux CONFIG=$(CONFIG)

lv2:
	$(MAKE) -C builds/linux/LV2 CONFIG=$(CONFIG)

vst:
	$(MAKE) -C builds/linux/VST CONFIG=$(CONFIG)

clean:
	$(MAKE) clean -C standalone/builds/linux CONFIG=$(CONFIG)
	$(MAKE) clean -C builds/linux/LV2 CONFIG=$(CONFIG)
	$(MAKE) clean -C builds/linux/VST CONFIG=$(CONFIG)

install: all
	install -d $(PATCHES) $(BIN) $(ICONS)
	install standalone/builds/linux/build/$(PROGRAM) $(BIN)
	install -m644 images/* $(ICONS)
	cp -rf patches/* $(PATCHES)

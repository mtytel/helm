ifndef CONFIG
  CONFIG=Release
endif

PROGRAM = helm
BIN     = $(DESTDIR)/usr/bin
LV2     = $(DESTDIR)/usr/lib/lv2/$(PROGRAM).lv2
VST     = $(DESTDIR)/usr/lib/lxvst
ICONS   = $(DESTDIR)/usr/share/$(PROGRAM)/icons
PATCHES = $(DESTDIR)/usr/share/$(PROGRAM)/patches

all: standalone_build lv2_build

standalone_build:
	$(MAKE) -C standalone/builds/linux CONFIG=$(CONFIG)

lv2_build:
	$(MAKE) -C builds/linux/LV2 CONFIG=$(CONFIG)

vst_build:
	$(MAKE) -C builds/linux/VST CONFIG=$(CONFIG)

clean:
	$(MAKE) clean -C standalone/builds/linux CONFIG=$(CONFIG)
	$(MAKE) clean -C builds/linux/LV2 CONFIG=$(CONFIG)
	$(MAKE) clean -C builds/linux/VST CONFIG=$(CONFIG)

install: all
	install -d $(PATCHES) $(BIN) $(ICONS)
	install standalone/builds/linux/build/$(PROGRAM) $(BIN)
	install -m644 images/* $(ICONS)
	install builds/linux/LV2/helm.lv2/* $(LV2)
	cp -rf patches/* $(PATCHES)

ifndef CONFIG
	CONFIG=Release
endif

ifndef LIBDIR
	LIBDIR=/usr/lib/
endif

PROGRAM = helm
BIN     = $(DESTDIR)/usr/bin
BINFILE = $(BIN)/$(PROGRAM)
LV2     = $(DESTDIR)/$(LIBDIR)/lv2/$(PROGRAM).lv2
VST     = $(DESTDIR)/$(LIBDIR)/lxvst/$(PROGRAM).so
SYSDATA = $(DESTDIR)/usr/share/$(PROGRAM)
ICONS   = $(SYSDATA)/icons
PATCHES = $(SYSDATA)/patches

all: standalone lv2

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
	install -d $(PATCHES) $(BIN) $(ICONS) $(LV2)
	install standalone/builds/linux/build/$(PROGRAM) $(BIN)
	install -m644 images/* $(ICONS)
	install -m644 builds/linux/LV2/helm.lv2/* $(LV2)
	cp -rf patches/* $(PATCHES)

install_vst: vst
	install builds/linux/VST/build/helm.so $(VST)

uninstall:
	rm -rf $(LV2)
	rm -rf $(SYSDATA)
	rm -rf $(BINFILE)

.PHONY: standalone

export DEB_BUILD_MAINT_OPTIONS = hardening=+all

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
MAN     = $(DESTDIR)/usr/share/man/man1/
CHANGES = $(DESTDIR)/usr/share/doc/$(PROGRAM)/

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
	install -d $(PATCHES) $(BIN) $(ICONS) $(LV2) $(MAN) $(CHANGES)
	install standalone/builds/linux/build/$(PROGRAM) $(BIN)
	install -m644 images/* $(ICONS)
	install -m644 builds/linux/LV2/helm.lv2/* $(LV2)
	gzip -n -9 ChangeLog
	mv ChangeLog.gz $(CHANGES)/changelog.gz
	cp docs/helm.1.gz $(MAN)
	cp -rf patches/* $(PATCHES)

install_vst: vst
	install builds/linux/VST/build/helm.so $(VST)

dist:
	rm -rf $(PROGRAM)
	mkdir $(PROGRAM)
	-cp -rf * $(PROGRAM)/
	$(MAKE) clean -C $(PROGRAM) CONFIG=Release
	$(MAKE) clean -C $(PROGRAM) CONFIG=Debug
	rm -rf $(PROGRAM)/.git
	rm -rf $(PROGRAM)/JUCE/extras
	rm -rf $(PROGRAM)/JUCE/examples
	rm -rf $(PROGRAM)/builds/osx
	rm -rf $(PROGRAM)/builds/vs12
	rm -rf $(PROGRAM)/builds/vs15
	rm -rf $(PROGRAM)/standalone/builds/osx
	rm -rf $(PROGRAM)/standalone/builds/vs12
	rm -rf $(PROGRAM)/standalone/builds/vs15
	tar -cvzf $(PROGRAM).tar.gz $(PROGRAM)
	rm -rf $(PROGRAM)

uninstall:
	rm -rf $(LV2)
	rm -rf $(SYSDATA)
	rm -rf $(BINFILE)

.PHONY: standalone

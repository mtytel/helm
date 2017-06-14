ifndef CONFIG
	CONFIG=Release
endif

ifndef LIBDIR
	LIBDIR=/usr/lib/
endif

DPKG := $(shell dpkg-buildflags --version 2> /dev/null)

ifdef DPKG
	DEB_BUILD_MAINT_OPTIONS = hardening=+all
	SDEBCXXFLAGS := $(shell dpkg-buildflags --get CXXFLAGS)
	SDEBLDFLAGS := $(shell dpkg-buildflags --get LDFLAGS)

	DEB_BUILD_MAINT_OPTIONS=hardening=+bindnow
	PDEBCXXFLAGS := $(shell dpkg-buildflags --get CXXFLAGS)
	PDEBLDFLAGS := $(shell dpkg-buildflags --get LDFLAGS)
endif

PROGRAM = helm
BIN     = $(DESTDIR)/usr/bin
BINFILE = $(BIN)/$(PROGRAM)
LV2     = $(DESTDIR)/$(LIBDIR)/lv2/$(PROGRAM).lv2
VSTDIR  = $(DESTDIR)/$(LIBDIR)/lxvst
VST     = $(VSTDIR)/$(PROGRAM).so
SYSDATA = $(DESTDIR)/usr/share/$(PROGRAM)
ICONS   = $(SYSDATA)/icons
PATCHES = $(SYSDATA)/patches
MAN     = $(DESTDIR)/usr/share/man/man1/
CHANGES = $(DESTDIR)/usr/share/doc/$(PROGRAM)/

all: standalone lv2 vst

standalone:
	$(MAKE) -C standalone/builds/linux CONFIG=$(CONFIG) DEBCXXFLAGS="$(SDEBCXXFLAGS)" DEBLDFLAGS="$(SDEBLDFLAGS)"

lv2:
	$(MAKE) -C builds/linux/LV2 CONFIG=$(CONFIG) DEBCXXFLAGS="$(PDEBCXXFLAGS)" DEBLDFLAGS="$(PDEBLDFLAGS)"

vst:
	$(MAKE) -C builds/linux/VST CONFIG=$(CONFIG) DEBCXXFLAGS="$(PDEBCXXFLAGS)" DEBLDFLAGS="$(PDEBLDFLAGS)"

clean:
	$(MAKE) clean -C standalone/builds/linux CONFIG=$(CONFIG)
	$(MAKE) clean -C builds/linux/LV2 CONFIG=$(CONFIG)
	$(MAKE) clean -C builds/linux/VST CONFIG=$(CONFIG)

install_patches:
	install -d $(PATCHES)
	cp -rf patches/* $(PATCHES)

install_standalone: standalone install_patches
	install -d $(BIN) $(ICONS) $(MAN) $(CHANGES)
	install standalone/builds/linux/build/$(PROGRAM) $(BIN)
	install -m644 images/* $(ICONS)
	cp ChangeLog changes
	gzip -n -9 ChangeLog
	mv changes ChangeLog
	mv ChangeLog.gz $(CHANGES)/changelog.gz
	cp docs/helm.1.gz $(MAN)

install_lv2: lv2 install_patches
	install -d $(PATCHES) $(LV2)
	install -m644 builds/linux/LV2/helm.lv2/* $(LV2)
	cp -rf patches/* $(PATCHES)

install_vst: vst install_patches
	install -d $(PATCHES) $(VSTDIR)
	install builds/linux/VST/build/helm.so $(VST)
	cp -rf patches/* $(PATCHES)

install: install_standalone install_vst install_lv2

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
	rm -rf $(VST)
	rm -rf $(SYSDATA)
	rm -rf $(BINFILE)

.PHONY: standalone

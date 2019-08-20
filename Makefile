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

MACHINE := $(shell sh -c 'uname -m 2> /dev/null || echo not')
ifneq (,$(findstring aarch,$(MACHINE)))
	SIMDFLAGS := -march=armv8-a -mtune=cortex-a53
else
ifneq (,$(findstring arm,$(MACHINE)))
	SIMDFLAGS := -march=armv8-a -mtune=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard
else
	SIMDFLAGS := -msse2
endif
endif

PROGRAM = helm
BIN     = $(DESTDIR)/usr/bin
BINFILE = $(BIN)/$(PROGRAM)
LV2     = $(DESTDIR)/$(LIBDIR)/lv2/$(PROGRAM).lv2
VSTDIR  = $(DESTDIR)/$(LIBDIR)/lxvst
VST     = $(VSTDIR)/$(PROGRAM).so
SYSDATA = $(DESTDIR)/usr/share/$(PROGRAM)
IMAGES  = $(SYSDATA)/icons
PATCHES = $(SYSDATA)/patches
MAN     = $(DESTDIR)/usr/share/man/man1/
CHANGES = $(DESTDIR)/usr/share/doc/$(PROGRAM)/
DESKTOP = $(DESTDIR)/usr/share/applications/

ICONS   = $(DESTDIR)/usr/share/icons/hicolor/
ICON16  = images/helm_icon_16_1x.png
ICON22  = images/helm_icon_22_1x.png
ICON24  = images/helm_icon_24_1x.png
ICON32  = images/helm_icon_32_1x.png
ICON48  = images/helm_icon_48_1x.png
ICON64  = images/helm_icon_32_2x.png
ICON128 = images/helm_icon_128_1x.png
ICON256 = images/helm_icon_256_1x.png

ICONDEST16 = $(ICONS)/16x16/apps
ICONDEST22 = $(ICONS)/22x22/apps
ICONDEST24 = $(ICONS)/24x24/apps
ICONDEST32 = $(ICONS)/32x32/apps
ICONDEST48 = $(ICONS)/48x48/apps
ICONDEST64 = $(ICONS)/64x64/apps
ICONDEST128 = $(ICONS)/128x128/apps
ICONDEST256 = $(ICONS)/256x256/apps

all: standalone lv2 vst

install_icons:
	install -d $(ICONDEST16) $(ICONDEST22) $(ICONDEST24) $(ICONDEST32)
	install -d $(ICONDEST48) $(ICONDEST64) $(ICONDEST128) $(ICONDEST256)
	cp $(ICON16) $(ICONDEST16)/$(PROGRAM).png
	cp $(ICON22) $(ICONDEST22)/$(PROGRAM).png
	cp $(ICON24) $(ICONDEST24)/$(PROGRAM).png
	cp $(ICON32) $(ICONDEST32)/$(PROGRAM).png
	cp $(ICON48) $(ICONDEST48)/$(PROGRAM).png
	cp $(ICON64) $(ICONDEST64)/$(PROGRAM).png
	cp $(ICON128) $(ICONDEST128)/$(PROGRAM).png
	cp $(ICON256) $(ICONDEST256)/$(PROGRAM).png

standalone:
	$(MAKE) -C standalone/builds/linux CONFIG=$(CONFIG) DEBCXXFLAGS="$(SDEBCXXFLAGS)" DEBLDFLAGS="$(SDEBLDFLAGS)" SIMDFLAGS="$(SIMDFLAGS)"

lv2:
	$(MAKE) -C builds/linux/LV2 CONFIG=$(CONFIG) DEBCXXFLAGS="$(PDEBCXXFLAGS)" DEBLDFLAGS="$(PDEBLDFLAGS)" SIMDFLAGS="$(SIMDFLAGS)"

vst:
	$(MAKE) -C builds/linux/VST CONFIG=$(CONFIG) DEBCXXFLAGS="$(PDEBCXXFLAGS)" DEBLDFLAGS="$(PDEBLDFLAGS)" SIMDFLAGS="$(SIMDFLAGS)"

clean:
	$(MAKE) clean -C standalone/builds/linux CONFIG=$(CONFIG)
	$(MAKE) clean -C builds/linux/LV2 CONFIG=$(CONFIG)
	$(MAKE) clean -C builds/linux/VST CONFIG=$(CONFIG)

install_patches:
	rm -rf $(PATCHES)
	install -d $(PATCHES)
	cp -rf patches/* $(PATCHES)

install_standalone: standalone install_patches install_icons
	install -d $(BIN) $(IMAGES) $(MAN) $(CHANGES) $(DESKTOP)
	install standalone/builds/linux/build/$(PROGRAM) $(BIN)
	install -m644 standalone/helm.desktop $(DESKTOP)/helm.desktop
	install -m644 images/* $(IMAGES)
	cp changelog changes
	gzip -n -9 changelog
	mv changes changelog
	mv changelog.gz $(CHANGES)/changelog.gz
	cp docs/helm.1.gz $(MAN)

install_lv2: lv2 install_patches
	install -d $(PATCHES) $(LV2)
	install -m644 builds/linux/LV2/helm.lv2/* $(LV2)
	cp -rf patches/* $(PATCHES)

install_vst: vst install_patches
	install -d $(PATCHES) $(VSTDIR)
	install builds/linux/VST/build/helm.so $(VST)
	cp -rf patches/* $(PATCHES)

thank_you:
ifdef PAID
	install -d $(PATCHES)
	cp docs/thank_you.txt $(PATCHES)
endif

install: install_standalone install_vst install_lv2 thank_you

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
	rm $(ICONDEST16)/$(PROGRAM).png
	rm $(ICONDEST22)/$(PROGRAM).png
	rm $(ICONDEST24)/$(PROGRAM).png
	rm $(ICONDEST32)/$(PROGRAM).png
	rm $(ICONDEST48)/$(PROGRAM).png
	rm $(ICONDEST64)/$(PROGRAM).png
	rm $(ICONDEST128)/$(PROGRAM).png
	rm $(ICONDEST256)/$(PROGRAM).png

.PHONY: standalone

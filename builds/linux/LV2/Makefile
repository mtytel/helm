ifndef CONFIG
  CONFIG=Release
endif

all: binary ttl_generator lv2

binary:
	$(MAKE) -f Makefile.binary CONFIG=$(CONFIG)

ttl_generator:
	$(MAKE) -f Makefile.ttl_generator CONFIG=$(CONFIG)

lv2: ttl_generator binary
	lv2_ttl_generator build/helm.so
	mkdir -p helm.lv2
	cp build/helm.so helm.lv2
	mv *.ttl helm.lv2

clean:
	$(MAKE) clean -f Makefile.binary
	$(MAKE) clean -f Makefile.ttl_generator
	rm -rf helm.lv2

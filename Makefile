.PHONY:all clean install
export GIMPTOOL:= gimptool-2.0
PLUGINS=hatching1 hello 
all clean install:
	$(foreach T,${PLUGINS},$(MAKE) -C src/${T} $@ ;)

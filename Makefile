.PHONY:all clean install
export GIMPTOOL:= gimptool-2.0
PLUGINS=gribouillis  
all clean install:
	$(foreach T,${PLUGINS},$(MAKE) -C src/${T} $@ ;)

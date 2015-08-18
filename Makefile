.PHONY:all clean install
export GIMPTOOL:= gimptool-2.0
all clean install:
	$(MAKE) -C src/hello $@

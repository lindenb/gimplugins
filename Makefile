.PHONY:all clean
export GIMPTOOL:= gimptool-2.0
all:
	$(MAKE) -C src/hello all
clean:
	$(MAKE) -C src/hello clean

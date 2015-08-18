.PHONY:all clean install

all:hello

install: hello
	$(GIMPTOOL) --install-bin $<

hello: hello.c
	$(GIMPTOOL) --build $<

clean:
	rm -f hello 

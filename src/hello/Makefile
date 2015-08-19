.PHONY:all clean install
SRCNAME=hello
all:${SRCNAME}

install: ${SRCNAME}
	$(GIMPTOOL) --install-bin $<

${SRCNAME}: $(addsuffix .c,${SRCNAME})
	$(GIMPTOOL) --build $<

clean:
	rm -f ${SRCNAME} 

.PHONY:all clean install
SRCNAME=hellocpp
CFLAGS=-I..
all:${SRCNAME}

install: ${SRCNAME}
	$(GIMPTOOL) --install-bin $<


${SRCNAME} : $(addsuffix .o,${SRCNAME})
	g++ -o $@   $< `$(GIMPTOOL) --libs` 


$(addsuffix .o,${SRCNAME}) : $(addsuffix .cpp,${SRCNAME})
	g++ -c -Wall -I.. `$(GIMPTOOL) --cflags` $< 

clean:
	rm -f ${SRCNAME} *.o

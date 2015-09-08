.PHONY:all clean install
GIMPTOOL?=gimptool-2.0
SRCNAME=gribouillis
CFLAGS=-I..
all:${SRCNAME}

install: ${SRCNAME}
	$(GIMPTOOL) --install-bin $<


${SRCNAME} : $(addsuffix .o,${SRCNAME})
	g++ -o $@   $< `$(GIMPTOOL) --libs` 

$(addsuffix .o,${SRCNAME}) : $(addsuffix .cpp,${SRCNAME}) $(addsuffix .h,${SRCNAME})
	g++ -c -Wall -I.. `$(GIMPTOOL) --cflags` $< 

$(addsuffix .h,${SRCNAME}) :  ../common/params2c.xsl $(addsuffix .xml,${SRCNAME})
	xsltproc --output $@ $^ 

clean:
	rm -f ${SRCNAME} *.o $(addsuffix .h,${SRCNAME})

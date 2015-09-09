.PHONY:all clean install
GIMPTOOL?=gimptool-2.0
SRCNAME=gribouillis
CFLAGS=-I..
all:${SRCNAME}

install: ${SRCNAME}
	$(GIMPTOOL) --install-bin $<


${SRCNAME} : $(addprefix ${SRCNAME},.tab.cpp .tab.hh .hh)
	g++ -g -o $@ -I.. -Wall  $< `$(GIMPTOOL) --cflags --libs` 


$(addsuffix .tab.cpp,${SRCNAME}) :  ../common/params2c.xsl $(addsuffix .xml,${SRCNAME})
	xsltproc --output $@ $^ 

$(addsuffix .tab.hh,${SRCNAME}) :  ../common/params2h.xsl $(addsuffix .xml,${SRCNAME})
	xsltproc --output $@ $^ 

clean:
	rm -f ${SRCNAME} *.o $(addprefix ${SRCNAME},.tab.cpp .tab.hh)

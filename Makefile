.PHONY:all clean install
GIMPTOOL?=gimptool-2.0
SRCNAME=gribouillis
CMD_PREFIX=cmd_
STANDALONE=$(addprefix ${CMD_PREFIX},${SRCNAME})
CFLAGS=-I..
COMMDIR=./common/

all:${SRCNAME} ${STANDALONE}

install: ${SRCNAME} ${STANDALONE} 
	$(GIMPTOOL) --install-bin $<


	
${SRCNAME} ${STANDALONE}: $(addprefix ${SRCNAME},.tab.cpp .tab.hh .hh)
	g++ -g -o $@ -I. -Wall $(if $(findstring ${CMD_PREFIX},$@),-DSTANDALONE=1,) $< `$(GIMPTOOL) --cflags --libs` 

$(addsuffix .tab.cpp,${SRCNAME}) :  ${COMMDIR}/params2c.xsl $(addsuffix .xml,${SRCNAME})
	xsltproc --output $@ $^ 

$(addsuffix .tab.hh,${SRCNAME}) :  ${COMMDIR}/params2h.xsl $(addsuffix .xml,${SRCNAME})
	xsltproc --output $@ $^ 

clean:
	rm -f ${SRCNAME} *.o $(addprefix ${SRCNAME},.tab.cpp .tab.hh)
	

.PHONY:all clean install
GIMPTOOL?=gimptool-2.0
SRCNAME=gribouillis
TESTNAME=$(addprefix test,${SRCNAME})
CFLAGS=-I..
all:${SRCNAME} ${TESTNAME}

install: ${SRCNAME} 
	$(GIMPTOOL) --install-bin $<

${TESTNAME}: 
	xsltproc --output $(addsuffix .cpp,$@)   ../common/params2test.xsl $(addsuffix .xml,${SRCNAME})
	g++ -g -o $@ -I.. -Wall `$(GIMPTOOL) --cflags `  $(addsuffix .cpp,$@) -lcairo
	#rm $(addsuffix .cpp,$@)
	
${SRCNAME} : $(addprefix ${SRCNAME},.tab.cpp .tab.hh .hh)
	g++ -g -o $@ -I.. -Wall  $< `$(GIMPTOOL) --cflags --libs` 


$(addsuffix .tab.cpp,${SRCNAME}) :  ../common/params2c.xsl $(addsuffix .xml,${SRCNAME})
	xsltproc --output $@ $^ 

$(addsuffix .tab.hh,${SRCNAME}) :  ../common/params2h.xsl $(addsuffix .xml,${SRCNAME})
	xsltproc --output $@ $^ 

clean:
	rm -f ${SRCNAME} *.o $(addprefix ${SRCNAME},.tab.cpp .tab.hh)

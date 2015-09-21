.PHONY:all clean install
SRCNAME=hatching1
CFLAGS=-I..
TESTNAME=$(addprefix test,${SRCNAME})
all:${SRCNAME}

install: ${SRCNAME} ${TESTNAME}
	$(GIMPTOOL) --install-bin $<

${TESTNAME}: 
	xsltproc --output $(addsuffix .cpp,$@)   ../common/params2test.xsl $(addsuffix .xml,${SRCNAME})
	g++ -g -o $@ -I.. -Wall `$(GIMPTOOL) --cflags `  $(addsuffix .cpp,$@) -lcairo
	#rm $(addsuffix .cpp,$@)

${SRCNAME} : $(addsuffix .o,${SRCNAME})
	g++ -o $@   $< `$(GIMPTOOL) --libs` 


$(addsuffix .o,${SRCNAME}) : $(addsuffix .cpp,${SRCNAME})
	g++ -c -Wall -I.. `$(GIMPTOOL) --cflags` $< 

clean:
	rm -f ${SRCNAME} *.o

.PHONY:all clean install test
GIMPTOOL?=gimptool-2.0
COMMANDS=gribouillis
CMD_PREFIX=cmd_
CFLAGS=-I..
COMMDIR=./common/

define compile

$(1) $(addprefix ${CMD_PREFIX},$(1)): $$(addprefix $(1),.tab.cpp .tab.hh .hh)
	g++ -g -o $$@ -I. -Wall $$(if $$(findstring $${CMD_PREFIX},$$@),-DSTANDALONE=1,) $$< `$$(GIMPTOOL) --cflags --libs` 

$$(addsuffix .tab.cpp,$(1)) :  $${COMMDIR}/params2c.xsl $$(addsuffix .xml,$(1))
	xsltproc --output $$@ $$^ 

$$(addsuffix .tab.hh,$(1)) :  $${COMMDIR}/params2h.xsl $$(addsuffix .xml,$(1))
	xsltproc --output $$@ $$^ 

endef

all:${COMMANDS} $(addprefix ${CMD_PREFIX},${COMMANDS})

install: all
	$(foreach A,${COMMANDS}, $(GIMPTOOL) --install-bin ${A} ;)

$(eval $(foreach A,${COMMANDS},$(call compile,${A})))

test: $(addprefix ${CMD_PREFIX},${COMMANDS})
	./$(addprefix ${CMD_PREFIX},gribouillis) -o jeter.png

clean:
	rm -f *.o $(foreach A,${COMMANDS}, $(addprefix ${A},.tab.cpp .tab.hh) $(addprefix ${CMD_PREFIX},${A}) )
	

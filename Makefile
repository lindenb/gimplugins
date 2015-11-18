.PHONY:all clean install test
GIMPTOOL?=gimptool-2.0
COMMANDS=gribouillis kirby01 gribouille01 hatching01
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

test: $(addprefix test_,${COMMANDS})

test_gribouillis:
	./$(addprefix ${CMD_PREFIX},gribouillis) -o jeter.png
test_kirby01:
	./$(addprefix ${CMD_PREFIX},kirby01) --shape both -o kirby01a.png
test_gribouille01:
	./$(addprefix ${CMD_PREFIX},gribouille01) -o gribouille01a.png
test_hatching01:
	./$(addprefix ${CMD_PREFIX},hatching01) -o hatching01.png

clean:
	rm -f *.o $(foreach A,${COMMANDS}, $(addprefix ${A},.tab.cpp .tab.hh) $(addprefix ${CMD_PREFIX},${A}) )
	

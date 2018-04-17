LIBS=$(shell pkg-config --libs fann)
VERSION=$(shell swipl -q -t "version(X),write(X)" pack.pl)
CFLAGS:=$(CFLAGS) -O2 -fomit-frame-pointer -s -Wno-unused-result
TARGET=plfann
SOBJ=$(PACKSODIR)/$(TARGET).$(SOEXT)

default_target: package

all: $(SOBJ)

$(SOBJ): c/$(TARGET).o
	mkdir -p $(PACKSODIR)
	$(LD) $(LDSOFLAGS) -o $@ $(SWISOLIB) $< $(LIBS)
	strip -x $@


compile: bin
	swipl-ld -o bin/plfann -shared $(CFLAGS) c/plfann.c $(LIBS) -Wno-unused-result
	swipl-ld -o bin/plfann_double -shared $(CFLAGS) -DDOUBLEFANN c/plfann.c $(LIBS) -Wno-unused-result
	swipl-ld -o bin/plfann_fixed -shared $(CFLAGS) -DFIXEDFANN c/plfann.c $(LIBS) -Wno-unused-result

bin:
	mkdir -p bin

check::
	swipl -q -g main,halt example/example.pl

install:
install-me:: package
	swipl -g "pack_install('plfann-$(VERSION).tgz',[upgrade(true)]),halt"

remove::
	swipl -g "pack_remove(plfann),halt"
	
package:
	tar zcvf "plfann-$(VERSION).tgz" pack.pl prolog c example README.md Makefile

clean::
	rm -rf *~ ./*/*~ bin



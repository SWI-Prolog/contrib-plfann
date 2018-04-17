LIBS=$(shell pkg-config --libs fann)
VERSION=$(shell swipl -q -t "version(X),write(X)" pack.pl)
CFLAGS:=$(CFLAGS) -O2 -march -fomit-frame-pointer -s

default_target: all

all: package

compile:
	swipl-ld -o bin/plfann -shared $(CFLAGS) src/plfann.c $(LIBS) -Wno-unused-result
	swipl-ld -o bin/plfann_double -shared $(CFLAGS) -DDOUBLEFANN src/plfann.c $(LIBS) -Wno-unused-result
	swipl-ld -o bin/plfann_fixed -shared $(CFLAGS) -DFIXEDFANN src/plfann.c $(LIBS) -Wno-unused-result
check::
	swipl -q -g main,halt example.pl

install:: package
	swipl -g "pack_install('plfann-$(VERSION).tgz'),halt"

remove::
	swipl -g "pack_remove(plfann),halt"
	
package: compile
	tar zcvf "plfann-$(VERSION).tgz" pack.pl prolog/plfann.pl bin/plfann.so README.md

clean::
	rm -f *~ ./*/*~ ./bin/*.so



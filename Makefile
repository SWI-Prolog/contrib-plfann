LIBS=$(shell pkg-config --libs fann)
VERSION=$(shell swipl -q -t "version(X),write(X)" pack.pl)
override CFLAGS += -O2 -fomit-frame-pointer -s -c -Wno-unused-result
LD=swipl-ld

default_target: package

all: $(PACKSODIR)/plfann.$(SOEXT) $(PACKSODIR)/plfann_double.$(SOEXT) $(PACKSODIR)/plfann_fixed.$(SOEXT)

$(PACKSODIR)/%.$(SOEXT): c/%.o
	mkdir -p $(PACKSODIR)
	$(LD) $(LDSOFLAGS) $(LIBS) -o $@ $(SWISOLIB) $<
	strip -x $@

c/%.o: c/%.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $<

c/plfann_double.o: c/plfann.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ -DDOUBLEFANN $<

c/plfann_fixed.o: c/plfann.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ -DFIXEDFANN $<

check::
	$(MAKE) -C example

install:
install-me:: package
	swipl -g "pack_install('plfann-$(VERSION).tgz',[upgrade(true)]),halt"

remove::
	swipl -g "pack_remove(plfann),halt"
	
package: clean
	tar zcvf "plfann-$(VERSION).tgz" pack.pl prolog c example README.md Makefile

clean::
	rm -rf *~ ./*/*~ ./*/*.o



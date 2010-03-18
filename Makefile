LIBS=$(shell pkg-config --libs fann)

default_target: plfann.so

plfann.so: plfann.c plfann.h
	swipl-ld -o $@ -shared plfann.c $(LIBS)

clean::
	rm -f *~ *.so
	rm xor_float.net

test::
	swipl -q -t main example.pl

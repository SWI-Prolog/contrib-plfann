LIBS=$(shell pkg-config --libs fann)

plfann.so:	plfann.c plfann.h
	plld -o $@ -shared plfann.c $(LIBS)

clean::
	rm -f *~ *.so

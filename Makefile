MAJORVERSION=0
MINORVERSION=1

LIBDIR=/usr/lib
INCDIR=/usr/include
OPTFLAGS=-march=native -mtune=native -O3
CFLAGS=-std=c99 -pedantic -Wall -Wextra ${OPTFLAGS}
LIBS=-lm
LDFLAGS=-fpic ${LIBS}

all: lib/libcerebellum.so.${MAJORVERSION}.${MINORVERSION}

install: lib/libcerebellum.so.${MAJORVERSION}.${MINORVERSION} uninstall
	cp include/cerebellum.h ${INCDIR}
	cp $< ${LIBDIR}
	ln -s ${LIBDIR}/libcerebellum.so.${MAJORVERSION}.${MINORVERSION} ${LIBDIR}/libcerebellum.so.${MAJORVERSION}
	ln -s ${LIBDIR}/libcerebellum.so.${MAJORVERSION} ${LIBDIR}/libcerebellum.so

lib/libcerebellum.so.${MAJORVERSION}.${MINORVERSION}: matrix.o learning.o
	cc ${LDFLAGS} -shared -o $@ $^ -Wl,-soname,libcerebellum.so.${MAJORVERSION}

matrix.o: src/matrix.c
	cc ${CFLAGS} ${LDFLAGS} -I include -c -o $@ $<

learning.o: src/learning.c
	cc ${CFLAGS} ${LDFLAGS} -I include -c -o $@ $<

uninstall:
	rm -f ${INCDIR}/cerebellum.h
	rm -f ${LIBDIR}/libcerebellum.*

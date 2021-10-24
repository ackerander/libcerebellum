MAJORVERSION=0
MINORVERSION=0

LIBDIR=/usr/lib
INCDIR=/usr/include
CFLAGS=-std=c99 -pedantic -Wall -Wextra
LDFLAGS=-fpic

all: lib/libcerebellum.so.${MAJORVERSION}.${MINORVERSION}

install: lib/libcerebellum.so.${MAJORVERSION}.${MINORVERSION} 
	cp include/cerebellum.h ${INCDIR}
	cp $< ${LIBDIR}
	ln -sf ${LIBDIR}/libcerebellum.so.${MAJORVERSION}.${MINORVERSION} ${LIBDIR}/libcerebellum.so.${MAJORVERSION}
	ln -sf ${LIBDIR}/libcerebellum.so.${MAJORVERSION} ${LIBDIR}/libcerebellum.so

lib/libcerebellum.so.${MAJORVERSION}.${MINORVERSION}: libcerebellum.o lib
	cc ${LDFLAGS} -shared -o $@ $< -Wl,-soname,libcerebellum.so.${MAJORVERSION}

lib:
	mkdir lib

libcerebellum.o: src/matrix.c
	cc ${CFLAGS} ${LDFLAGS} -I include -c -o $@ $^

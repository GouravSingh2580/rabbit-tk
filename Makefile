LDFLAGS=-L/usr/local/lib -lxcb -lxcb-render -lxcb-render-util -lcairo -lxcb-atom
CXXFLAGS=-Wall -g -I/usr/local/include/cairo -I/usr/local/include -I/usr/include/freetype2 -I/usr/include/libpng12
LD=g++
OBJFILES=main.o window.o eventloop.o atomcache.o menu.o

all: main

main: $(OBJFILES)
	$(LD) $(LDFLAGS) $(OBJFILES) -o $@
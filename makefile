# compiler options
CC = g++
CC_OPTIONS := -Wall -Wextra -O2 `pkg-config --cflags pugixml`
LIBS != pkg-config --libs pugixml
PREFIX = /usr

build: backend_jmdict.o termjiten.o utils.o
	$(CC) *.o -o termjiten $(LIBS) $(CC_OPTIONS)

clean:
	rm *.o termjiten

install:
	mkdir -p $(PREFIX)/share/termjiten
	mkdir -p $(PREFIX)/share/doc/termjiten
	cp -f JMdict $(PREFIX)/share/termjiten
	cp -r documentation/* $(PREFIX)/share/doc/termjiten
	cp -f ./termjiten $(PREFIX)/bin

uninstall:
	rm -r $(PREFIX)/share/termjiten
	rm -r $(PREFIX)/share/doc/termjiten
	rm $(PREFIX)/bin/termjiten

# individual files
backend_jmdict.o:
	$(CC) -c include/backend_jmdict.cpp $(CC_OPTIONS) -o backend_jmdict.o

utils.o:
	$(CC) -c include/utils.cpp $(CC_OPTIONS) -o utils.o

termjiten.o:
	$(CC) -c termjiten.cpp $(CC_OPTIONS) -o termjiten.o


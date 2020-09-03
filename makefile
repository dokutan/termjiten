# compiler options
CC = g++
CC_OPTIONS := -Wall -Wextra -O2 `pkg-config --cflags pugixml`
LIBS != pkg-config --libs pugixml
PREFIX = /usr

build: backend_jmdict.o backend_jmnedict.o backend_kanjidic.o termjiten.o utils.o
	$(CC) *.o -o termjiten $(LIBS) $(CC_OPTIONS)

clean:
	rm *.o termjiten

install:
	mkdir -p $(PREFIX)/share/termjiten
	mkdir -p $(PREFIX)/share/doc/termjiten
	cp -f JMdict $(PREFIX)/share/termjiten
	cp -f JMnedict.xml $(PREFIX)/share/termjiten | true
	cp -f kanjidic2.xml $(PREFIX)/share/termjiten
	cp -r documentation/* $(PREFIX)/share/doc/termjiten
	cp -f ./termjiten $(PREFIX)/bin

uninstall:
	rm -r $(PREFIX)/share/termjiten
	rm -r $(PREFIX)/share/doc/termjiten
	rm $(PREFIX)/bin/termjiten

# individual files
backend_jmdict.o:
	$(CC) -c include/backend_jmdict.cpp $(CC_OPTIONS) -o backend_jmdict.o

backend_jmnedict.o:
	$(CC) -c include/backend_jmnedict.cpp $(CC_OPTIONS) -o backend_jmnedict.o

backend_kanjidic.o:
	$(CC) -c include/backend_kanjidic.cpp $(CC_OPTIONS) -o backend_kanjidic.o

utils.o:
	$(CC) -c include/utils.cpp $(CC_OPTIONS) -o utils.o

termjiten.o:
	$(CC) -c termjiten.cpp $(CC_OPTIONS) -o termjiten.o

# download and extract dictionary files
jmdict:
	wget http://ftp.monash.edu/pub/nihongo/JMdict.gz
	gunzip JMdict.gz

jmnedict:
	wget http://ftp.monash.edu/pub/nihongo/JMnedict.xml.gz
	gunzip JMnedict.xml.gz

kanjidic:
	wget http://www.edrdg.org/kanjidic/kanjidic2.xml.gz
	gunzip kanjidic2.xml.gz

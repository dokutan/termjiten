# termjiten
A simple Japanese/English dictionary for the terminal.

## Installing
0. Install dependencies [pugixml](https://pugixml.org/), a recent C++ compiler (gcc), make, pkgconf
1. Clone or download this repository
```
git clone https://github.com/dokutan/termjiten
cd termjiten
```
2. Download and extract the JMdict, Kanjidic and optionally the JMnedict dictionary files, manually or with
```
make jmdict
make jmnedict
make kanjidic
```
3. Build and install with
```
make -j
sudo make install
```

## Usage
Show a list of all options
```
termjiten -h
```
Show license information and sources
```
termjiten -l
```
Exact search term matching
```
termjiten -e query
```
Fuzzy search term matching
```
termjiten -f query
```
Regex based search term matching
```
termjiten -r query
```

### Configuration
You can use an .ini file to modify the output format and select which dictionaries should be searched. The default path for this is ``$HOME/.config/termjiten.ini``, if the ``TERMJITEN_CONFIG`` environmental variable is set, that is used as a path. An annotated example is found in this directory.

## TODO
- Use additional sources to display more detailed information
- Add an option to select the sources
- Optionally display translations in more/different languages

## Limitations an potential problems
- If you don't see any japanese characters, install an appropriate font and use a terminal emulator that supports UTF-8
- If you use a non-UTF-8 locale (if you don't know you most likely are using UTF-8) things might not work as intended, solution: use UTF-8.
- Searching by romaji is not supported, you have to use japanese characters if you want to search for japanese words. An IME (e.g. anthy) might be useful.

## License and sources
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program uses the JMdict/EDICT, JMnedict/ENAMDICT and KANJIDIC dictionary files. These files are the property of the Electronic Dictionary Research and Development Group, and are used in conformance with the Group's licence.<br>
Copies of the documentation and license files are installed in /usr/share/doc/termjiten/jmdict+jmnedict+kanjidic and can be found in documentation/jmdict+jmnedict+kanjidic.<br>
http://www.edrdg.org<br>
http://www.edrdg.org/wiki/index.php/JMdict-EDICT_Dictionary_Project<br>
http://www.edrdg.org/enamdict/enamdict_doc.html<br>
http://www.edrdg.org/wiki/index.php/KANJIDIC_Project<br>
http://www.edrdg.org/edrdg/licence.html

The SKIP (System of Kanji Indexing by Patterns) system for ordering kanji was developed by Jack Halpern (Kanji Dictionary Publishing Society at http://www.kanji.org/), and is used with his permission.

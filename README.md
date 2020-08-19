# termjiten
A simple Japanese/English dictionary for the terminal.

## Installing
0. Install dependencies [pugixml](https://pugixml.org/), a recent C++ compiler (gcc), make, pkgconf
1. Clone or download this repository
```
git clone https://github.com/dokutan/termjiten
cd termjiten
```
2. Download and extract the JMdict dictionary file, manually or with
```
wget http://ftp.monash.edu/pub/nihongo/JMdict.gz
gunzip JMdict.gz
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

## TODO
- There is currently no control over the output format
- Use additional sources to display more detailed information

## License and sources
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This program uses the JMdict/EDICT dictionary files. These files are the property of the Electronic Dictionary Research and Development Group, and are used in conformance with the Group's licence.<br>
Copies of the documentation and license files are installed in /usr/share/doc/termjiten/jmdict and can be found in documentation/jmdict.<br>
http://www.edrdg.org<br>
http://www.edrdg.org/wiki/index.php/JMdict-EDICT_Dictionary_Project<br>
http://www.edrdg.org/edrdg/licence.html

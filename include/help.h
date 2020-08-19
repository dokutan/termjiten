/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <string>
#include <cstring>
#include <exception>

void print_help( std::ostream& output ){
	
	output
	<< "termjiten usage:\n\n"
	<< "-e QUERY\tsearch (exact matching)\n"
	<< "-f QUERY\tsearch (fuzzy matching)\n"
	<< "-f QUERY\tsearch (regex matching)\n"
	<< "-l\t\tshow license and source informaton\n"
	<< "-h\t\tshow this message\n";
	
}

void print_licenses( std::ostream& output ){
	
	output
	<< "This program is free software; you can redistribute it and/or "
	<< "modify it under the terms of the GNU General Public License as "
	<< "published by the Free Software Foundation; either version 3 of "
	<< "the License, or (at your option) any later version.\n\n";
	
	output
	<< "This program uses the JMdict/EDICT dictionary files. "
	<< "These files are the property of the Electronic Dictionary "
	<< "Research and Development Group, and are used in conformance "
	<< "with the Group's licence.\n"
	<< "Copies of the documentation and license files are installed in "
	<< "/usr/share/doc/termjiten/jmdict.\n"
	<< "http://www.edrdg.org\n"
	<< "http://www.edrdg.org/wiki/index.php/JMdict-EDICT_Dictionary_Project\n"
	<< "http://www.edrdg.org/edrdg/licence.html\n";
	
}

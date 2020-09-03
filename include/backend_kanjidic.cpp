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

#include <pugixml.hpp>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include "backend_kanjidic.h"
#include "utils.h"

// Prints a kanjidic entry (character)
void dictionary_kanjidic::print_entry( pugi::xml_node& character, std::ostream& output, std::map< std::string, std::string >& options ){
	
	// parse options
	bool color = string_to_bool( options["kanjidic.color"], true );
	bool verbose = string_to_bool( options["kanjidic.verbose"], true );
	bool symbols = string_to_bool( options["kanjidic.symbols"], false );
	
	std::map< std::string, std::string > colors = {
		{"keb", "\e[91m"},
		{"reb", "\e[94m"},
		{"trans", "\e[30m"},
		{"extra", "\e[92m"},
		{"reset", "\e[0m"}
	};
	
	output << "\n";
	
	// print character.literal (kanji)
	for( pugi::xml_node literal : character.children("literal") ){
		output << (color ? colors["keb"] : "");
		output << literal.child_value() << "\n";
		output << (color ? colors["reset"] : "");
	}
	
	// print readings and meanings of the character
	for( pugi::xml_node reading_meaning : character.children("reading_meaning") ){
		
		// readings and meanigns
		for( pugi::xml_node rmgroup : reading_meaning.children("rmgroup") ){
			
			// readings
			for( pugi::xml_node reading : rmgroup.children("reading") ){
				
				// if japanese reading, TODO! mark jouyou kanji readings
				if( strcmp( reading.attribute("r_type").as_string(), "ja_on" ) == 0 || 
					strcmp( reading.attribute("r_type").as_string(), "ja_kun" ) == 0 ){
					
					output << (color ? colors["reb"] : "");
					output << reading.child_value() << "\n";
					output << (color ? colors["reset"] : "");
						
				}
				
			}
			
			// meanings
			for( pugi::xml_node meaning : rmgroup.children("meaning") ){
				
				// if english meaning
				if( !meaning.attribute("m_lang") ){
					output << (color ? colors["trans"] : "");
					output << meaning.child_value() << "\n";
					output << (color ? colors["reset"] : "");
				}
				
			}
				
		}
		
		// nanori TODO!
		
	}
	
	
	// additional information
	if( verbose ){
		output << (color ? colors["extra"] : "");
	
		// codepoint
		for( pugi::xml_node cp_value : character.child("codepoint").children("cp_value") ){
			
			// Unicode
			if( strcmp( cp_value.attribute("cp_type").as_string(), "ucs" ) == 0 )
				output << "U+" << cp_value.child_value();
			
			// JIS X 0208-1997
			if( strcmp( cp_value.attribute("cp_type").as_string(), "jis208" ) == 0 )
				output << ", JIS X 0208-1997: " << cp_value.child_value();
			
			// JIS X 0212-1990
			if( strcmp( cp_value.attribute("cp_type").as_string(), "jis212" ) == 0 )
				output << ", JIS X 0212-1990: " << cp_value.child_value();
			
			// JIS X 0213-2000
			if( strcmp( cp_value.attribute("cp_type").as_string(), "jis213" ) == 0 )
				output << ", JIS X 0213-2000: " << cp_value.child_value();
			
		}
		output << "\n";
	
		// radical name (if kanji is a radical)
		for( pugi::xml_node rad_name : character.child("misc").children("rad_name") )
			output << "Radical name: " << rad_name.child_value() << "\n";
		
		// stroke count
		output << "Strokes: ";
		bool first = true;
		for( pugi::xml_node stroke_count : character.child("misc").children("stroke_count") ){
			
			if( !first )
				output << ", Not: ";
			
			first = false;
			output << stroke_count.child_value();
		}
		output << "\n";
		
		// grade TODO!
		
		// jlpt level
		for( pugi::xml_node jlpt : character.child("misc").children("jlpt") )
			output << "Former JLPT level (4-1): " << jlpt.child_value() << "\n";
		
		// query code TODO!
		
		// dic_ref TODO!
		
		output << (color ? colors["reset"] : "");
	}

}

/** Loads Kanjidic, performs search and prints results
 * \param query search query
 * \param method string matching method (exact, fuzzy, regex)
 */
void dictionary_kanjidic::search( std::string query, std::string method, std::ostream& output, std::map< std::string, std::string >& options ){
	
	comparison comp(method);
	
	// for each entry:
	for( pugi::xml_node character : kanjidic.child("kanjidic2").children("character") ){
		
		bool match_found = false;

		// match in character.literal (kanji)?
		for( pugi::xml_node literal : character.children("literal") ){
			if( comp( literal.child_value(), query ) )
				match_found = true;
		}
		
		// TODO! more search options: query code, reading
		
		// if match, print entry
		if( match_found )
			print_entry( character, output, options );
		
	}
	
}

void dictionary_kanjidic::parse_entities( std::string path ){
	
	// open file
	std::ifstream xml_file( path );
	if( !xml_file.is_open() )
		return;
	
	std::string line;
	while( std::getline( xml_file, line ) ){
		
		if( std::regex_match( line, std::regex("<!ENTITY [[:print:]]+ \\\"[[:print:]]+\\\">") ) ){
			
			std::string entity = std::regex_replace( line, std::regex("<!ENTITY "), "" );
			entity = std::regex_replace( entity, std::regex(" \\\"[[:print:]]+\\\">"), "" );
			
			std::string value = std::regex_replace( line, std::regex("<!ENTITY [[:print:]]+ \\\""), "" );
			value = std::regex_replace( value, std::regex("\">"), "" );
			
			entities[ "&"+entity+";" ] = value;
		}
		
		// stop reading the file after all entities are loaded
		if( line == "<kanjidic2>" )
			break;
		
	}
	
	xml_file.close();
	
}
	

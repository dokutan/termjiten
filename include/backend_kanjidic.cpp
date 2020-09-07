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
	
	// currently not used
	// bool symbols = string_to_bool( options["kanjidic.symbols"], false );
	
	output << "\n";
	
	// print character.literal (kanji)
	for( pugi::xml_node literal : character.children("literal") ){
		output << (color ? options["colors.kanji"] : "");
		output << literal.child_value() << "\n";
		output << (color ? options["colors.reset"] : "");
	}
	
	// print readings and meanings of the character
	for( pugi::xml_node reading_meaning : character.children("reading_meaning") ){
		
		// readings and meanigns
		for( pugi::xml_node rmgroup : reading_meaning.children("rmgroup") ){
			
			// readings
			for( pugi::xml_node reading : rmgroup.children("reading") ){
				
				// if japanese reading
				if( strcmp( reading.attribute("r_type").as_string(), "ja_on" ) == 0 || 
					strcmp( reading.attribute("r_type").as_string(), "ja_kun" ) == 0 ){
					
					output << (color ? options["colors.kana"] : "");
					output << reading.child_value();
					
					// type of reading
					/* these are described in the DTD but don't seem to be used
					if( reading.attribute("on_type") ){
						output << (color ? options["colors.extra"] : "");
						output << " (" << reading.attribute("on_type").as_string() << ")";
					}
					
					if( strcmp( reading.attribute("r_status").as_string(), "jy" ) == 0 ){
						output << (color ? options["colors.extra"] : "");
						output << " (jouyou kanji)";
					}
					*/
					
					output << "\n" << (color ? options["colors.reset"] : "");
						
				}
				
			}
			
			// nanori (readings only associated with names)
			for( pugi::xml_node nanori : reading_meaning.children("nanori") ){
				output << (color ? options["colors.kana"] : "");
				output << nanori.child_value();
				output << (color ? options["colors.extra"] : "") << " (nanori)\n";
				output << (color ? options["colors.reset"] : "");
			}
			
			// meanings
			for( pugi::xml_node meaning : rmgroup.children("meaning") ){
				
				// if english meaning
				if( !meaning.attribute("m_lang") ){
					output << (color ? options["colors.trans"] : "");
					output << meaning.child_value() << "\n";
					output << (color ? options["colors.reset"] : "");
				}
				
				// non-english meaning
				else if( options["kanjidic.languages"].find( meaning.attribute("m_lang").as_string() ) != std::string::npos ){
					output << (color ? options["colors.trans"] : "");
					output << "[" << meaning.attribute("m_lang").as_string() << "] " << meaning.child_value() << "\n";
					output << (color ? options["colors.reset"] : "");
				}
				
			}
				
		}
		
	}
	
	
	// additional information
	if( verbose ){
		output << (color ? options["colors.extra"] : "");
	
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
		
		// grade
		for( pugi::xml_node grade : character.child("misc").children("grade") )
			output << "Grade (1-10): " << grade.child_value() << "\n";
		
		// jlpt level
		for( pugi::xml_node jlpt : character.child("misc").children("jlpt") )
			output << "Former JLPT level (4-1): " << jlpt.child_value() << "\n";
		
		// query code
		first = true;
		for( pugi::xml_node q_code : character.child("query_code").children("q_code") ){
			
			// SKIP
			if( strcmp( q_code.attribute("qc_type").as_string(), "skip" ) == 0 ){
				output << (first ? "" : ", ") << "SKIP: ";
				output << (q_code.attribute("skip_misclass") ? "not " : "");
				output << q_code.child_value();
				first = false;
			}
			// The Kanji Dictionary (Tuttle 1996) by Spahn and Hadamitzky
			else if( strcmp( q_code.attribute("qc_type").as_string(), "sh_desc" ) == 0 ){
				output << (first ? "" : ", ") << "Spahn and Hadamitzky: ";
				// output << (q_code.attribute("misclass") ? "not " : ""); // unused
				output << q_code.child_value();
				first = false;
			}
			// Four corner
			else if( strcmp( q_code.attribute("qc_type").as_string(), "four_corner" ) == 0 ){
				output << (first ? "" : ", ") << "Four corner: ";
				// output << (q_code.attribute("misclass") ? "not " : ""); // unused
				output << q_code.child_value();
				first = false;
			}
			// De Roo
			else if( strcmp( q_code.attribute("qc_type").as_string(), "deroo" ) == 0 ){
				output << (first ? "" : ", ") << "De Roo: ";
				// output << (q_code.attribute("misclass") ? "not " : ""); // unused
				output << q_code.child_value();
				first = false;
			}
			
		}
		output << "\n";
		
		// TODO! dic_number.dic_ref
		
		// variant or cross-reference, TODO! type
		/*for( pugi::xml_node variant : character.child("misc").children("variant") ){
			output << (color ? options["colors.extra"] : "");
			output << (symbols ? "⇒ " : "See also: ") << variant.child_value() << "\n";
			output << (color ? options["colors.reset"] : "");
		}*/
		
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
		
		output << (color ? options["colors.reset"] : "");
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
		
		// match in reading
		for( pugi::xml_node reading : character.child("reading_meaning").child("rmgroup").children("reading") ){
			
			// non-japanese reading?
			if( strcmp( reading.attribute("r_type").as_string(), "ja_on" ) != 0 && 
				strcmp( reading.attribute("r_type").as_string(), "ja_kun" ) != 0 ){
					continue;
			}
			
			if( comp( reading.child_value(), query ) )
				match_found = true;
		}
		
		// match in nanori
		for( pugi::xml_node nanori : character.child("reading_meaning").children("nanori") ){
			if( comp( nanori.child_value(), query ) )
				match_found = true;
		}
		
		// match in query code
		for( pugi::xml_node q_code : character.child("query_code").children("q_code") ){
			if( comp( q_code.child_value(), query ) )
				match_found = true;
		}
		
		// match in meanings
		for( pugi::xml_node meaning : character.child("reading_meaning").child("rmgroup").children("meaning") ){
			
			// if meaning in language that should not be included
			if( options["kanjidic.languages"].find( meaning.attribute("m_lang").as_string() ) == std::string::npos )
				continue;
			
			if( comp( meaning.child_value(), query ) )
				match_found = true;
		}
		
		// match in codepoint
		for( pugi::xml_node cp_value : character.child("codepoint").children("cp_value") ){
			if( comp( cp_value.child_value(), query ) )
				match_found = true;
		}
		
		// TODO! search in dic_number.dic_ref
		
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
	

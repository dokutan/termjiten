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

#include "backend_jmnedict.h"
#include "utils.h"

// Prints a JMnedict entry
void dictionary_jmnedict::print_entry( pugi::xml_node& entry, std::ostream& output, std::map< std::string, std::string >& options ){
	
	// parse options
	bool color = string_to_bool( options["jmnedict.color"], true );
	bool verbose = string_to_bool( options["jmnedict.verbose"], true );
	bool symbols = string_to_bool( options["jmnedict.symbols"], false );
	
	output << "\n";
	
	// print all k_ele.keb (kanji)
	for( pugi::xml_node k_ele : entry.children("k_ele") ){
		for( pugi::xml_node keb : k_ele.children("keb") ){
			output << (color ? options["colors.kanji"] : "");
			output << keb.child_value() << "\n";
			output << (color ? options["colors.reset"] : "");
		}
		
		if( verbose ){
			
			// kanji information
			for( pugi::xml_node ke_inf : k_ele.children("ke_inf") ){
				output << (color ? options["colors.extra"] : "");
				output << (symbols ? "🛈 " : "Info: ") << entities[ke_inf.child_value()] << "\n";
				output << (color ? options["colors.reset"] : "");
			}
		}
		
		
	}

	// print all r_ele.reb (reading)
	for( pugi::xml_node r_ele : entry.children("r_ele") ){
		for( pugi::xml_node reb : r_ele.children("reb") ){
			output << (color ? options["colors.kana"] : "");
			output << reb.child_value() << "\n";
			output << (color ? options["colors.reset"] : "");
		}
		
		// print extra information if requested
		if( verbose ){
			
			// reading information
			for( pugi::xml_node re_inf : r_ele.children("re_inf") ){
				output << (color ? options["colors.extra"] : "");
				output << (symbols ? "🛈 " : "Info: ") << entities[re_inf.child_value()] << "\n";
				output << (color ? options["colors.reset"] : "");
			}
			
			// reading restricted to one kanji element
			for( pugi::xml_node re_restr : r_ele.children("re_restr") ){
				output << (color ? options["colors.extra"] : "");
				output << (symbols ? "⚠ " : "Restricted to: ") << re_restr.child_value() << "\n";
				output << (color ? options["colors.reset"] : "");
			}
			
		}
		
	}

	// print all translations of the word/entry
	for( pugi::xml_node trans : entry.children("trans") ){
		
		// trans_det
		std::vector < std::string > meanings;
		for( pugi::xml_node trans_det : trans.children("trans_det") ){
			
			// non-english translation?
			if( trans_det.attribute("xml:lang") )
				continue;
			
			meanings.push_back( trans_det.child_value() );
			
		}
		
		if( meanings.size() > 0 )
			output << (color ? options["colors.trans"] : "") << meanings[0];
		for( size_t i = 1; i < meanings.size(); i++ )
			output << ", " << meanings[i];
		if( meanings.size() > 0 )
			output << (color ? options["colors.reset"] : "") << "\n";
		
		// print extra information if requested
		if( verbose ){
			
			// name type
			for( pugi::xml_node name_type : trans.children("name_type") ){
				output << (color ? options["colors.extra"] : "");
				output << (symbols ? "Ⓣ " : "Type: ") << entities[name_type.child_value()] << "\n";
				output << (color ? options["colors.reset"] : "");
			}
			
			// cross references
			for( pugi::xml_node xref : trans.children("xref") ){
				output << (color ? options["colors.extra"] : "");
				output << (symbols ? "⇒ " : "See also: ") << xref.child_value() << "\n";
				output << (color ? options["colors.reset"] : "");
			}
			
		}
		
	}

}

/** Loads JMnedict, performs search and prints results
 * \param query search query
 * \param method string matching method (exact, fuzzy, regex)
 */
void dictionary_jmnedict::search( std::string query, std::string method, std::ostream& output, std::map< std::string, std::string >& options ){
	
	comparison comp(method);
	
	// for each entry:
	for( pugi::xml_node entry : jmnedict.child("JMnedict").children("entry") ){
		
		bool match_found = false;

		// match in k_ele.keb (kanji)?
		for( pugi::xml_node k_ele : entry.children("k_ele") ){
			for( pugi::xml_node keb : k_ele.children("keb") ){
				if( comp( keb.child_value(), query ) )
					match_found = true;
			}
		}

		// match in r_ele.reb (reading)?
		for( pugi::xml_node r_ele : entry.children("r_ele") ){
			for( pugi::xml_node reb : r_ele.children("reb") ){
				if( comp( reb.child_value(), query ) )
					match_found = true;
			}
		}

		// match in trans.trans_det (english translation)?
		for( pugi::xml_node trans : entry.children("trans") ){
			for( pugi::xml_node trans_det : trans.children("trans_det") ){
				
				// non-english meaning?
				if( trans_det.attribute("xml:lang") )
					continue;
				
				if( comp( trans_det.child_value(), query ) )
					match_found = true;
			}
		}

		// if match, print entry
		if( match_found )
			print_entry( entry, output, options );
		
	}
	
}

void dictionary_jmnedict::parse_entities( std::string path ){
	
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
		if( line == "<JMnedict>" )
			break;
		
	}
	
	xml_file.close();
	
}
	

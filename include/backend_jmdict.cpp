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

#include "backend_jmdict.h"

// Prints a JMdict entry
void dictionary_jmdict::print_entry( pugi::xml_node& entry, std::ostream& output, bool color, bool verbose ){

	std::map< std::string, std::string > colors = {
		{"keb", "\e[91m"},
		{"reb", "\e[94m"},
		{"gloss", "\e[90m"},
		{"extra", "\e[92m"},
		{"reset", "\e[0m"}
	};
	
	output << "\n";
	
	// print all k_ele.keb (kanji)
	for( pugi::xml_node k_ele : entry.children("k_ele") ){
		for( pugi::xml_node keb : k_ele.children("keb") ){
			output << (color ? colors["keb"] : "");
			output << keb.child_value() << "\n";
			output << (color ? colors["reset"] : "");
		}
		
		// kanji information
		/*for( pugi::xml_node ke_inf : k_ele.children("ke_inf") ){
			output << (color ? colors["extra"] : "");
			output << ke_inf.child_value() << "\n";
			output << (color ? colors["reset"] : "");
		}*/
		
	}

	// print all r_ele.reb (reading)
	for( pugi::xml_node r_ele : entry.children("r_ele") ){
		for( pugi::xml_node reb : r_ele.children("reb") ){
			output << (color ? colors["reb"] : "");
			output << reb.child_value() << "\n";
			output << (color ? colors["reset"] : "");
		}
	}

	// print all senses
	for( pugi::xml_node sense : entry.children("sense") ){
		
		// gloss (english meaning)
		std::vector < std::string > meanings;
		for( pugi::xml_node gloss : sense.children("gloss") ){
			
			// non-english meaning?
			if( gloss.attribute("xml:lang") )
				continue;
			
			meanings.push_back( gloss.child_value() );
		}
		if( meanings.size() > 0 )
			output << (color ? colors["gloss"] : "") << meanings[0];
		for( size_t i = 1; i < meanings.size(); i++ )
			output << ", " << meanings[i];
		if( meanings.size() > 0 )
			output << (color ? colors["reset"] : "") << "\n";
		
		// print extra information if requested
		if( verbose ){
			
			// sense information
			for( pugi::xml_node s_inf : sense.children("s_inf") ){
				output << (color ? colors["extra"] : "");
				output << s_inf.child_value() << "\n";
				output << (color ? colors["reset"] : "");
			}
			
			// restricted meanings
			for( pugi::xml_node stagk : sense.children("stagk") ){
				output << (color ? colors["extra"] : "");
				output << "Restricted to: " << stagk.child_value() << "\n";
				output << (color ? colors["reset"] : "");
			}
			for( pugi::xml_node stagr : sense.children("stagr") ){
				output << (color ? colors["extra"] : "");
				output << "Restricted to: " << stagr.child_value() << "\n";
				output << (color ? colors["reset"] : "");
			}
			
			// field
			
			// pos
			
			// cross references
			for( pugi::xml_node xref : sense.children("xref") ){
				output << (color ? colors["extra"] : "");
				output << "See also: " << xref.child_value() << "\n";
				output << (color ? colors["reset"] : "");
			}
			
		}
		
	}

}

/** Loads JMdict, performs search and prints results
 * \param query search query
 * \param method string matching method (exact, fuzzy, regex)
 */
void dictionary_jmdict::search( std::string query, std::string method, std::ostream& output, bool color, bool verbose ){
	
	comparison comp(method);
	
	// for each entry:
	for( pugi::xml_node entry : jmdict.child("JMdict").children("entry") ){
		
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

		// match in sense.gloss (english meaning)?
		for( pugi::xml_node sense : entry.children("sense") ){
			for( pugi::xml_node gloss : sense.children("gloss") ){
				
				// non-english meaning?
				if( gloss.attribute("xml:lang") )
					continue;
				
				if( comp( gloss.child_value(), query ) )
					match_found = true;
			}
		}

		// if match, print entry
		if( match_found )
			print_entry( entry, output, color, verbose );
		
	}
	
}

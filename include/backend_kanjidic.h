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

#ifndef BACKEND_KANJIDIC
#define BACKEND_KANJIDIC

#include <pugixml.hpp>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <fstream>
#include <cstring>

#include "comparison.h"

/// Class for interaction with JMnedict
class dictionary_kanjidic{
	
	private:
		
		void print_entry( pugi::xml_node& entry, std::ostream& output, std::map< std::string, std::string >& options );
		
		pugi::xml_document kanjidic;
		pugi::xml_parse_result parse_result;
		
		/// a lookup table for the custom xml entities (these are not parsed by pugixml) 
		std::map< std::string, std::string > entities;
		
		void parse_entities( std::string path );
	
	public:
		
		/// constructor (opens file)
		dictionary_kanjidic( std::string path ){
			
			parse_result = kanjidic.load_file( path.c_str(), pugi::parse_full );
			parse_entities( path );
			
		}
		
		/// check if file was loaded correctly
		bool is_open(){
			if( parse_result )
				return true;
			else
				return false;
		}
		
		/// Perform search, print results
		void search( std::string query, std::string method, std::ostream& output, std::map< std::string, std::string >& options );
	
};

#endif

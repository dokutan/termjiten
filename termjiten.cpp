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

#include <iostream>
#include <string>
#include <cstring>

#include <stdlib.h> // for getenv()

#include "include/options.h"
#include "include/help.h"
#include "include/backend_jmdict.h"

int main( int argc, char *argv[] ){
	
	// determine jmdict path
	char* jmdict_env = getenv( "JMDICT" );
	std::string jmdict_path;
	
	if( jmdict_env == NULL )
		jmdict_path = "/usr/share/termjiten/JMdict";
	else
		jmdict_path = jmdict_env;
	
	// Parse commandline arguments (TODO! verbose, color)
	std::string query, method;
	
	if( option_exists( "-h", argc, argv ) ){
		
		print_help( std::cout );
		return 0;
		
	} else if( option_exists( "-l", argc, argv ) ){
		
		print_licenses( std::cout );
		return 0;
		
	} else if( option_get_argument( "-e", query, argc, argv ) ){
		
		method = "exact";
		
	} else if( option_get_argument( "-r", query, argc, argv ) ){
		
		method = "regex";
		
	} else if( option_get_argument( "-f", query, argc, argv ) ){
		
		method = "fuzzy";
		
	} else{
		
		std::cerr << "Arguments missing, run " << argv[0] << " -h for help\n";
		return 0;
		
	}
	
	// Open JMdict
	dictionary_jmdict jmdict(jmdict_path);
	if( !jmdict.is_open() ){
		std::cerr << "Error: could not open " << jmdict_path << "\n";
		return 1;
	}
	
	// Perform search
	jmdict.search( query, method, std::cout, true, true );
	
	return 0;
}

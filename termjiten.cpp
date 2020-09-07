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
#include "include/simple_ini_parser.h"
#include "include/utils.h"
#include "include/backend_jmdict.h"
#include "include/backend_jmnedict.h"
#include "include/backend_kanjidic.h"

int main( int argc, char *argv[] ){
	
	// Parse commandline arguments
	std::string query, method;
	
	if( option_exists( "-h", argc, argv ) ){
		
		print_help( std::cout );
		return 0;
		
	} else if( option_exists( "-a", argc, argv ) ){
		
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
	
	// Open config file
	simple_ini_parser config;
	config.read( ( getenv("TERMJITEN_CONFIG") == NULL ? 
		( std::string( getenv("HOME") )+"/.config/termjiten.ini" )
		: getenv("TERMJITEN_CONFIG") ) );
	
	// check if colors spefied, else save hardcoded defaults in config
	config.map()["colors.kanji"] = "\e" + config.get( "colors.kanji", "[91m" );
	config.map()["colors.kana"] = "\e" + config.get( "colors.kana", "[94m" );
	config.map()["colors.trans"] = "\e" + config.get( "colors.trans", "[0m" );
	config.map()["colors.extra"] = "\e" + config.get( "colors.extra", "[92m" );
	config.map()["colors.reset"] = "\e" + config.get( "colors.reset", "[0m" );
	
	// Determine which dictionaries should be used, commandline > config
	bool use_jmdict = false, use_jmnedict = false, use_kanjidic = false;
	std::string sources = "";
	if( option_get_argument( "-s", sources, argc, argv ) ){
		
		if( sources.find("jmdict") != std::string::npos )
			use_jmdict = true;
		
		if( sources.find("jmnedict") != std::string::npos )
			use_jmnedict = true;
		
		if( sources.find("kanjidic") != std::string::npos )
			use_kanjidic = true;
		
	} else{
		// check config
		use_jmdict = string_to_bool( config.get( "jmdict.enable", "true" ), true );
		use_jmnedict = string_to_bool( config.get( "jmnedict.enable", "false" ), false );
		use_kanjidic = string_to_bool( config.get( "kanjidic.enable", "true" ), true );
	}
	
	// JMdict
	if( use_jmdict ){
		
		// Determine jmdict path, 1. config 2. default
		std::string jmdict_path = config.get( "jmdict.path", "/usr/share/termjiten/JMdict" );
		
		// Open JMdict
		dictionary_jmdict jmdict(jmdict_path);
		if( !jmdict.is_open() ){
			std::cerr << "Error: could not open " << jmdict_path << "\n";
			return 1;
		}
		
		// Perform search
		jmdict.search( query, method, std::cout, config.map() );
		
	}
	
	// JMnedict
	if( use_jmnedict ){
		
		// Determine jmnedict path, 1. config 2. default
		std::string jmnedict_path = config.get( "jmnedict.path", "/usr/share/termjiten/JMnedict.xml" );
		
		// Open JMdict
		dictionary_jmnedict jmnedict(jmnedict_path);
		if( !jmnedict.is_open() ){
			std::cerr << "Error: could not open " << jmnedict_path << "\n";
			return 1;
		}
		
		// Perform search
		jmnedict.search( query, method, std::cout, config.map() );
		
	}
	
	// kanjidic
	if( use_kanjidic ){
		
		// Determine kanjidic path, 1. config 2. default
		std::string kanjidic_path = config.get( "kanjidic.path", "/usr/share/termjiten/kanjidic2.xml" );
		
		// Open kanjidic
		dictionary_kanjidic kanjidic(kanjidic_path);
		if( !kanjidic.is_open() ){
			std::cerr << "Error: could not open " << kanjidic_path << "\n";
			return 1;
		}
		
		// Perform search
		kanjidic.search( query, method, std::cout, config.map() );
		
	}
	
	return 0;
}

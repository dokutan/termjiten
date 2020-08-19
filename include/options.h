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

/// Check if commandline option exists
bool option_exists( const std::string& option, int argc, char *argv[] ){
	
	for( int i = 1; i < argc; i++ ){
		
		if( strcmp( argv[i], option.c_str() ) == 0 )
			return true;
		
	}
	
	return false;
}


/** Check if commandline option and argument exists, get argument
 */
bool option_get_argument( const std::string& option, std::string& argument, int argc, char *argv[] ){
	
	for( int i = 1; i < argc; i++ ){
		
		if( strcmp( argv[i], option.c_str() ) == 0 && (i+1) < argc ){
			argument = argv[i+1];
			return true;
		}
		
	}
	
	return false;
}

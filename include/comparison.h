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
#include <regex>

/// Used to compare strings
class comparison{
	
	private:
		
		// method
		std::string comp_method;
	
	public:
		
		/// constructor
		comparison(){ comp_method = "fuzzy"; }
	
		/// constructor
		comparison( std::string method ){ comp_method = method; }
	
		// operator ()
		bool operator()( std::string base, std::string query ){
			
			// exact match
			if( comp_method == "exact" ){
				return (bool)(base == query);
			}
			
			// regex match
			else if( comp_method == "regex" ){ 
				return std::regex_match( base, std::regex( query, std::regex::optimize ) );
			}
			
			// fuzzy match
			else if( comp_method == "fuzzy" ){ 
				return std::regex_match( base, std::regex( ".*"+query+".*", std::regex::icase | std::regex::optimize ) );
			}
			
			// invalid method
			return false;
		}
};

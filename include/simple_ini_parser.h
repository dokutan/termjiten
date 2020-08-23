/*
 * This is a standalone .ini parser, originally written to replace boost property
 * tree in the mouse_m908 project. It is not intended to be a general
 * purpose replacement.
 * 
 * Dual licensed CC0 or 0BSD
 */

#ifndef SIMPLE_INI_PARSER
#define SIMPLE_INI_PARSER

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <map>

/**
 * This is a standalone .ini parser, originally written to replace boost property
 * tree in the mouse_m908 project. It is not intended to be a general
 * purpose replacement.
 */
class simple_ini_parser{
	
	private:
	
	/// Stores the key-value pairs
	std::map< std::string, std::string > _ini_values;
	
	public:
	
	/// \returns a reference to the key-value map
	std::map< std::string, std::string >& map(){ return _ini_values; }
	
	/// \returns a const reference to the key-value map
	const std::map< std::string, std::string >& const_map(){ return _ini_values; }
	
	/** 
	 * Read the specified .ini file. The already existing key-value
	 * pairs do not get cleared, only overwritten.
	 * \return 0 if succesful
	 */
	int read( std::string path );
	
	/// Check if the specified key exists
	bool has( std::string key );
	
	/**
	 * Get the value of the specified key.
	 * \return The value of the specified key, or the specified default
	 * value if the key is unknown
	 */
	std::string get( std::string key, std::string default_value );
	
	/// Print all key-value pairs to output.
	void print_all( std::ostream& output );
};

// read and parse ini file
int simple_ini_parser::read( std::string path ){
	
	// open file
	std::ifstream inifile;
	inifile.open( path );
	
	if( !inifile.is_open() )
		return 1;
	
	// go through each line
	std::string line, current_section = "";
	
	while( std::getline( inifile, line ) ){
		
		// empty line ?
		if( line.length() == 0 )
			continue;
		
		// comment ?
		if( line[0] == ';' || line[0] == '#' )
			continue;
		
		// remove whitespace
		line = std::regex_replace( line, std::regex("[[:space:]]"), "" );
		
		// section header?
		if( std::regex_match( line, std::regex("\\[[[:print:]]+\\]") ) ){
			current_section = std::regex_replace( line, std::regex("[\\[\\]]"), "" ) + ".";
			continue;
		}
		
		// key=value ?
		if( std::regex_match( line, std::regex("[[:print:]]+=[[:print:]]+") ) ){
			_ini_values.emplace( current_section + std::regex_replace( line, std::regex("=[[:print:]]+"), "" ),
				std::regex_replace( line, std::regex("[[:print:]]+="), "" ) );
		}
		
	}
	
	// close file
	inifile.close();
	
	return 0;
}

// key exists?
bool simple_ini_parser::has( std::string key ){
	
	if( _ini_values.find( key ) != _ini_values.end() )
		return true;
	
	return false;	
}

// get values
std::string simple_ini_parser::get( std::string key, std::string default_value ){
	
	// check if key exists
	if( _ini_values.find( key ) != _ini_values.end() )
		return _ini_values.at( key );
	
	return default_value;
}

// print all key-value pairs to output
void simple_ini_parser::print_all( std::ostream& output ){
	
	for( auto i : _ini_values )
		output << i.first << "=" << i.second << "\n";
	
}

#endif

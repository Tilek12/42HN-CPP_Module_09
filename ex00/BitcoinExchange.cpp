/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 19:54:39 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/16 20:32:43 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange( void ) {}

BitcoinExchange::BitcoinExchange( const BitcoinExchange& other ) : _database( other._database ) {}

BitcoinExchange&	BitcoinExchange::operator=( const BitcoinExchange& other ) {

	if ( this != &other )
		_database = other._database;

	return *this;
}

BitcoinExchange::~BitcoinExchange( void ) {}

bool	BitcoinExchange::_isValidDate( const std::string& date ) const {

	if ( date.length() != 10 || date[4] != '-' || date[7] != '-' )
		return false;

	int year, month, day;
	try {
		year = std::stoi( date.substr( 0, 4 ) );
		month = std::stoi( date.substr( 5, 2 ) );
		day = std::stoi( date.substr( 8, 2 ) );
	} catch (...) {
		return false;
	}

	if ( month < 1 || month > 12 || day < 1 || day > 31 )
		return false;

	return true;
}

bool	BitcoinExchange::_isValidValue( const std::string& value ) const {

	try {
		float val = std::stof( value );
		return val >= 0 && val <= 1000;
	} catch (...) {
		return false;
	}
}

void	BitcoinExchange::loadDatabase( const std::string& databaseFile ) {

	std::ifstream	file( databaseFile );
	if ( !file.is_open() )
		throw std::runtime_error( "ERROR: could not open file.");

	std::string	line;
	while ( std::getline( file, line ) ) {
		std::istringstream iss( line );
		std::string date;
		float rate;
		if ( std::getline( iss, date, ',' ) && iss >> rate )
			_database[date] = rate;
	}

	file.close();
}

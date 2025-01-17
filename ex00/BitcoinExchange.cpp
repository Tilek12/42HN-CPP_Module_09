/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 19:54:39 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/17 13:06:28 by tkubanyc         ###   ########.fr       */
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
		throw std::runtime_error( "Error: could not open file.");

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

std::string	BitcoinExchange::findClosestDate( const std::string& date ) const {

	auto it = _database.lower_bound( date );

	if ( it == _database.end() || it->first != date ) {
		if ( it == _database.begin() )
			return "";
		--it;
	}

	return it->first;
}

void	BitcoinExchange::evaluate( const std::string& inputFile ) const {

	std::ifstream file( inputFile );
	if ( !file.is_open() )
		throw std::runtime_error( "Error: could not open file." );

	std::string	line;
	while ( std::getline( file, line ) ) {
		std::istringstream iss( line );
		std::string date, valueStr;
		if ( std::getline( iss, date, '|' ) && std::getline( iss, valueStr ) ) {
			date = date.substr( 0, date.find_last_not_of( " \t" ) + 1 );
			valueStr = valueStr.substr( valueStr.find_first_not_of( " \t" ) );

			if ( !_isValidDate( date ) ) {
				std::cerr << "Error: bad input => " << date << std::endl;
				continue;
			}

			if ( !_isValidValue( valueStr ) ) {
				std::cerr << "Error: invalid value => " << valueStr << std::endl;
				continue;
			}

			float value = std::stof( valueStr );
			std::string closestDate = findClosestDate( date );
			if ( closestDate.empty() ) {
				std::cerr << "Error: no matching date were found for " << date << std::endl;
				continue;
			}

			float rate = _database.at( closestDate );
			std::cout << date << " => " << value << " = " << ( value * rate ) << std::endl;
		} else {
			std::cerr << "Error: bad input format => " << line << std::endl;
		}
	}

	file.close();
}

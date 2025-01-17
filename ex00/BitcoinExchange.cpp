/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 19:54:39 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/17 19:23:43 by tkubanyc         ###   ########.fr       */
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

	if ( date.size() != 10 || date[4] != '-' || date[7] != '-' )
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

std::string	BitcoinExchange::_checkValue( const std::string& valueStr ) const {

	try {
		float value = std::stof( valueStr );

		if ( value < 0 )
			return "Error: not a positive number.";

		if ( value > 1000 )
			return "Error: too large number.";

		return "valid";
	} catch (...) {
		return "Error: invalid value.";
	}
}

std::string	BitcoinExchange::_findClosestDate( const std::string& date ) const {

	auto it = _database.lower_bound( date );

	if ( it == _database.end() || ( it != _database.begin() && it->first != date ) )
		--it;

	if ( it == _database.end() )
			return "";

	return it->first;
}

void	BitcoinExchange::loadDatabase( const std::string& databaseFile ) {

	std::ifstream	file( databaseFile );
	if ( !file.is_open() )
		throw std::runtime_error( "Error: could not open data file.");

	std::string	line;

	if ( std::getline( file, line ) ) {
		if ( line != "date,exchange_rate" )
			throw std::runtime_error( "Error: invalid header in data file." );
	}

	while ( std::getline( file, line ) ) {
		std::istringstream stream( line );
		std::string date, rateStr;
		if ( std::getline( stream, date, ',' ) && std::getline( stream, rateStr ) ) {
			try {
				float rate = std::stof( rateStr );
				_database[date] = rate;
			} catch ( const std::exception& ) {
				throw std::runtime_error( "Error: invalid rate in database.");
			}
		}
	}

	file.close();
}

void	BitcoinExchange::evaluate( const std::string& inputFile ) const {

	std::ifstream file( inputFile );
	if ( !file.is_open() )
		throw std::runtime_error( "Error: could not open input file." );

	std::string	line;

	if ( std::getline( file, line ) ) {
		if ( line != "date | value" )
			throw std::runtime_error( "Error: invalid header in input file.");
	}

	while ( std::getline( file, line ) ) {
		std::istringstream stream( line );
		std::string date, valueStr;
		if ( std::getline( stream, date, '|' ) && std::getline( stream, valueStr ) ) {
			date.erase( date.find_last_not_of( " \t\n\r" ) + 1 );
			valueStr.erase( 0, valueStr.find_first_not_of( " \t\n\r" ) );

			if ( !_isValidDate( date ) ) {
				std::cerr << "Error: bad input => " << date << std::endl;
				continue;
			}

			std::string valueCheck = _checkValue( valueStr );
			if ( valueCheck != "valid" ) {
				std::cerr << valueCheck << std::endl;
				continue;
			}

			float value = std::stof( valueStr );
			std::string closestDate = _findClosestDate( date );
			if ( closestDate.empty() ) {
				std::cerr << "Error: no matching date were found for " << date << std::endl;
				continue;
			}

			float rate = _database.at( closestDate );
			float result = value * rate;
			std::cout << date << " => " << value << " = " << result << std::endl;
		} else {
			std::cerr << "Error: bad input => " << line << std::endl;
		}
	}

	file.close();
}

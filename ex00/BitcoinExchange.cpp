/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 19:54:39 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/12 17:19:13 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

/*-------------------------------------*/
/*  BitcoinExchange Class constructor  */
/*-------------------------------------*/
BitcoinExchange::BitcoinExchange( const std::string& databaseFile ) {
	_loadDatabase( databaseFile );
}

/*------------------------------------------*/
/*  BitcoinExchange Class copy constructor  */
/*------------------------------------------*/
BitcoinExchange::BitcoinExchange( const BitcoinExchange& other )
	: _database( other._database ) {}

/*--------------------------------------------------*/
/*  BitcoinExchange Class copy assignment operator  */
/*--------------------------------------------------*/
BitcoinExchange&	BitcoinExchange::operator=( const BitcoinExchange& other ) {

	if ( this != &other )
		_database = other._database;

	return *this;
}

/*------------------------------------*/
/*  BitcoinExchange Class destructor  */
/*------------------------------------*/
BitcoinExchange::~BitcoinExchange( void ) {}

/*-------------------------*/
/*  Define _trim function  */
/*-------------------------*/
std::string	BitcoinExchange::_trim( const std::string& str ) const {
	size_t start = str.find_first_not_of( " \t\n\r" );
	size_t end = str.find_last_not_of( " \t\n\r" );
	return ( start == std::string::npos ) ? "" : str.substr( start, end - start + 1 );
}

/*---------------------------------*/
/*  Define _loadDatabase function  */
/*---------------------------------*/
void	BitcoinExchange::_loadDatabase( const std::string& databaseFile ) {

	std::ifstream	file( databaseFile );
	if ( !file.is_open() )
		throw std::runtime_error( "Error: could not open data file." );

	std::string	line;
	if ( std::getline( file, line ) && line != "date,exchange_rate" )
		throw std::runtime_error( "Error: invalid header in data file." );

	while ( std::getline( file, line ) ) {
		std::istringstream	stream( line );
		std::string			date, rateStr;
		double				rate;

		if ( !std::getline( stream, date, ',' ) || !std::getline( stream, rateStr ) )
			throw std::runtime_error( "Error: invalid database format." );

		date = _trim( date );
		rateStr = _trim( rateStr );

		try {
			rate = std::stod( rateStr );
		} catch (...) {
			throw std::runtime_error( "Error: invalid rate in database." );
		}

		if ( rateStr.substr( 0, 1 ) == "-" || rate < 0 )
			throw std::runtime_error( "Error: negative rate in database." );

		if ( !_isValidDate( date ) )
			throw std::runtime_error( "Error: invalid date in database." );

		_database[date] = rate;
	}
}

/*--------------------------------*/
/*  Define _isValidDate function  */
/*--------------------------------*/
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

	if ( ( month == 4 || month == 6 || month == 9 || month == 11 ) && day > 30 )
		return false;

	if ( month == 2 ) {
		bool isLeap = ( year % 4 == 0 && year % 100 != 0 ) || ( year % 400 == 0 );
		if ( ( isLeap && day > 29 ) || ( !isLeap && day > 28 ) )
			return false;
	}

	return true;
}

/*-------------------------------*/
/*  Define _checkValue function  */
/*-------------------------------*/
std::string	BitcoinExchange::_checkValue( const std::string& valueStr ) const {

	try {
		double value = std::stod( valueStr );

		if ( value < 0 )
			return "Error: not a positive number.";

		if ( value > 1000 )
			return "Error: too large number.";

		return "valid";
	} catch (...) {
		return "Error: invalid value.";
	}
}

/*------------------------------------*/
/*  Define _findClosestDate function  */
/*------------------------------------*/
std::string	BitcoinExchange::_findClosestDate( const std::string& date ) const {

	auto it = _database.lower_bound( date );

	if ( it == _database.end() || ( it != _database.begin() && it->first != date ) )
		--it;

	return ( it == _database.begin() && it->first != date ) ? "" : it->first;
}

/*----------------------------*/
/*  Define evaluate function  */
/*----------------------------*/
void	BitcoinExchange::evaluate( const std::string& inputFile ) const {

	std::ifstream file( inputFile );
	if ( !file.is_open() )
		throw std::runtime_error( "Error: could not open input file." );

	std::string	line;
	if ( std::getline( file, line ) && line != "date | value" )
		throw std::runtime_error( "Error: invalid header in input file." );

	while ( std::getline( file, line ) ) {
		std::istringstream	stream( line );
		std::string			date, valueStr;

		if ( std::getline( stream, date, '|' ) && std::getline( stream, valueStr ) ) {
			date = _trim( date );
			valueStr = _trim( valueStr );

			if ( !_isValidDate( date ) ) {
				std::cerr << "Error: bad input => " << date << std::endl;
				continue;
			}

			std::string valueCheck = _checkValue( valueStr );
			if ( valueCheck != "valid" ) {
				std::cerr << valueCheck << std::endl;
				continue;
			}

			double value = std::stod( valueStr );
			std::string closestDate = _findClosestDate( date );
			if ( closestDate.empty() ) {
				std::cerr << "Error: no matching date for " << date << std::endl;
				continue;
			}

			double rate = _database.at( closestDate );
			std::cout << date << " => " << value << " = " << value * rate << std::endl;
		} else {
			std::cerr << "Error: bad input => " << line << std::endl;
		}
	}
}

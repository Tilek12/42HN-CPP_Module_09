#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>

class	BitcoinExchange {

private:

	std::map<std::string, float>	_database;

public:

	BitcoinExchange( void );
	BitcoinExchange( const BitcoinExchange& other );
	BitcoinExchange&	operator=( const BitcoinExchange& other );
	~BitcoinExchange( void );

	void		loadDatabase( const std::string& databaseFile );
	void		evaluate( const std::string& inputFile ) const;
	std::string	findClosestDate( const std::string& date ) const;

};

#endif

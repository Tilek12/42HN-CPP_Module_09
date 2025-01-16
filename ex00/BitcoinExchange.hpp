/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 19:54:48 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/16 20:32:45 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class	BitcoinExchange {

private:

	std::map<std::string, float>	_database;

	bool	_isValidDate( const std::string& date ) const;
	bool	_isValidValue( const std::string& value ) const;

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

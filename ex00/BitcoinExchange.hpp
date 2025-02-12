/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 19:54:48 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/12 09:51:31 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

/*--------------------------------*/
/*  Define BitcoinExchange class  */
/*--------------------------------*/
class	BitcoinExchange {

private:

	std::map<std::string, double>	_database;

	std::string	_trim( const std::string& str ) const;
	void		_loadDatabase( const std::string& databaseFile );
	bool		_isValidDate( const std::string& date ) const;
	std::string	_checkValue( const std::string& valueStr ) const;
	std::string	_findClosestDate( const std::string& date ) const;

public:

	BitcoinExchange( const std::string& databaseFile );
	BitcoinExchange( const BitcoinExchange& other );
	BitcoinExchange&	operator=( const BitcoinExchange& other );
	~BitcoinExchange( void );

	void	evaluate( const std::string& inputFile ) const;

};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:22:09 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/12 09:52:05 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

/*-----------------*/
/*  Main function  */
/*-----------------*/
int	main( int argc, char** argv ) {

	if ( argc != 2 ) {
		std::cerr << "Usage: ./btc <input.txt>" << std::endl;
		return 1;
	}

	try {
		BitcoinExchange	btc( "data.csv" );
		btc.evaluate( argv[1] );
	} catch ( const std::exception& e ) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

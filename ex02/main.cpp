/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 20:27:30 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/28 18:49:29 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int	main( int argc, char** argv ) {

	if ( argc < 2 ) {
		std::cerr << "Error: No input provided." << std::endl << std::endl
				  << "Usage: " << argv[0] << " <numbers to sort>" << std::endl;
		return 1;
	}

	try {
		PmergeMe	pmergeMe;

		pmergeMe.parseInput( argc, argv );
		pmergeMe.sortData();
	} catch ( const std::exception& e ) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

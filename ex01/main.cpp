/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 20:51:30 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/18 20:55:36 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int	main( int argc, char* argv[] ) {

	if ( argc != 2 ) {
		std::cerr << "Error: Invalid number of arguments." << std::endl;
		return 1;
	}

	try {
		RPN	rpn;
		int result = rpn.calculate( argv[1] );
		std::cout << result << std::endl;
	} catch ( const std::exception& e ) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

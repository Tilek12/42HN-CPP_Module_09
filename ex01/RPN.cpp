/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:45:20 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/18 20:08:32 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN( void ) {}

RPN::RPN( const RPN& other ) : _stack( other._stack ) {}

RPN&	RPN::operator=( const RPN& other ) {

	if ( this != &other )
		_stack = other._stack;

	return *this;
}

RPN::~RPN( void ) {}

int	RPN::calculate( const std::string& expression ) {

}

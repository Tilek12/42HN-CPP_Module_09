/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:45:20 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/12 09:54:54 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

/*-------------------------*/
/*  RPN Class constructor  */
/*-------------------------*/
RPN::RPN( void ) {}

/*------------------------------*/
/*  RPN Class copy constructor  */
/*------------------------------*/
RPN::RPN( const RPN& other ) : _stack( other._stack ) {}

/*--------------------------------------*/
/*  RPN Class copy assignment operator  */
/*--------------------------------------*/
RPN&	RPN::operator=( const RPN& other ) {

	if ( this != &other )
		_stack = other._stack;

	return *this;
}

/*------------------------*/
/*  RPN Class destructor  */
/*------------------------*/
RPN::~RPN( void ) {}

/*-----------------------------*/
/*  Define calculate function  */
/*-----------------------------*/
int	RPN::calculate( const std::string& expression ) {

	std::istringstream	tokens( expression );
	std::string			token;

	while ( tokens >> token ) {
		if ( std::isdigit( token[0] ) && token.size() == 1 ) {
			_stack.push( std::stoi( token ) );
		} else if ( token == "+" || token == "-" || token == "*" || token == "/") {
			if ( _stack.size() < 2 )
				throw std::invalid_argument( "Malformed expression." );

			int b = _stack.top(); _stack.pop();
			int a = _stack.top(); _stack.pop();

			if ( token == "+" ) _stack.push( a + b );
			else if ( token == "-" ) _stack.push( a - b );
			else if ( token == "*" ) _stack.push( a * b );
			else if ( token == "/" ) {
				if ( b == 0 ) throw std::invalid_argument( "Division by zero." );
				_stack.push( a / b );
			}
		} else {
			throw std::invalid_argument( "Invalid token in expression." );
		}
	}

	if ( _stack.size() != 1 )
		throw std::invalid_argument( "Malformed expression." );

	return _stack.top();
}

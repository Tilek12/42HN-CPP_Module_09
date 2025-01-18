/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:36:39 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/18 19:43:35 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <exception>

class	RPN {

private:

	std::stack<int>	_stack;

public:

	RPN( void );
	RPN( const RPN& other );
	RPN&	operator=( const RPN& other );
	~RPN( void );

	int	calculate( const std::string& expression );

};

#endif

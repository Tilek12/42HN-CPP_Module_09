/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:17:03 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/19 20:17:54 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <chrono>

class	PmergeMe {

private:

	std::vector<int>	_vectorData;
	std::deque<int>		_dequeData;

	template <typename Container>
	void	_sortPairs( Container& data, Container& pairs );

	template <typename Container>
	void	_insertLargerElements( Container& sorted, const Container& larger );

	template <typename Container>
	void	_sortFordJohnson( Container& data );

public:

	PmergeMe( void );
	PmergeMe( const PmergeMe& other );
	PmergeMe&	operator=( const PmergeMe& other );
	~PmergeMe( void );

	void	parseInput( int argc, char** argv );
	void	sortData( void );

};

#endif

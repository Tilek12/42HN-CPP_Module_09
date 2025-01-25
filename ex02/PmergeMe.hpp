/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:17:03 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/25 13:09:03 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iterator>
#include <limits>
#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <chrono>
#include <cmath>

class	PmergeMe {

private:

	// template <typename Container>
	// void	_sortPairs( Container& data, Container& smaller, Container& larger );

	// template <typename Container>
	// void	_insertLargerElements( Container& sorted, const Container& larger );

	size_t				_comparisonCounter = 0;
	std::vector<int>	_vectorData;
	std::deque<int>		_dequeData;

	size_t							_countPossibleComparisons( size_t numElements ) const;

	long							_numberJacobsthal( long n );

	template <typename Container>
	typename Container::iterator	_nextIterator( typename Container::iterator it, int steps );

	template <typename Container>
	void							_swapPair( typename Container::iterator it, int pairLevel );

	template <typename Container>
	void							_sortFordJohnson( Container& data, int pairLevel );


public:

	PmergeMe( void );
	PmergeMe( const PmergeMe& other );
	PmergeMe&	operator=( const PmergeMe& other );
	~PmergeMe( void );

	void	parseInput( int argc, char** argv );
	void	sortData( void );

};

#endif

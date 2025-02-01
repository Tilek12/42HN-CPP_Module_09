/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:17:03 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/01 19:22:28 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

enum	Prefix { BEFORE, AFTER };

class	PmergeMe {

private:

	size_t				_comparisonCounter;
	std::vector<int>	_vectorData;
	double				_vectorTime;
	std::deque<int>		_dequeData;
	double				_dequeTime;

	size_t							_countMinComparisons( size_t n ) const;

	long							_numberJacobsthal( long n );

	template <typename Container>
	void							_printElements( Container& data ) const;

	void							_printComparisons( void ) const;

	void							_printResult( Prefix prefix ) const;


	template <typename Container>
	void							_swapPair( typename Container::iterator it, int pairLevel );

	template <typename Iter>
	static bool						_compare( Iter left, Iter right );

	template <typename Container>
	void							_sortPairs( typename Container::iterator start,
											typename Container::iterator end, int pairLevel );

	// template <typename Container>
	// typename Container::iterator	_binarySearchInsertPosition(Container& mainChain,
    //                                   							const typename Container::value_type& target,
    //                                   							typename Container::iterator bound);

	template <typename Container>
	void							_insertion( Container& data, typename Container::iterator end,
												int pairLevel, size_t elementsNum );

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

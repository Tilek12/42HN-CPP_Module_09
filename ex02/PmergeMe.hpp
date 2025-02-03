/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:17:03 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/03 21:36:46 by tkubanyc         ###   ########.fr       */
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

	size_t							_jacobsthal( size_t n );

	template <typename Container>
	void							_printElements( Container& data ) const;

	void							_printComparisons( void ) const;

	void							_printResult( Prefix prefix ) const;


	template <typename Container>
	void							_swapPair( typename Container::iterator it, size_t elementSize );

	template <typename Iter>
	static bool						_compare( Iter left, Iter right );

	template <typename Container>
	void							_sortPairs( typename Container::iterator start,
											typename Container::iterator end, size_t elementSize );

	// template <typename Container>
	// void 							binaryInsertToMainChain(typename Container::iterator elemToInsert,
    //                                    Container& mainChain,
    //                                    size_t elementSize);

	// void							calculateNextJacobsthal(size_t& currJacobsthal, size_t& prevJacobsthal);

	template <typename Container>
	void							_binarySearchInsert(Container& dst, const Container& src,
											typename Container::const_iterator srcIt, size_t elementSize);

	template <typename Container>
	void							_insertion( Container& data, typename Container::iterator end,
												size_t elementSize, size_t elementsNum );

	template <typename Container>
	void							_sortFordJohnson( Container& data, size_t elementSize );

public:

	PmergeMe( void );
	PmergeMe( const PmergeMe& other );
	PmergeMe&	operator=( const PmergeMe& other );
	~PmergeMe( void );

	void	parseInput( int argc, char** argv );
	void	sortData( void );

};

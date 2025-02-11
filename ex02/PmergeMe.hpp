/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:17:03 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/08 18:52:19 by tkubanyc         ###   ########.fr       */
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

	size_t		_countPossibleComparisons( size_t n ) const;

	template <typename Container>
	void		_printElements( Container& data ) const;

	void		_printComparisons( void ) const;

	void		_printResult( Prefix prefix ) const;

	void		_updateJacobsthal( size_t& currJacobsthal, size_t& prevJacobsthal );

	template <typename Container>
	size_t		_findElementIndex( Container& dst, Container& src,
									size_t srcIndex, size_t elementSize );

	template <typename Container>
	void		_binarySearchInsert( Container& dst, size_t dstEnd, const Container& src,
									typename Container::const_iterator srcIt, size_t elementSize );

	template <typename Container>
	bool		_isExist(const Container& container, size_t elementIndex, size_t elementSize);

	template <typename Container>
	void		_insertElements( Container& mainChain, Container& largers, Container& smallers,
									typename Container::iterator smallCurrIt,
									size_t elementsToInsert, size_t elementSize );

	template <typename Container>
	void		_defineContainers( Container& data, size_t elementsNum, size_t elementSize,
									Container& mainChain, Container& largers, Container& smallers );

	template <typename Container>
	void		_sortInsertion( Container& data, size_t elementsNum, size_t elementSize );

	template <typename Container>
	void		_sortPairs( typename Container::iterator start,
							typename Container::iterator end, size_t elementSize );

	template <typename Container>
	void		_sortFordJohnson( Container& data, size_t elementSize );

public:

	PmergeMe( void );
	PmergeMe( const PmergeMe& other );
	PmergeMe&	operator=( const PmergeMe& other );
	~PmergeMe( void );

	void	parseInput( int argc, char** argv );
	void	sortData( void );

};

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:17:03 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/12 09:57:46 by tkubanyc         ###   ########.fr       */
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

/*----------------------*/
/*  Define Prefix enum  */
/*----------------------*/
enum	Prefix { BEFORE, AFTER };

/*-----------------------*/
/*  Define Index struct  */
/*-----------------------*/
struct	Index {
	size_t	constIndex;
	size_t	actualIndex;

	Index( size_t constant, size_t actual ) : constIndex( constant ),
											  actualIndex( actual ) {}
};

/*-------------------------*/
/*  Define PmergeMe class  */
/*-------------------------*/
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

	void		_updateIndexes( std::vector<Index>& dst , size_t insertIndex );

	template <typename Container>
	size_t		_binarySearchInsert( Container& dst, size_t dstEnd, const Container& src,
									typename Container::const_iterator srcIt,
									size_t elementSize );

	size_t		_findElementIndex( const std::vector<Index>& dst, size_t searchIndex );

	bool		_isExist( const std::vector<Index>& dst, size_t searchIndex);

	template <typename Container>
	void		_insertElements( Container& mainChain, Container& smallers,
								 std::vector<Index>& largerIndexes,
								 typename Container::iterator smallCurrIt,
								 size_t elementsToInsert, size_t elementSize );

	template <typename Container>
	void		_defineContainers( Container& data, size_t elementsNum, size_t elementSize,
									Container& mainChain,
									std::vector<Index>& largerIndexes,
									Container& smallers );

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

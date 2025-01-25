/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/25 12:34:55 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe( void ) {}

PmergeMe::PmergeMe( const PmergeMe& other ) : _vectorData( other._vectorData ),
											  _dequeData( other._dequeData ) {}

PmergeMe&	PmergeMe::operator=( const PmergeMe& other ) {

	if ( this != &other ) {
		_vectorData = other._vectorData;
		_dequeData = other._dequeData;
	}

	return *this;
}

PmergeMe::~PmergeMe( void ) {}

size_t	PmergeMe::_countPossibleComparisons( size_t numElements ) const {
	return ( numElements * ( numElements - 1 ) ) / 2;
}

// template <typename Container>
// void	PmergeMe::_sortPairs( Container& data, Container& smaller, Container& larger ) {

// 	for ( size_t i = 0; i + 1 < data.size(); i += 2 ) {
// 		if ( data[i] > data[i + 1] ) {
// 			smaller.push_back( data[i + 1] );
// 			larger.push_back( data [i] );
// 		} else {
// 			smaller.push_back( data[i] );
// 			larger.push_back( data[i + 1] );
// 		}
// 	}

// 	if ( data.size() % 2 != 0 )
// 		smaller.push_back( data.back() );

// 	std::sort( smaller.begin(), smaller.end() );
// }

// template <typename Container>
// void	PmergeMe::_insertLargerElements( Container& sorted, const Container& larger ) {

// 	for ( size_t i = 0; i < larger.size(); i++ ) {
// 		auto pos = std::upper_bound( sorted.begin(), sorted.end(), larger[i] );
// 		sorted.insert( pos, larger[i] );
// 	}
// }

template <typename Container>
typename Container::iterator	_nextIterator( typename Container::iterator it, int steps ) {

	std::advance( it, steps );
	return it;
}

template <typename Container>
void	_swapPair( typename Container::iterator it, int pairLevel ) {

	auto start = _nextIterator<Container>( it, -pairLevel + 1 );
	auto end = nextIterator<Container>( start. pairLevel );

	while ( start != end ) {
		std::iter_swap( start, _nextIterator<Container>( start, pairLevel ) );
		start++;
	}
}

template <typename Container>
void	PmergeMe::_sortFordJohnson( Container& data, int pairLevel ) {

	using	iterator = typename Container::iterator;

	size_t	pairUnits = data.size() / pairlevel;
	if ( pairUnits < 2 ) return;

	bool	isOdd = pairUnits % 2 == 1;

	iterator start = container.begin();
	iterator end = nextIterator( data, pairLevel * pairUnits - ( isOdd ? pairLevel : 0 ) );

	for ( iterator it = start; it != end; std::advance( it, 2 * pairLevel ) ) {
		iterator thisPair = nextIterator( data, pairLevel - 1 );
		iterator nextPair = nextIterator( data, pairLevel * 2 - 1 );

		if ( *thisPair > * nextPair ) {
			_comparisonCounter++;
			swapPair<Container>( thisPair, pairLevel );
		}
	}

	_sortFordJohnson( data, pairLevel * 2 );
}

void	PmergeMe::parseInput( int argc, char** argv ) {

	for ( int i = 1; i < argc; i++ ) {
		int					value;
		std::istringstream	iss( argv[i] );

		if ( !( iss >> value ) || value <= 0 )
			throw std::invalid_argument( "Error: Invalid input. \
				Only positive integers are allowed to use." );

		if ( !( iss >> value ) || value > std::numeric_limits<int>::max() )
			throw std::out_of_range( "Error: Invalid input. \
				Out of int range number is detected." );

		_vectorData.push_back( value );
		_dequeData.push_back( value );
	}

	std::cout << "\n------------------------------------\n\n";
	std::cout << "Before: ";
	for ( int i = 1; i < argc; i++ )
		std::cout << argv[i] << " ";
	std::cout << std::endl;
}

void	PmergeMe::sortData( void ) {

	double	vectorTime;
	double	dequeTime;

	auto start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _vectorData, 1 );
	auto end = std::chrono::high_resolution_clock::now();
	vectorTime = std::chrono::duration<double, std::micro>(end - start).count();

	start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _dequeData, 1 );
	end = std::chrono::high_resolution_clock::now();
	dequeTime = std::chrono::duration<double, std::micro>(end - start).count();

	std::cout << "\n------------------------------------\n\n";
	std::cout << "After: ";
	for ( const auto& num : _vectorData ) std::cout << num << " ";
	std::cout << std::endl;

	std::cout << "\n------------------------------------\n\n";
	std::cout << "Time to process a range of " << _vectorData.size()
			  << " elements with std::vector: " << vectorTime << " us" << std::endl;

	std::cout << "Time to process a range of " << _dequeData.size()
			  << " elements with std::deque: " << dequeTime << " us" << std::endl;

	std::cout << "\n------------------------------------\n\n";
}

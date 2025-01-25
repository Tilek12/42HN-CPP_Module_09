/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/25 13:09:29 by tkubanyc         ###   ########.fr       */
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
typename Container::iterator	PmergeMe::_nextIterator( typename Container::iterator it, int steps ) {

	std::advance( it, steps );
	return it;
}

template <typename Container>
void	PmergeMe::_swapPair( typename Container::iterator it, int pairLevel ) {

	auto start = _nextIterator<Container>( it, -pairLevel + 1 );
	auto end = _nextIterator<Container>( start. pairLevel );

	while ( start != end ) {
		std::iter_swap( start, _nextIterator<Container>( start, pairLevel ) );
		start++;
	}
}

long	PmergeMe::_numberJacobsthal( long n ) {
	return std::round( ( std::pow( 2, n + 1 ) + std::pow( -1, n ) ) / 3 );
}

template <typename Container>
void	PmergeMe::_sortFordJohnson( Container& data, int pairLevel ) {

	using	iterator = typename Container::iterator;

	size_t	pairUnits = data.size() / pairlevel;
	if ( pairUnits < 2 ) return;

	bool	isOdd = pairUnits % 2 == 1;

	iterator start = container.begin();
	iterator end = _nextIterator( data, pairLevel * pairUnits - ( isOdd ? pairLevel : 0 ) );

	for ( iterator it = start; it != end; std::advance( it, 2 * pairLevel ) ) {
		iterator thisPair = _nextIterator( data, pairLevel - 1 );
		iterator nextPair = _nextIterator( data, pairLevel * 2 - 1 );

		if ( *thisPair > * nextPair ) {
			_comparisonCounter++;
			_swapPair<Container>( thisPair, pairLevel );
		}
	}

	_sortFordJohnson( data, pairLevel * 2 );

	std::vector<iterator>	mainChain;
	std::vector<iterator>	pending;

	mainChain.push_back( _nextIterator( data, pairLevel -1 ) );
	mainChain.push_back( _nextIterator( data, pairLevel * 2 -1 ) );

	for ( size_t i = 4; i <= pairUnits; i += 2 ) {
		pending.push_back( _nextIterator( data, pairLevel * ( i - 1 ) - 1 ) );
		mainChain.push_back( _nextIterator( data, pairLevel * i - 1 ) );
	}

	int prevJacobsthalNum = _numberJacobsthal( 1 );
	int inserted = 0;

	for ( int j = 2;; j++ ) {
		int currJacobsthalNum = _numberJacobsthal( j );
		int diff = currJacobsthalNum - prevJacobsthalNum;

		if ( diff > static_cast<int>(pending.size() ) ) break;

		auto bound = mainChain.begin() + currJacobsthalNum + inserted;

		for ( int k = 0; k < diff; k++ ) {
			auto idx = std::upper_bound( mainChain.begin(), bound, *pending[k], [](auto lhs, auto rhs ) {
				return *lhs < *rhs;
			} );
			mainChain.insert( idx, pending[k] );
			inserted += diff;
		}

		prevJacobsthalNum = currJacobsthalNum;
		inserted += diff;
	}

	while ( !pending.empty() ) {
		auto idx = std::upper_bound( mainChain.begin(), mainChain.end(), *pending.back(), []( autolhs, auto rhs ) {
			return *lhs < *rhs;
		} );

		mainChain.insert( idx, pending.back() );
		pending.pop_back();
		_comparisonCounter++;
	}

	size_t i = 0;
	for ( auto it : mainChain )
		data[i++] = *it;

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

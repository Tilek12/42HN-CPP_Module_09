/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/28 18:54:04 by tkubanyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe( void ) : _comparisonCounter( 0 ),
							 _vectorTime( 0.0 ),
							 _dequeTime( 0.0 ) {}

PmergeMe::PmergeMe( const PmergeMe& other ) : _comparisonCounter( other._comparisonCounter ),
											  _vectorData( other._vectorData ),
											  _vectorTime( other._vectorTime ),
											  _dequeData( other._dequeData ),
											  _dequeTime( other._dequeTime ) {}

PmergeMe&	PmergeMe::operator=( const PmergeMe& other ) {

	if ( this != &other ) {
		_comparisonCounter = other._comparisonCounter;
		_vectorData = other._vectorData;
		_vectorTime = other._vectorTime;
		_dequeData = other._dequeData;
		_dequeTime = other._dequeTime;
	}

	return *this;
}

PmergeMe::~PmergeMe( void ) {}

size_t	PmergeMe::_countPossibleComparisons( size_t numElements ) const {
	return ( numElements * ( numElements - 1 ) ) / 2;
}

long	PmergeMe::_numberJacobsthal( long n ) {
	return std::round( ( std::pow( 2, n + 1 ) + std::pow( -1, n ) ) / 3 );
}

template <typename Container>
void	PmergeMe::_printElements( Container& data ) {

	for ( const auto& num : data ) std::cout << num << " ";
	std::cout << std::endl;
	std::cout << "\n------------------------------------\n\n";
}

void	PmergeMe::_printComparisons( void ) {

	std::cout << "\n------------------------------------\n\n";
	std::cout << "Possible comparisons for " << _vectorData.size()
			  << " elements: " << _countPossibleComparisons( _vectorData.size() )
			  << std::endl;

	std::cout << "Comparisons made: " << _comparisonCounter << std::endl;
	std::cout << "\n------------------------------------\n\n";
}

void	PmergeMe::_printResult( Prefix prefix ) {

	switch ( prefix ) {

		case BEFORE:
			std::cout << "\n------------------------------------\n\n";
			std::cout << "Before: ";
			_printElements( _vectorData );
			break;

		case AFTER:
			std::cout << "After: ";
			_printElements( _vectorData );
			std::cout << "Time to process a range of " << _vectorData.size()
					  << " elements with std::vector: " << _vectorTime << " us"
					  << std::endl;
			std::cout << "Time to process a range of " << _dequeData.size()
					  << " elements with std::deque: " << _dequeTime << " us"
					  << std::endl;
			_printComparisons();
			break;

		default:
			std::cerr << "Error: Incorrect prefix";
			break;
	}
}

template <typename Container>
typename Container::iterator	PmergeMe::_nextIterator( typename Container::iterator it, int steps ) {

	std::advance( it, steps );
	return it;
}

template <typename Container>
void	PmergeMe::_swapPair( typename Container::iterator it, int pairLevel ) {

	auto start = _nextIterator<Container>( it, -pairLevel + 1 );
	auto end = _nextIterator<Container>( start, pairLevel );

	while ( start != end ) {
		std::iter_swap( start, _nextIterator<Container>( start, pairLevel ) );
		start++;
	}
}

template <typename Container>
void	PmergeMe::_sortFordJohnson( Container& data, int pairLevel ) {

//////////////////////////////////////////////////////////////////////
	std::cout << "Vector Container: ";
	for ( const auto& num : _vectorData ) std::cout << num << " ";
	std::cout << std::endl;

	std::cout << "Deque Container: ";
	for ( const auto& num : _dequeData ) std::cout << num << " ";
	std::cout << std::endl;
	std::cout << std::endl;
//////////////////////////////////////////////////////////////////////

	typedef typename Container::iterator Iterator;

	size_t	pairUnits = data.size() / pairLevel;
	if ( pairUnits < 2 ) return;

	bool	isOdd = pairUnits % 2 == 1;

	Iterator start = data.begin();
	Iterator end = _nextIterator<Container>( start, pairLevel * pairUnits - ( isOdd ? pairLevel : 0 ) );

	for ( Iterator it = start; it != end; std::advance( it, 2 * pairLevel ) ) {
		Iterator thisPair = _nextIterator<Container>( it, pairLevel - 1 );
		Iterator nextPair = _nextIterator<Container>( it, pairLevel * 2 - 1 );

		if ( *thisPair > * nextPair ) {
			_comparisonCounter++;
			_swapPair<Container>( thisPair, pairLevel );
		}
	}

	_sortFordJohnson( data, pairLevel * 2 );

	std::vector<Iterator>	mainChain;
	std::vector<Iterator>	pending;

	mainChain.push_back( _nextIterator<Container>( data.begin(), pairLevel -1 ) );
	mainChain.push_back( _nextIterator<Container>( data.begin(), pairLevel * 2 -1 ) );

	for ( size_t i = 4; i <= pairUnits; i += 2 ) {
		pending.push_back( _nextIterator<Container>( data.begin(), pairLevel * ( i - 1 ) - 1 ) );
		mainChain.push_back( _nextIterator<Container>( data.begin(), pairLevel * i - 1 ) );
	}

	int prevJacobsthalNum = _numberJacobsthal( 1 );
	int inserted = 0;

	for ( int j = 2;; j++ ) {
		int currJacobsthalNum = _numberJacobsthal( j );
		int diff = currJacobsthalNum - prevJacobsthalNum;

		if ( diff > static_cast<int>(pending.size() ) ) break;

		auto bound = mainChain.begin() + currJacobsthalNum + inserted;

		for ( int k = 0; k < diff; k++ ) {
			auto idx = std::upper_bound( mainChain.begin(), bound, pending[k], []( Iterator lhs, Iterator rhs ) {
				return *lhs < *rhs;
			} );
			mainChain.insert( idx, pending[k] );
			inserted += diff;
		}

		prevJacobsthalNum = currJacobsthalNum;
		inserted += diff;
	}

	while ( !pending.empty() ) {
		auto idx = std::upper_bound( mainChain.begin(), mainChain.end(), pending.back(), []( Iterator lhs, Iterator rhs ) {
			return *lhs < *rhs;
		} );

		mainChain.insert( idx, pending.back() );
		pending.pop_back();
		_comparisonCounter++;
	}

	// size_t i = 0;
	// for ( auto it : mainChain )
	// 	data[i++] = *it;

	std::vector<int> copy;
	copy.reserve( data.size() );
	for ( auto it : mainChain ) {
		for ( int i = 0; i < pairLevel; i++ ) {
			Iterator pairStart = it;
			std::advance( pairStart, -pairLevel + i + 1 );
			copy.push_back( *pairStart );
		}
	}

	std::copy( copy.begin(), copy.end(), data.begin() );

}

void	PmergeMe::parseInput( int argc, char** argv ) {

	for ( int i = 1; i < argc; i++ ) {
		int					value;
		std::istringstream	iss( argv[i] );

		if ( !( iss >> value ) || value <= 0 )
			throw std::invalid_argument( "Error: Invalid input. \
				Only positive integers are allowed to use." );

		// if ( !( iss >> value ) || value > std::numeric_limits<int>::max() )
		// 	throw std::out_of_range( "Error: Invalid input. \
		// 		Out of int range number is detected." );

		_vectorData.push_back( value );
		_dequeData.push_back( value );
	}
}

void	PmergeMe::sortData( void ) {

	_printResult( BEFORE );

	auto start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _vectorData, 1 );
	auto end = std::chrono::high_resolution_clock::now();
	_vectorTime = std::chrono::duration<double, std::micro>(end - start).count();

	start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _dequeData, 1 );
	end = std::chrono::high_resolution_clock::now();
	_dequeTime = std::chrono::duration<double, std::micro>(end - start).count();

	_printResult( AFTER );
}

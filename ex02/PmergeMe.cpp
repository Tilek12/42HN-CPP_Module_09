/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/11 22:46:09 by tkubanyc         ###   ########.fr       */
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

size_t	PmergeMe::_countPossibleComparisons( size_t n ) const {

	size_t result = 0;

	for ( size_t k = 2; k <= n; ++k )
		result += static_cast<size_t>(std::ceil( std::log2( 3 * k / 4.0 ) ));

	return result;
}

template <typename Container>
void	PmergeMe::_printElements( Container& data ) const {

	for ( const auto& num : data ) std::cout << num << " ";
	std::cout << std::endl;
	std::cout << "\n---------------------------------------------\n\n";
}

void	PmergeMe::_printComparisons( void ) const {

	std::cout << "\n---------------------------------------------\n\n";
	std::cout << "Possible comparisons for " << _vectorData.size()
			  << " elements: " << _countPossibleComparisons( _vectorData.size() )
			  << std::endl;

	std::cout << "Comparisons made: " << _comparisonCounter << std::endl;
	std::cout << "\n---------------------------------------------\n\n";
}

void	PmergeMe::_printResult( Prefix prefix ) const {

	switch ( prefix ) {

		case BEFORE:
			std::cout << "\n---------------------------------------------\n\n";
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
			if ( std::is_sorted( _vectorData.begin(), _vectorData.end() )
				&& std::is_sorted( _dequeData.begin(), _dequeData.end() )) {

				std::cout << "--------------- !!! SORTED !!! --------------"
						  << std::endl << std::endl;
			}
			break;

		default:
			std::cerr << "Error: Incorrect prefix";
			break;
	}
}

void	PmergeMe::_updateJacobsthal( size_t& currJacobsthal, size_t& prevJacobsthal ) {

	size_t nextJacobsthal = currJacobsthal + 2 * prevJacobsthal;
	prevJacobsthal = currJacobsthal;
	currJacobsthal = nextJacobsthal;
}

void	PmergeMe::_updateIndexes( std::vector<Index>& dst , size_t insertPos ) {

	for ( auto& index : dst ) {
		if ( index.actualIndex >= insertPos )
			index.actualIndex = index.actualIndex + 1;
	}
}

template <typename Container>
size_t	PmergeMe::_binarySearchInsert( Container& dst, size_t dstEnd, const Container& src,
									typename Container::const_iterator srcIt, size_t elementSize ) {
	// Validate input parameters
	if ( elementSize == 0 || dst.empty() || src.empty() || dstEnd > dst.size() / elementSize ||
		srcIt < src.begin() || srcIt + elementSize > src.end() )
		throw std::out_of_range( "Error: Invalid input parameters!" );

	// Define the insertion element
	int insertValue = *( srcIt + elementSize - 1 );
	auto srcStart = srcIt;
	auto srcEnd = srcStart + elementSize;

	int low = 0;
	int high = static_cast<int>(dstEnd);

	// Perform binary search for the insertion position
	while ( low <= high ) {
		int mid = low + ( high - low ) / 2;
		int dstElemIndex = mid * static_cast<int>(elementSize) + static_cast<int>(elementSize - 1);

		// Check bounds
		if ( dstElemIndex >= static_cast<int>(dst.size()) )
			throw std::out_of_range( "Error: dstElemIndex out of bounds!" );

		int dstElemValue = dst[dstElemIndex];
		if ( dstElemValue < insertValue )
			low = mid + 1;
		else
			high = mid - 1;

		_comparisonCounter++;
	}

	int insertPosition = low * static_cast<int>(elementSize);

	// Ensure insertPosition is within bounds
	if ( insertPosition > static_cast<int>(dst.size()) )
		throw std::out_of_range( "Error: insertPosition out of bounds!" );

	// Insert the element into the container
	dst.insert( dst.begin() + insertPosition, srcStart, srcEnd );

	return static_cast<size_t>(low);
}

// template <typename Container>
size_t	PmergeMe::_findElementIndex( const std::vector<Index>& dst, size_t searchIndex ) {

	for ( const auto& index : dst ) {
		if ( index.constIndex == searchIndex )
			return index.actualIndex;
	}

	// If the element is not found, throw an exception
	throw std::runtime_error( "Error: Element not found in dst!" );
}

bool	PmergeMe::_isExist( const std::vector<Index>& dst, size_t searchIndex ) {

	for ( const auto& index : dst ) {
		if ( index.constIndex == searchIndex )
			return true;
	}

	return false;
}

template <typename Container>
void	PmergeMe::_insertElements( Container& mainChain, Container& smallers,
									std::vector<Index>& largerIndexes,
									typename Container::iterator insertEnd,
									size_t elementsToInsert, size_t elementSize ) {

	size_t	searchIndex;
	size_t	lastIndex;
	size_t	insertIndex;

	for ( size_t i = 0; i < elementsToInsert; i++ ) {
		insertEnd -= elementSize;
		searchIndex = std::distance( smallers.begin(), insertEnd ) / elementSize;

		// Define the last last element in searching range
		if ( _isExist( largerIndexes, searchIndex ) )
			lastIndex = _findElementIndex( largerIndexes, searchIndex ) - 1;
		else
			lastIndex = ( mainChain.size() / elementSize ) - 1;

		// Insert element using binary search and return the insertion index
		insertIndex = _binarySearchInsert( mainChain, lastIndex, smallers, insertEnd, elementSize );

		// Update actual indexes in largerIndexes after insertion
		_updateIndexes( largerIndexes, insertIndex );
	}
}

template <typename Container>
void	PmergeMe::_defineContainers( Container& data, size_t elementsNum, size_t elementSize,
									 Container& mainChain,
									 std::vector<Index>& largerIndexes,
									 Container& smallers ) {

	auto iter = data.begin();

	Container	largers;

	for ( size_t i = 0; i < elementsNum / 2; i++ ) {
		// Define the start and end of the current pair
		auto firstElementStart = iter;
		auto firstElementEnd = std::next( iter, elementSize );
		auto secondElementStart = firstElementEnd;
		auto secondElementEnd = std::next( secondElementStart, elementSize );

		// Compare the last element of the first pair with the last element of the second pair
		auto firstLast = std::prev(firstElementEnd);
		auto secondLast = std::prev(secondElementEnd);

		// The larger element goes to the main chain
		if ( *firstLast > *secondLast ) {
			largers.insert( largers.end(), firstElementStart, firstElementEnd );
			smallers.insert( smallers.end(), secondElementStart, secondElementEnd );
		} else {
			largers.insert( largers.end(), secondElementStart, secondElementEnd );
			smallers.insert( smallers.end(), firstElementStart, firstElementEnd );
		}

		// Move to the next pair
		iter = secondElementEnd;
	}

	// Put the last unpaired element to smallerElements sequens
	if ( elementsNum % 2 == 1 )
		smallers.insert( smallers.end(), iter, std::next( iter, elementSize ) );

	// Build the initial mainChain
	mainChain.insert( mainChain.end(), smallers.begin(), std::next( smallers.begin(), elementSize) );
	mainChain.insert( mainChain.end(), largers.begin(), largers.end() );

	size_t idxConst = 0;
	size_t idxActual = 1;
	for ( auto it = mainChain.begin() + elementSize; it != mainChain.end(); it += elementSize ) {
		largerIndexes.emplace_back( idxConst++, idxActual++ );
	}
}

template <typename Container>
void	PmergeMe::_sortInsertion( Container& data, size_t elementsNum, size_t elementSize ) {

	if ( elementsNum <= 2 ) return;

	// Temporary containers
	Container			mainChain;
	std::vector<Index>	largerIndexes;
	Container			smallerElements;

	// Fill in temporary containers
	_defineContainers( data, elementsNum, elementSize, mainChain, largerIndexes, smallerElements );

	size_t currJacobsthal = 3;
	size_t prevJacobsthal = 1;

	while ( true ) {

		auto	insertStart = smallerElements.begin();
		auto	insertEnd = smallerElements.end();
		size_t	elementsToInsert = 1;

		// Put elements in reverse order using Jacobsthal number
		if ( currJacobsthal <= smallerElements.size() / elementSize ) {
			elementsToInsert = currJacobsthal - prevJacobsthal;
			insertEnd = smallerElements.begin() + ( currJacobsthal * elementSize );

			// Insert elements from smallElements to mainChain
			_insertElements( mainChain, smallerElements, largerIndexes, insertEnd, elementsToInsert, elementSize );

			insertStart = smallerElements.begin() + ( ( currJacobsthal - 1 ) * elementSize );
			if ( insertStart + elementSize == smallerElements.end() ) break;

			// Calculate next Jacobsthal number and update the previous one
			_updateJacobsthal( currJacobsthal, prevJacobsthal );

		} else {

			// Put elements in reverse order
			insertStart = smallerElements.begin() + ( ( prevJacobsthal - 1 ) * elementSize );
			insertEnd = smallerElements.end();
			elementsToInsert = std::distance( insertStart, insertEnd - elementSize ) / elementSize;

			// Insert elements from smallElements to mainChain
			_insertElements( mainChain, smallerElements, largerIndexes, insertEnd, elementsToInsert, elementSize );

			break;
		}
	}

	// Update the main sequence of elements
	std::copy( mainChain.begin(), mainChain.end(), data.begin() );
}

template <typename Container>
void	PmergeMe::_sortPairs( typename Container::iterator start,
							  typename Container::iterator end, size_t elementSize ) {

	// Iterate through the container in steps of 2 * elementSize
	for ( auto it = start; it != end; std::advance( it, 2 * elementSize ) ) {
		// Define the range of the current pair
		auto firstPairStart = it;
		auto firstPairEnd = std::next( it, elementSize );
		auto secondPairStart = firstPairEnd;
		auto secondPairEnd = std::next( secondPairStart, elementSize );

		// Ensure the second pair is within bounds
		if ( secondPairStart >= end || secondPairEnd > end ) break;

		// Compare the last element of the first pair with the last element of the second pair
		auto firstPairLast = std::prev( firstPairEnd );
		auto secondPairLast = std::prev( secondPairEnd );

		// Increment the comparison counter only for this essential comparison
		_comparisonCounter++;

		if ( *firstPairLast > *secondPairLast )
			std::swap_ranges(firstPairStart, firstPairEnd, secondPairStart);
	}
}

template <typename Container>
void	PmergeMe::_sortFordJohnson( Container& data, size_t elementSize ) {

	// Nothing to sort if the number of elements is < 2
	size_t elementsNum = data.size() / elementSize;
	if ( elementsNum < 2 ) return;

	// Define starting and end position
	auto start = data.begin();
	auto end = ( elementsNum % 2 == 1 ) ? std::next( start, elementSize * ( elementsNum - 1 ) )
										: std::next( start, elementSize * elementsNum );

	// Start the first step of sorting algorithm - sorting elements in pairs
	_sortPairs<Container>( start, end, elementSize );

	// Recursively call function itself until the number of elements is < 2
	_sortFordJohnson( data, elementSize * 2 );

	// Start the second step of sorting algorithm - insert elements using binary search
	_sortInsertion( data, elementsNum, elementSize );
}

void	PmergeMe::parseInput( int argc, char** argv ) {

	for ( int i = 1; i < argc; i++ ) {
		int					value;
		std::istringstream	iss( argv[i] );

		if ( !( iss >> value ) || value <= 0 )
			throw std::out_of_range( "Error: Invalid input. Use only positive integers over 0." );

		_vectorData.push_back( value );
		_dequeData.push_back( value );
	}
}

void	PmergeMe::sortData( void ) {

	_printResult( BEFORE );

	// Start sorting for vector container
	_comparisonCounter = 0;
	auto start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _vectorData, 1 );
	auto end = std::chrono::high_resolution_clock::now();
	_vectorTime = std::chrono::duration<double, std::micro>(end - start).count();

	// Start sorting for deque container
	_comparisonCounter = 0;
	start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _dequeData, 1 );
	end = std::chrono::high_resolution_clock::now();
	_dequeTime = std::chrono::duration<double, std::micro>(end - start).count();

	_printResult( AFTER );
}

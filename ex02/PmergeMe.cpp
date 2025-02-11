/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/11 12:05:50 by tkubanyc         ###   ########.fr       */
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

	for (size_t k = 2; k <= n; ++k)
		result += static_cast<size_t>(std::ceil(std::log2( 3 * k / 4.0 )));

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

template <typename Container>
void PmergeMe::_binarySearchInsert( Container& dst, size_t dstEnd, const Container& src,
									typename Container::const_iterator srcIt, size_t elementSize ) {
	// Validate input parameters
	if ( elementSize == 0 || dst.empty() || src.empty() || dstEnd > dst.size() / elementSize ||
		srcIt < src.begin() || srcIt + elementSize > src.end() ) {
		std::cerr << "Error: Invalid input parameters!" << std::endl;
		return;
	}

	// Define the insertion element
	int insertValue = *( srcIt + elementSize - 1 );
	auto srcStart = srcIt;
	auto srcEnd = srcStart + elementSize;

	int low = 0;
	int high = static_cast<int>(dstEnd);

size_t currComparisons = _comparisonCounter;

	// Perform binary search for the insertion position
	while ( low <= high ) {
		int mid = low + ( high - low ) / 2;
		int dstElemIndex = mid * static_cast<int>(elementSize) + static_cast<int>(elementSize - 1);

		// Check bounds
		if ( dstElemIndex >= static_cast<int>(dst.size()) ) {
			std::cerr << "Error: dstElemIndex out of bounds!" << std::endl;
			return;
		}

		int dstElemValue = dst[dstElemIndex];
		if ( dstElemValue < insertValue )
			low = mid + 1;
		else
			high = mid - 1;

		_comparisonCounter++;
	}
// std::cout << "=== elementSize = " << elementSize << "\n";
// std::cout << "*** smallers: ";
// for ( const auto& num : src ) std::cout << num << " ";
// 	std::cout << std::endl;
// std::cout << "*** insertValue = " << insertValue << "\n";
// std::cout << "+++ mainChain: ";
// for ( const auto& num : dst ) std::cout << num << " ";
// 	std::cout << std::endl;
// std::cout << "+++ mainChain last index = " << dstEnd << "\n";
// std::cout << "--- currComparisons = " << _comparisonCounter - currComparisons << "\n";
// std::cout << "=== all Comparisons = " << _comparisonCounter << "\n\n";

	int insertPosition = low * static_cast<int>(elementSize);

	// Ensure insertPosition is within bounds
	if ( insertPosition > static_cast<int>(dst.size()) ) {
		std::cerr << "Error: insertPosition out of bounds!" << std::endl;
		return;
	}

	// Insert the element into the container
	dst.insert( dst.begin() + insertPosition, srcStart, srcEnd );
}


template <typename Container>
size_t PmergeMe::_findElementIndex( Container& dst, Container& src,
									size_t srcIndex, size_t elementSize ) {

	// Check if srcIndex is valid
	if ( srcIndex * elementSize + elementSize > src.size() ) {
		throw std::out_of_range("Error: srcIndex is out of range!");
	}

	// Extract the element from src
	size_t srcStartIndex = srcIndex * elementSize;
	Container srcElement( src.begin() + srcStartIndex, src.begin() + srcStartIndex + elementSize );

	// Get the representer of the srcElement
	int srcRepresenter = srcElement.back();

	// Perform binary search on dst
	int low = 0;
	int high = dst.size() / elementSize - 1;

	while ( low <= high ) {
		int mid = low + ( high - low ) / 2;
		int dstStartIndex = mid * elementSize;

		// Extract the current element from dst
		Container dstElement( dst.begin() + dstStartIndex, dst.begin() + dstStartIndex + elementSize );

		// Get the representer of the dstElement
		int dstRepresenter = dstElement.back();

		if ( dstRepresenter < srcRepresenter ) {
			// If the representer is smaller, search the right half
			low = mid + 1;
		} else if ( dstRepresenter > srcRepresenter ) {
			// If the representer is larger, search the left half
			high = mid - 1;
		} else {
			// If the representers match, check the entire element
			if ( dstElement == srcElement ) {
				return mid; // Return the index of the element in dst
			} else {
				// If the elements don't match, continue searching in both directions
				// First, search the left half
				int left = mid - 1;
				while ( left >= low ) {
					int leftStartIndex = left * elementSize;
					Container leftElement( dst.begin() + leftStartIndex, dst.begin() + leftStartIndex + elementSize );

					if ( leftElement.back() != srcRepresenter ) break;
					if ( leftElement == srcElement ) return left;

					left--;
				}

				// Then, search the right half
				int right = mid + 1;
				while ( right <= high ) {
					int rightStartIndex = right * elementSize;
					Container rightElement( dst.begin() + rightStartIndex, dst.begin() + rightStartIndex + elementSize );

					if ( rightElement.back() != srcRepresenter ) break;
					if ( rightElement == srcElement ) return right;

					right++;
				}

				break;
			}
		}
	}

	// If the element is not found, throw an exception
	throw std::runtime_error( "Error: Element not found in dst!" );
}

template <typename Container>
bool	PmergeMe::_isExist( const Container& container, size_t elementIndex, size_t elementSize ) {

	size_t startIndex = elementIndex * elementSize;

	return ( startIndex + elementSize <= container.size() );
}

template <typename Container>
void	PmergeMe::_defineContainers( Container& data, size_t elementsNum, size_t elementSize,
									 Container& mainChain, Container& largers, Container& smallers ) {

	auto it = data.begin();

	for ( size_t i = 0; i < elementsNum / 2; i++ ) {
		// Define the start and end of the current pair
		auto firstElementStart = it;
		auto firstElementEnd = std::next( it, elementSize );
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
		it = secondElementEnd;
	}

	// Put the last unpaired element to smallerElements sequens
	if ( elementsNum % 2 == 1 )
		smallers.insert( smallers.end(), it, std::next( it, elementSize ) );

	// Build the initial mainChain
	mainChain.insert( mainChain.end(), smallers.begin(), std::next( smallers.begin(), elementSize) );
	mainChain.insert( mainChain.end(), largers.begin(), largers.end() );
}

template <typename Container>
void	PmergeMe::_insertElements( Container& mainChain, Container& largers, Container& smallers,
									typename Container::iterator insertEnd,
									size_t elementsToInsert, size_t elementSize ) {

	size_t	insertIndex;
	size_t	lastIndex;

	for ( size_t i = 0; i < elementsToInsert; i++ ) {

		insertEnd -= elementSize;
		insertIndex = std::distance( smallers.begin(), insertEnd ) / elementSize;

		if ( _isExist( largers, insertIndex, elementSize ) )
			lastIndex = _findElementIndex( mainChain, largers, insertIndex, elementSize ) - 1;
		else
			lastIndex = ( mainChain.size() / elementSize ) - 1;

		_binarySearchInsert( mainChain, lastIndex, smallers, insertEnd, elementSize );

	}
}

template <typename Container>
void	PmergeMe::_sortInsertion( Container& data, size_t elementsNum, size_t elementSize ) {

	if ( elementsNum <= 2 ) return;

	// Temporary containers
	Container	mainChain;
	Container	largerElements;
	Container	smallerElements;

	_defineContainers( data, elementsNum, elementSize, mainChain, largerElements, smallerElements );

	size_t currJacobsthal = 3;
	size_t prevJacobsthal = 1;

	while ( true ) {

		auto	insertStart = smallerElements.begin();
		auto	insertEnd = smallerElements.end();
		size_t	elementsToInsert = 1;

		// Put elements from smallerElements to mainChain using Jacobsthal number
		if ( currJacobsthal <= smallerElements.size() / elementSize ) {
			elementsToInsert = currJacobsthal - prevJacobsthal;
			insertEnd = smallerElements.begin() + ( currJacobsthal * elementSize );

			_insertElements( mainChain, largerElements, smallerElements, insertEnd, elementsToInsert, elementSize );

			insertStart = smallerElements.begin() + ( ( currJacobsthal - 1 ) * elementSize );
			if ( insertStart + elementSize == smallerElements.end() ) break;

			_updateJacobsthal( currJacobsthal, prevJacobsthal );

		} else {

			// Put elements from smallerElements to mainChain
			insertStart = smallerElements.begin() + ( ( prevJacobsthal - 1 ) * elementSize );
			insertEnd = smallerElements.end();
			elementsToInsert = std::distance( insertStart, insertEnd - elementSize ) / elementSize;

			_insertElements( mainChain, largerElements, smallerElements, insertEnd, elementsToInsert, elementSize );

			break;
		}
	}

	std::copy( mainChain.begin(), mainChain.end(), data.begin() );

}

template <typename Container>
void	PmergeMe::_sortPairs( typename Container::iterator start,
							  typename Container::iterator end, size_t elementSize ) {

	// Iterate through the container in steps of 2 * pairLevel
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

	size_t elementsNum = data.size() / elementSize;
	if ( elementsNum < 2 ) return;

	using Iterator = typename Container::iterator;

	Iterator start = data.begin();
	Iterator end;

	if ( elementsNum % 2 == 1 )
		end = std::next( start, elementSize * ( elementsNum - 1) );
	else
		end = std::next( start, elementSize * elementsNum );

	this->_sortPairs<Container>( start, end, elementSize );
	_sortFordJohnson( data, elementSize * 2 );
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

	// Check if the input is already sorted
	// if ( std::is_sorted( _vectorData.begin(), _vectorData.end() )
	// 	&& std::is_sorted( _dequeData.begin(), _dequeData.end() ))
	// 	throw std::invalid_argument( "Input is already sorted" );
}

void	PmergeMe::sortData( void ) {

	_printResult( BEFORE );

	_comparisonCounter = 0;
	auto start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _vectorData, 1 );
	auto end = std::chrono::high_resolution_clock::now();
	_vectorTime = std::chrono::duration<double, std::micro>(end - start).count();

	// _comparisonCounter = 0;
	// start = std::chrono::high_resolution_clock::now();
	// _sortFordJohnson( _dequeData, 1 );
	// end = std::chrono::high_resolution_clock::now();
	// _dequeTime = std::chrono::duration<double, std::micro>(end - start).count();

	_printResult( AFTER );
}

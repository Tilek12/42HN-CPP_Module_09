/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/03 21:48:03 by tkubanyc         ###   ########.fr       */
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

size_t	PmergeMe::_countMinComparisons( size_t n ) const {

	double result = 0.0;

	if ( n != 0 && n != 1 )
		result = n * log2( n ) - n * log2( M_E ) + 0.5 * log2( 2 * M_PI * n );

	return static_cast<size_t>( std::ceil( result ) );
}

size_t	PmergeMe::_jacobsthal( size_t n ) {
	return std::round( ( std::pow( 2, n + 1 ) + std::pow( -1, n ) ) / 3 );
}

template <typename Container>
void	PmergeMe::_printElements( Container& data ) const {

	for ( const auto& num : data ) std::cout << num << " ";
	std::cout << std::endl;
	std::cout << "\n-----------------------------------------\n\n";
}

void	PmergeMe::_printComparisons( void ) const {

	std::cout << "\n-----------------------------------------\n\n";
	std::cout << "Possible comparisons for " << _vectorData.size()
			  << " elements: " << _countMinComparisons( _vectorData.size() )
			  << std::endl;

	std::cout << "Comparisons made: " << _comparisonCounter << std::endl;
	std::cout << "\n-----------------------------------------\n\n";
}

void	PmergeMe::_printResult( Prefix prefix ) const {

	switch ( prefix ) {

		case BEFORE:
			std::cout << "\n-----------------------------------------\n\n";
			std::cout << "Before: ";
			_printElements( _vectorData );
			break;

		case AFTER:
			std::cout << "After: ";
			_printElements( _vectorData );
			std::cout << "Time to process a range of " << _vectorData.size()
					  << " elements with std::vector: " << _vectorTime << " us"
					  << std::endl;
			// std::cout << "Time to process a range of " << _dequeData.size()
			// 		  << " elements with std::deque: " << _dequeTime << " us"
			// 		  << std::endl;
			_printComparisons();
			break;

		default:
			std::cerr << "Error: Incorrect prefix";
			break;
	}
}

template <typename Container>
void	PmergeMe::_swapPair( typename Container::iterator it, size_t elementSize ) {
	auto start = std::next( it, -elementSize + 1 );
	auto end = std::next( start, elementSize );

	while ( start != end ) {
		std::iter_swap( start, std::next( start, elementSize ) );
		start++;
	}
}

template <typename Iter>
bool	PmergeMe::_compare( Iter left, Iter right ) {
	return *left < *right;
}

// void	PmergeMe::calculateNextJacobsthal(size_t& currJacobsthal, size_t& prevJacobsthal) {

// 	size_t nextJacobsthal = currJacobsthal + 2 * prevJacobsthal;
// 	prevJacobsthal = currJacobsthal;
// 	currJacobsthal = nextJacobsthal;
// 	// std::cout << "--- prevJacobsthal = " << prevJacobsthal << std::endl;
// 	std::cout << "--- currJacobsthal = " << currJacobsthal << std::endl;
// }

// Template function to perform binary search and insert an element into the correct position
template <typename Container>
void	PmergeMe::_binarySearchInsert( Container& dst, const Container& src,
									typename Container::const_iterator srcIt, size_t elementSize ) {

	// Ensure srcIt is within the bounds of src
	if ( srcIt < src.begin() || srcIt + elementSize > src.end() ) {
		std::cerr << "Error: srcIt is out of bounds!" << std::endl;
		return;
	}

	// Calculate the value of the element to insert
	int insertValue = *(srcIt + elementSize - 1);

	// Calculate the start and end iterators for the element to insert
	auto srcStart = srcIt; // Start of the element in src
	auto srcEnd = srcStart + elementSize; // End of the element in src (one past the last)

	// Perform binary search to find the correct position in the destination container
	int low = 0;
	int high = ( dst.size() / elementSize ) - 1;

	while ( low <= high ) {
		int mid = low + (high - low) / 2;
		int dstElemIndex = mid * elementSize + (elementSize - 1);
		int dstElemValue = dst[dstElemIndex];

		if ( dstElemValue < insertValue )
			low = mid + 1;
		else
			high = mid - 1;
	}

	// Calculate the position to insert the new element
	int insertPosition = low * elementSize;

	// Insert the element into the container
	dst.insert( dst.begin() + insertPosition, srcStart, srcEnd );
}

template <typename Container>
void	PmergeMe::_insertion( Container& data, typename Container::iterator end,
								size_t elementSize, size_t elementsNum ) {

	if ( elementsNum <= 2 )
		return;

	using Iterator = typename Container::iterator;

	// Define temporary containers
	Container	mainChain;
	Container	largerElements;
	Container	smallerElements;


	Iterator it = data.begin();
	for ( size_t i = 0; i < elementsNum / 2; i++ ) {
		// Define the start and end of the current pair
		Iterator firstElementStart = it;
		Iterator firstElementEnd = std::next( it, elementSize );
		Iterator secondElementStart = firstElementEnd;
		Iterator secondElementEnd = std::next( secondElementStart, elementSize );

		// Compare the last element of the first pair with the last element of the second pair
		auto firstLast = std::prev(firstElementEnd);
		auto secondLast = std::prev(secondElementEnd);

		// The larger element goes to the main chain
		if ( *firstLast > *secondLast ) {
			largerElements.insert( largerElements.end(), firstElementStart, firstElementEnd );
			smallerElements.insert( smallerElements.end(), secondElementStart, secondElementEnd );
		} else {
			largerElements.insert( largerElements.end(), secondElementStart, secondElementEnd );
			smallerElements.insert( smallerElements.end(), firstElementStart, firstElementEnd );
		}

		// Move to the next pair
		it = secondElementEnd;
	}

	// Put the last unpaired element to smallerElements sequens
	if ( elementsNum % 2 == 1 ) {
		smallerElements.insert( smallerElements.end(), end, std::next( end, elementSize ) );
	}

	// Build the initial mainChain
	if ( !smallerElements.empty() )
		mainChain.insert( mainChain.end(), smallerElements.begin(), std::next( smallerElements.begin(), elementSize) );

	if ( !largerElements.empty() )
		mainChain.insert( mainChain.end(), largerElements.begin(), std::next( largerElements.begin(), elementSize));

	if ( largerElements.size() > elementSize )
		mainChain.insert( mainChain.end(), std::next( largerElements.begin(), elementSize ), std::next( largerElements.begin(), 2 * elementSize ) );

	Iterator smallElemPos = smallerElements.begin() + elementSize;

	_binarySearchInsert( mainChain, smallerElements, smallElemPos, elementSize );

    // // Insert elements from smallerElements into mainChain using Jacobsthal numbers
    // size_t currJacobsthal = 3;
    // size_t prevJacobsthal = 1;
    // size_t elementsToInsert = 1;
    // Iterator smallElemPosToInsert = smallerElements.begin() + elementSize;
    // Iterator largeElemPosToInsert = mainChain.begin() + elementSize;

    // while (true) {
    //     if (currJacobsthal <= smallerElements.size() / elementSize) {
    //         elementsToInsert = currJacobsthal - prevJacobsthal;
    //         smallElemPosToInsert = smallerElements.begin() + (currJacobsthal * elementSize);

    //         for (size_t i = 0; i < elementsToInsert; i++) {
    //             smallElemPosToInsert -= elementSize;
    //             binaryInsertToMainChain(smallElemPosToInsert, mainChain, elementSize);
    //         }

    //         if (largerElements.size() > (largeElemPosToInsert - mainChain.begin()) / elementSize) {
    //             for (size_t i = 0; i < elementsToInsert; i++) {
    //                 largeElemPosToInsert += elementSize;
    //                 binaryInsertToMainChain(largeElemPosToInsert, mainChain, elementSize);
    //                 if (largeElemPosToInsert == largerElements.end())
    //                     break;
    //             }
    //         }

	// 		calculateNextJacobsthal(currJacobsthal, prevJacobsthal);
    //     } else {
    //         smallElemPosToInsert += elementSize;
    //         binaryInsertToMainChain(smallElemPosToInsert, mainChain, elementSize);

    //         if (largerElements.size() > (largeElemPosToInsert - mainChain.begin()) / elementSize) {
    //             largeElemPosToInsert += elementSize;
    //             binaryInsertToMainChain(largeElemPosToInsert, mainChain, elementSize);
    //         }
    //     }

    //     if (smallElemPosToInsert >= smallerElements.end())
    //         break;

    //     elementsToInsert = 1;
    // }

	// // Copy the sorted mainChain back to the original data
	// std::copy( mainChain.begin(), mainChain.end(), data.begin() );




/////// START OF PSEUDOCODE OF INSERTIONS PART ///////

	// Insert elements from smallerElements into mainChain using Jacobsthal numbers

	// size_t currJacobsthal = 3;
	// size_t prevJacobsthal = 1;
	// size_t elementsToInsert = 1;
	// Iterator smallElemPosToInsert = 1;
	// Iterator largeElemPosToInsert = mainChain.size() - 1;

	// while ( true ) {

	// 	if ( currJacobsthal <= smallerElements.size() ) {
	// 		elementsToInsert = currJacobsthal - prevJacobsthal;
	// 		smallElemPosToInsert = currJacobsthal;

	// 		for ( size_t i = elementsToInsert; i > 0; i-- ) {
	// 			binaryInsertToMainChain( smallElemPosToInsert, mainChain, startPointInMainChain, endPointInMainChain );
	// 			smallElemPosToInsert--;
	// 		}
	// 		if ( largerElements.size() > largeElemPosToInsert ) {
	// 			for ( size_t i = 0; i <= elementsToInsert; i++ ) {
	// 				largeElemPosToInsert++;
	// 				binaryInsertToMainChain( largeElemPosToInsert, mainChain, startPointInMainChain, endPointInMainChain );
	// 				if ( largeElemPosToInsert == largerElements.size() )
	// 					break;
	// 			}
	// 		}
	// 	} else {
	// 		smallElemPosToInsert++;
	// 		binaryInsertToMainChain( smallElemPosToInsert, mainChain, startPointInMainChain, endPointInMainChain );
	// 		if ( largerElements.size() > largeElemPosToInsert ) {
	// 			largeElemPosToInsert++;
	// 			binaryInsertToMainChain( largeElemPosToInsert, mainChain, startPointInMainChain, endPointInMainChain );
	// 		}

	// 	}

	// 	if ( smallElemPosToInsert == smallerElements.size() )
	// 		break;

	// 	elementsToInsert = 1;
	// 	prevJacobsthal = currJacobsthal;
	// 	currJacobsthal = calculateNextJacobsthal();

	// }

	// std::copy( sortedData.begin(), sortedData.end(), data.begin() );

/////// END OF PSEUDOCODE OF INSERTIONS PART ///////

//////////////////////////////////////////////////////////////////////
	std::cout << "mainChain " << elementSize << "\t : ";
	size_t counter = 0;
	for ( const auto& num : mainChain ) {
		counter++;
		std::cout << num << " ";
		if ( counter % elementSize == 0 )
			std::cout << "- ";
	}
	std::cout << std::endl;

	std::cout << "largerElements " << elementSize << " : ";
	counter = 0;
	for ( const auto& num : largerElements ) {
		counter++;
		std::cout << num << " ";
		if ( counter % elementSize == 0 )
			std::cout << "- ";
	}
	std::cout << std::endl;

	std::cout << "smallerElements " << elementSize << ": ";
	counter = 0;
	for ( const auto& num : smallerElements ) {
		counter++;
		std::cout << num << " ";
		if ( counter % elementSize == 0 )
			std::cout << "- ";
	}
	std::cout << std::endl << std::endl;
//////////////////////////////////////////////////////////////////////

}

template <typename Container>
void PmergeMe::_sortPairs( typename Container::iterator start,
						typename Container::iterator end, size_t elementSize ) {

	// Iterate through the container in steps of 2 * pairLevel
	for ( auto it = start; it != end; std::advance( it, 2 * elementSize ) ) {
		// Define the range of the current pair
		auto firstPairStart = it;
		auto firstPairEnd = std::next( it, elementSize );
		auto secondPairStart = firstPairEnd;
		auto secondPairEnd = std::next( secondPairStart, elementSize );

		// Ensure the second pair is within bounds
		if ( secondPairStart >= end || secondPairEnd > end )
			break; // No more pairs to process

		// Compare the last element of the first pair with the last element of the second pair
		auto firstPairLast = std::prev( firstPairEnd );
		auto secondPairLast = std::prev( secondPairEnd );

		// Increment the comparison counter only for this essential comparison
		_comparisonCounter++;

		if ( *firstPairLast > *secondPairLast ) {
			// Swap the two pairs
			for ( size_t i = 0; i < elementSize; ++i ) {
				std::iter_swap( firstPairStart, secondPairStart );
				firstPairStart++;
				secondPairStart++;
			}
		}
	}
}

template <typename Container>
void	PmergeMe::_sortFordJohnson( Container& data, size_t elementSize ) {

	using Iterator = typename Container::iterator;

	size_t elementsNum = data.size() / elementSize;
	if ( elementsNum < 2 ) return;

//////////////////////////////////////////////////////////////////////
	std::cout << "Before " << elementSize << ": ";
	size_t counter = 0;
	for ( const auto& num : data ) {
		counter++;
		std::cout << num << " ";
		if ( counter % ( elementSize * 2 ) == 0 )
			std::cout << "|| ";
	}
	std::cout << std::endl << std::endl;
//////////////////////////////////////////////////////////////////////

	bool isOdd = elementsNum % 2 == 1;
	Iterator start = data.begin();
	Iterator last = std::next( data.begin(), elementSize * elementsNum );
	Iterator end = std::next( last, -(isOdd * elementSize ) );

	this->_sortPairs<Container>( start, end, elementSize );
	_sortFordJohnson( data, elementSize * 2 );
	_insertion( data, end, elementSize, elementsNum );

//////////////////////////////////////////////////////////////////////
	std::cout << "After " << elementSize << " : ";
	counter = 0;
	for ( const auto& num : data ) {
		counter++;
		std::cout << num << " ";
		if ( counter % (elementSize * 2) == 0 )
			std::cout << "|| ";
	}
	std::cout << std::endl << std::endl;
//////////////////////////////////////////////////////////////////////

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

	auto start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _vectorData, 1 );
	auto end = std::chrono::high_resolution_clock::now();
	_vectorTime = std::chrono::duration<double, std::micro>(end - start).count();

	// start = std::chrono::high_resolution_clock::now();
	// _sortFordJohnson( _dequeData, 1 );
	// end = std::chrono::high_resolution_clock::now();
	// _dequeTime = std::chrono::duration<double, std::micro>(end - start).count();

	_printResult( AFTER );
}

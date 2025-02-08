/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/08 02:05:53 by tkubanyc         ###   ########.fr       */
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
		result += static_cast<size_t>(std::ceil(std::log2( 3 * k / 4.0)));

	return result;
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
			  << " elements: " << _countPossibleComparisons( _vectorData.size() )
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
			std::cout << "Time to process a range of " << _dequeData.size()
					  << " elements with std::deque: " << _dequeTime << " us"
					  << std::endl;
			_printComparisons();
			if ( std::is_sorted( _vectorData.begin(), _vectorData.end() )
				&& std::is_sorted( _dequeData.begin(), _dequeData.end() )) {

				std::cout << "--------------- SORTED!!! ---------------"
						  << std::endl << std::endl;
			}
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

void	PmergeMe::_updateJacobsthal(size_t& currJacobsthal, size_t& prevJacobsthal) {

	size_t nextJacobsthal = currJacobsthal + 2 * prevJacobsthal;
	prevJacobsthal = currJacobsthal;
	currJacobsthal = nextJacobsthal;
}

template <typename Container>
void PmergeMe::_binarySearchInsert( Container& dst, size_t dstEnd, const Container& src,
									typename Container::const_iterator srcIt, size_t elementSize ) {

	// Check for edge cases
	if (elementSize == 0 || dst.empty() || src.empty()) {
		std::cerr << "Error: Invalid input parameters!" << std::endl;
		return;
	}

	size_t numElements = dst.size() / elementSize;

	// Ensure srcIt is within the bounds of src
	if (srcIt < src.begin() || srcIt + elementSize > src.end()) {
		std::cerr << "Error: srcIt is out of bounds!" << std::endl;
		return;
	}

	// Calculate the value of the element to insert
	int insertValue = *(srcIt + elementSize - 1);

	// Calculate the start and end iterators for the element to insert
	auto srcStart = srcIt; // Start of the element in src
	auto srcEnd = srcStart + elementSize; // End of the element in src (one past the last)

	// Validate dstEnd
	if (dstEnd > numElements) {
		std::cerr << "Error: Invalid dstEnd!" << std::endl;
		return;
	}

	// Perform binary search to find the correct position in the destination container
	int low = 0;
	int high = dstEnd;

	while (low <= high) {
		int mid = low + (high - low) / 2;

		// Calculate dstElemIndex safely
		if (mid > (std::numeric_limits<int>::max() - ((int)elementSize - 1)) / (int)elementSize) {
			std::cerr << "Error: Integer overflow in dstElemIndex calculation!" << std::endl;
			return;
		}
		int dstElemIndex = mid * elementSize + (elementSize - 1);

		// std::cout << "+++ dstElemIndex = " << dstElemIndex << std::endl;

		// Ensure dstElemIndex is within bounds
		if (dstElemIndex >= static_cast<int>(dst.size())) {
			std::cerr << "Error: dstElemIndex out of bounds!" << std::endl;
			std::cerr << "^^^^ dstElemIndex = " << dstElemIndex << std::endl;
			return;
		}

		// Get the representer value of the current element in dst
		int dstElemValue = dst[dstElemIndex];

		if (dstElemValue < insertValue) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
		_comparisonCounter++;
	}

	// Calculate the position to insert the new element
	int insertPosition = low * elementSize;

	// Ensure insertPosition is within bounds
	if (insertPosition > (int)dst.size()) {
		std::cerr << "Error: insertPosition out of bounds!" << std::endl;
		return;
	}

	// Insert the element into the container
	dst.insert(dst.begin() + insertPosition, srcStart, srcEnd);
}

template <typename Container>
size_t PmergeMe::_findElementIndex(Container& dst, Container& src, size_t srcIndex, size_t elementSize) {

	// Check if srcIndex is valid
	if (srcIndex * elementSize + elementSize > src.size()) {
		throw std::out_of_range("Error: srcIndex is out of range!");
	}

	// Extract the element from src
	size_t srcStartIndex = srcIndex * elementSize;
	Container srcElement(src.begin() + srcStartIndex, src.begin() + srcStartIndex + elementSize);

	// Get the representer of the srcElement
	int srcRepresenter = srcElement.back();

	// Perform binary search on dst
	int low = 0;
	int high = dst.size() / elementSize - 1;

	while (low <= high) {
		int mid = low + (high - low) / 2;
		int dstStartIndex = mid * elementSize;

		// Extract the current element from dst
		Container dstElement(dst.begin() + dstStartIndex, dst.begin() + dstStartIndex + elementSize);

		// Get the representer of the dstElement
		int dstRepresenter = dstElement.back();

		if (dstRepresenter < srcRepresenter) {
			// If the representer is smaller, search the right half
			low = mid + 1;
		} else if (dstRepresenter > srcRepresenter) {
			// If the representer is larger, search the left half
			high = mid - 1;
		} else {
			// If the representers match, check the entire element
			if (dstElement == srcElement) {
				return mid; // Return the index of the element in dst
			} else {
				// If the elements don't match, continue searching in both directions
				// First, search the left half
				int left = mid - 1;
				while (left >= low) {
					int leftStartIndex = left * elementSize;
					Container leftElement(dst.begin() + leftStartIndex, dst.begin() + leftStartIndex + elementSize);

					if (leftElement.back() != srcRepresenter) {
						break; // No more elements with the same representer
					}

					if (leftElement == srcElement) {
						return left; // Return the index of the element in dst
					}

					left--;
				}

				// Then, search the right half
				int right = mid + 1;
				while (right <= high) {
					int rightStartIndex = right * elementSize;
					Container rightElement(dst.begin() + rightStartIndex, dst.begin() + rightStartIndex + elementSize);

					if (rightElement.back() != srcRepresenter) {
						break; // No more elements with the same representer
					}

					if (rightElement == srcElement) {
						return right; // Return the index of the element in dst
					}

					right++;
				}

				// If no match is found, break the loop
				break;
			}
		}
	}

	// If the element is not found, throw an exception
	throw std::runtime_error( "Error: Element not found in dst!" );
}

template <typename Container>
bool	PmergeMe::_isExist(const Container& container, size_t elementIndex, size_t elementSize) {

	// Calculate the starting index of the element in the container
	size_t startIndex = elementIndex * elementSize;

	// Check if the entire element fits within the container
	return ( startIndex + elementSize <= container.size() );
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
		mainChain.insert( mainChain.end(), largerElements.begin(), largerElements.end() );

	size_t currJacobsthal = 3;
	size_t prevJacobsthal = 1;

	while ( true ) {

		Iterator	smallStartIt = smallerElements.begin();
		Iterator	smallCurrIt;
		size_t		smallItIndex;
		size_t		lastIndex;
		size_t		elementsToInsert = 1;

		if ( currJacobsthal <= smallerElements.size() / elementSize ) {
			elementsToInsert = currJacobsthal - prevJacobsthal;
			smallCurrIt = smallerElements.begin() + ( currJacobsthal * elementSize );

			// Put elements from smallerElements to mainChain using Jacobsthal number
			for ( size_t i = 0; i < elementsToInsert; i++ ) {

				smallCurrIt -= elementSize;
				smallItIndex = std::distance(smallerElements.begin(), smallCurrIt) / elementSize;

				if ( _isExist( largerElements, smallItIndex, elementSize ) )
					lastIndex = _findElementIndex( mainChain, largerElements, smallItIndex, elementSize ) - 1;
				else
					lastIndex = (mainChain.size() / elementSize) - 1;

				_binarySearchInsert( mainChain, lastIndex, smallerElements, smallCurrIt, elementSize );

			}

			smallStartIt = smallerElements.begin() + ( ( currJacobsthal - 1 ) * elementSize );
			if ( smallStartIt + elementSize == smallerElements.end() )
				break;

			_updateJacobsthal(currJacobsthal, prevJacobsthal);

		} else {

			// Put elements from smallerElements to mainChain
			smallStartIt = smallerElements.begin() + ( ( prevJacobsthal - 1 ) * elementSize );
			smallCurrIt = smallerElements.end();
			elementsToInsert = std::distance( smallStartIt, smallCurrIt - elementSize ) / elementSize;

			for ( size_t i = 0; i < elementsToInsert; i++ ) {
				smallCurrIt -= elementSize;
				smallItIndex = std::distance(smallerElements.begin(), smallCurrIt) / elementSize;

				if ( _isExist( largerElements, smallItIndex, elementSize ) )
					lastIndex = _findElementIndex( mainChain, largerElements, smallItIndex, elementSize ) - 1;
				else
					lastIndex = mainChain.size() / elementSize - 1;

				_binarySearchInsert( mainChain, lastIndex, smallerElements, smallCurrIt, elementSize );

			}

			break;
		}
	}

	std::copy( mainChain.begin(), mainChain.end(), data.begin() );


//////////////////////////////////////////////////////////////////////
	// std::cout << "data: ";
	// for ( const auto& num : data )
	// 	std::cout << num << " ";
	// std::cout << std::endl;

	// std::cout << "mainChain " << elementSize << "\t : ";
	// size_t counter = 0;
	// for ( const auto& num : mainChain ) {
	// 	counter++;
	// 	std::cout << num << " ";
	// 	if ( counter % elementSize == 0 )
	// 		std::cout << "- ";
	// }
	// std::cout << std::endl;

	// std::cout << "largerElements " << elementSize << " : ";
	// counter = 0;
	// for ( const auto& num : largerElements ) {
	// 	counter++;
	// 	std::cout << num << " ";
	// 	if ( counter % elementSize == 0 )
	// 		std::cout << "- ";
	// }
	// std::cout << std::endl;

	// std::cout << "smallerElements " << elementSize << ": ";
	// counter = 0;
	// for ( const auto& num : smallerElements ) {
	// 	counter++;
	// 	std::cout << num << " ";
	// 	if ( counter % elementSize == 0 )
	// 		std::cout << "- ";
	// }
	// std::cout << std::endl << std::endl;
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

	bool isOdd = elementsNum % 2 == 1;
	Iterator start = data.begin();
	Iterator last = std::next( data.begin(), elementSize * elementsNum );
	Iterator end = std::next( last, -(isOdd * elementSize ) );

	this->_sortPairs<Container>( start, end, elementSize );
	_sortFordJohnson( data, elementSize * 2 );
	_insertion( data, end, elementSize, elementsNum );

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

	if ( std::is_sorted( _vectorData.begin(), _vectorData.end() )
		&& std::is_sorted( _dequeData.begin(), _dequeData.end() ))
		throw std::invalid_argument( "Input is already sorted" );
}

void	PmergeMe::sortData( void ) {

	_printResult( BEFORE );

	_comparisonCounter = 0;
	auto start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _vectorData, 1 );
	auto end = std::chrono::high_resolution_clock::now();
	_vectorTime = std::chrono::duration<double, std::micro>(end - start).count();

	_comparisonCounter = 0;
	start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _dequeData, 1 );
	end = std::chrono::high_resolution_clock::now();
	_dequeTime = std::chrono::duration<double, std::micro>(end - start).count();

	_printResult( AFTER );

}

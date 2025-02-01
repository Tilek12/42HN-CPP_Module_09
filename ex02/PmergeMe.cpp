/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/02/01 19:22:28 by tkubanyc         ###   ########.fr       */
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

long	PmergeMe::_numberJacobsthal( long n ) {
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
void	PmergeMe::_swapPair( typename Container::iterator it, int pairLevel ) {
	auto start = std::next( it, -pairLevel + 1 );
	auto end = std::next( start, pairLevel );

	while ( start != end ) {
		std::iter_swap( start, std::next( start, pairLevel ) );
		start++;
	}
}

template <typename Iter>
bool	PmergeMe::_compare( Iter left, Iter right ) {
	return *left < *right;
}

template <typename Container>
void PmergeMe::_insertion(Container& data, typename Container::iterator end,
                          int pairLevel, size_t elementsNum) {
    using Iterator = typename Container::iterator;

    // Helper function for binary search to find the insertion position
    auto binarySearchInsertPosition = [this](Container& chain, const typename Container::value_type& target, Iterator bound) -> Iterator {
        auto low = chain.begin();
        auto high = bound;

        while (low < high) {
            auto mid = low + (high - low) / 2;
            _comparisonCounter++; // Increment only for essential comparisons
            if (this->_compare(*mid, target)) { // Compare values, not iterators
                low = mid + 1;
            } else {
                high = mid;
            }
        }
        return low;
    };

    // Step 1: Build the main chain and pending elements
    Container mainChain; // Use the template container directly
    Container pending;   // Use the template container directly

    // Add the first two elements to the main chain
    mainChain.push_back(*std::next(data.begin(), pairLevel - 1));
    mainChain.push_back(*std::next(data.begin(), pairLevel * 2 - 1));

    // Add remaining elements to pending
    for (size_t i = 4; i <= elementsNum; i += 2) {
        pending.push_back(*std::next(data.begin(), pairLevel * (i - 1) - 1));
        mainChain.push_back(*std::next(data.begin(), pairLevel * i - 1));
    }

    // Step 2: Insert elements using Jacobsthal numbers
    int prevJacobsthalNum = _numberJacobsthal(1);
    int insertedNums = 0;

    for (int i = 2; ; i++) {
        int currJacobsthalNum = _numberJacobsthal(i);
        int diffJacobsthalNum = currJacobsthalNum - prevJacobsthalNum;

        // Stop if no more elements to insert
        if (diffJacobsthalNum > static_cast<int>(pending.size())) break;

        // Insert elements in the current Jacobsthal range
        auto pendingIt = pending.begin() + (diffJacobsthalNum - 1);
        auto boundIt = mainChain.begin() + (currJacobsthalNum + insertedNums);

        for (int j = 0; j < diffJacobsthalNum; j++) {
            // Find the insertion position using the helper function
            auto insertPos = binarySearchInsertPosition(mainChain, *pendingIt, boundIt);

            // Insert the element into the main chain
            mainChain.insert(insertPos, *pendingIt);
            pendingIt = pending.erase(pendingIt) - 1; // Move back after erasing
        }

        prevJacobsthalNum = currJacobsthalNum;
        insertedNums += diffJacobsthalNum;
    }

    // Step 3: Insert any remaining pending elements
    for (auto& pendingElem : pending) {
        // Find the insertion position using the helper function
        auto insertPos = binarySearchInsertPosition(mainChain, pendingElem, mainChain.end());

        // Insert the element into the main chain
        mainChain.insert(insertPos, pendingElem);
    }

    // Step 4: Handle the odd element if present
    if (elementsNum % 2 == 1) {
        auto oddPair = *std::next(end, pairLevel - 1);

        // Find the insertion position using the helper function
        auto insertPos = binarySearchInsertPosition(mainChain, oddPair, mainChain.end());

        // Insert the odd element into the main chain
        mainChain.insert(insertPos, oddPair);
    }

    // Step 5: Reconstruct the sorted data from the main chain
    Container sortedData; // Use the template container directly

    for (auto& chainElem : mainChain) {
        for (int i = 0; i < pairLevel; i++) {
            auto pairStart = chainElem;
            sortedData.push_back(pairStart);
        }
    }

    // Step 6: Copy the sorted data back to the original container
    std::copy(sortedData.begin(), sortedData.end(), data.begin());
}

// template <typename Container>
// void PmergeMe::_insertion(Container& data, typename Container::iterator end,
//                           int pairLevel, size_t elementsNum) {

// 	using Iterator = typename Container::iterator;

//     // Step 1: Build the main chain and pending elements
//     std::vector<Iterator> mainChain;
//     std::vector<Iterator> pending;

//     // Add the first two elements to the main chain
//     mainChain.push_back(std::next(data.begin(), pairLevel - 1));
//     mainChain.push_back(std::next(data.begin(), pairLevel * 2 - 1));

//     // Add remaining elements to pending
//     for (size_t i = 4; i <= elementsNum; i += 2) {
//         pending.push_back(std::next(data.begin(), pairLevel * (i - 1) - 1));
//         mainChain.push_back(std::next(data.begin(), pairLevel * i - 1));
//     }

//     // Step 2: Insert elements using Jacobsthal numbers
//     int prevJacobsthalNum = _numberJacobsthal(1);
//     int insertedNums = 0;

//     for (int i = 2; ; i++) {
//         int currJacobsthalNum = _numberJacobsthal(i);
//         int diffJacobsthalNum = currJacobsthalNum - prevJacobsthalNum;

//         // Stop if no more elements to insert
//         if (diffJacobsthalNum > static_cast<int>(pending.size())) break;

//         // Insert elements in the current Jacobsthal range
//         auto pendingIt = pending.begin() + (diffJacobsthalNum - 1);
//         auto boundIt = mainChain.begin() + (currJacobsthalNum + insertedNums);

//         for (int j = 0; j < diffJacobsthalNum; j++) {
//             // Find the insertion position using a custom binary search
//             auto insertPos = mainChain.begin();
//             auto low = mainChain.begin();
//             auto high = boundIt;

//             while (low < high) {
//                 auto mid = low + (high - low) / 2;
//                 _comparisonCounter++;
//                 if (_compare(*pendingIt, *mid)) {
//                     high = mid;
//                 } else {
//                     low = mid + 1;
//                 }
//             }
//             insertPos = low;

//             // Insert the element into the main chain
//             mainChain.insert(insertPos, *pendingIt);
//             pendingIt = pending.erase(pendingIt) - 1; // Move back after erasing
//         }

//         prevJacobsthalNum = currJacobsthalNum;
//         insertedNums += diffJacobsthalNum;
//     }

//     // Step 3: Insert any remaining pending elements
//     for (auto& pendingElem : pending) {
//         // Find the insertion position using a custom binary search
//         auto insertPos = mainChain.begin();
//         auto low = mainChain.begin();
//         auto high = mainChain.end();

//         while (low < high) {
//             auto mid = low + (high - low) / 2;
//             _comparisonCounter++;
//             if (_compare(pendingElem, *mid)) {
//                 high = mid;
//             } else {
//                 low = mid + 1;
//             }
//         }
//         insertPos = low;

//         // Insert the element into the main chain
//         mainChain.insert(insertPos, pendingElem);
//     }

//     // Step 4: Handle the odd element if present
//     if (elementsNum % 2 == 1) {
//         auto oddPair = std::next(end, pairLevel - 1);

//         // Find the insertion position using a custom binary search
//         auto insertPos = mainChain.begin();
//         auto low = mainChain.begin();
//         auto high = mainChain.end();

//         while (low < high) {
//             auto mid = low + (high - low) / 2;
//             _comparisonCounter++;
//             if (_compare(oddPair, *mid)) {
//                 high = mid;
//             } else {
//                 low = mid + 1;
//             }
//         }
//         insertPos = low;

//         // Insert the odd element into the main chain
//         mainChain.insert(insertPos, oddPair);
//     }

//     // Step 5: Reconstruct the sorted data from the main chain
//     std::vector<int> sortedData;
//     sortedData.reserve(data.size());

//     for (auto& chainElem : mainChain) {
//         for (int i = 0; i < pairLevel; i++) {
//             Iterator pairStart = chainElem;
//             std::advance(pairStart, -pairLevel + i + 1);
//             sortedData.push_back(*pairStart);
//         }
//     }

//     // Step 6: Copy the sorted data back to the original container
//     std::copy(sortedData.begin(), sortedData.end(), data.begin());
// }

// template <typename Container>
// void PmergeMe::_insertion( Container& data, typename Container::iterator end,
// 							bool isOdd, int pairLevel, size_t elementsNum ) {

// 	using Iterator = typename Container::iterator;

// 	// Helper function to perform binary search and find the insertion position
// 	auto binarySearchInsertPosition = [this](const std::vector<Iterator>& chain, Iterator target, typename std::vector<Iterator>::const_iterator bound) -> typename std::vector<Iterator>::const_iterator {
// 		typename std::vector<Iterator>::const_iterator low = chain.begin();
// 		typename std::vector<Iterator>::const_iterator high = bound;

// 		while (low < high) {
// 			typename std::vector<Iterator>::const_iterator mid = low + (high - low) / 2;
// 			_comparisonCounter++;
// 			if ( _compare( target, *mid ) )
// 				high = mid;
// 			else
// 				low = mid + 1;
// 		}
// 		return low;
// 	};

// 	// Step 1: Build the main chain and pending elements
// 	std::vector<Iterator> mainChain;
// 	std::vector<Iterator> pending;

// 	// Add the first two elements to the main chain
// 	mainChain.push_back( std::next( data.begin(), pairLevel - 1 ) );
// 	mainChain.push_back( std::next( data.begin(), pairLevel * 2 - 1 ) );

// 	// Add remaining elements to pending
// 	for ( size_t i = 4; i <= elementsNum; i += 2 ) {
// 		pending.push_back( std::next( data.begin(), pairLevel * ( i - 1 ) - 1 ) );
// 		mainChain.push_back( std::next( data.begin(), pairLevel * i - 1 ) );
// 	}

// 	// Step 2: Insert elements using Jacobsthal numbers
// 	int prevJacobsthalNum = _numberJacobsthal( 1 );
// 	int insertedNums = 0;

// 	for ( int i = 2; ; i++ ) {
// 		int currJacobsthalNum = _numberJacobsthal( i );
// 		int diffJacobsthalNum = currJacobsthalNum - prevJacobsthalNum;

// 		// Stop if no more elements to insert
// 		if ( diffJacobsthalNum > static_cast<int>( pending.size() ) ) break;

// 		// Insert elements in the current Jacobsthal range
// 		auto pendingIt = pending.begin() + ( diffJacobsthalNum - 1 );
// 		auto boundIt = mainChain.begin() + ( currJacobsthalNum + insertedNums );

// 		for ( int j = 0; j < diffJacobsthalNum; j++ ) {
// 			auto insertPos = binarySearchInsertPosition(mainChain, *pendingIt, boundIt);
// 			mainChain.insert( insertPos, *pendingIt );
// 			pendingIt = pending.erase( pendingIt ) - 1; // Move back after erasing
// 		}

// 		prevJacobsthalNum = currJacobsthalNum;
// 		insertedNums += diffJacobsthalNum;
// 	}

// 	// Step 3: Insert any remaining pending elements
// 	for ( auto& pendingElem : pending ) {
// 		auto insertPos = binarySearchInsertPosition( mainChain, pendingElem, mainChain.end() );
// 		mainChain.insert( insertPos, pendingElem );
// 	}

// 	// Step 4: Handle the odd element if present
// 	if ( isOdd ) {
// 		auto oddPair = std::next( end, pairLevel - 1 );
// 		auto insertPos = binarySearchInsertPosition( mainChain, oddPair, mainChain.end() );
// 		mainChain.insert( insertPos, oddPair );
// 	}

// 	// Step 5: Reconstruct the sorted data from the main chain
// 	std::vector<int> sortedData;
// 	sortedData.reserve( data.size() );

// 	for ( auto& chainElem : mainChain ) {
// 		for ( int i = 0; i < pairLevel; i++ ) {
// 			Iterator pairStart = chainElem;
// 			std::advance( pairStart, -pairLevel + i + 1 );
// 			sortedData.push_back( *pairStart );
// 		}
// 	}

// 	// Step 6: Copy the sorted data back to the original container
// 	std::copy( sortedData.begin(), sortedData.end(), data.begin() );
// }

template <typename Container>
void PmergeMe::_sortPairs( typename Container::iterator start,
						typename Container::iterator end, int pairLevel ) {

	// Iterate through the container in steps of 2 * pairLevel
	for ( auto it = start; it != end; std::advance( it, 2 * pairLevel ) ) {
		// Define the range of the current pair
		auto firstPairStart = it;
		auto firstPairEnd = std::next( it, pairLevel );
		auto secondPairStart = firstPairEnd;
		auto secondPairEnd = std::next( secondPairStart, pairLevel );

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
			for ( int i = 0; i < pairLevel; ++i ) {
				std::iter_swap( firstPairStart, secondPairStart );
				firstPairStart++;
				secondPairStart++;
			}
		}
	}
}

template <typename Container>
void	PmergeMe::_sortFordJohnson( Container& data, int pairLevel ) {

	using Iterator = typename Container::iterator;

	size_t elementsNum = data.size() / pairLevel;
	if (elementsNum < 2) return;

//////////////////////////////////////////////////////////////////////
	std::cout << "Before in pair level " << pairLevel << ": ";
	for ( const auto& num : data ) std::cout << num << " ";
	std::cout << std::endl;
//////////////////////////////////////////////////////////////////////

	bool isOdd = elementsNum % 2 == 1;
	Iterator start = data.begin();
	Iterator last = std::next(data.begin(), pairLevel * elementsNum);
	Iterator end = std::next(last, -(isOdd * pairLevel));

	this->_sortPairs<Container>( start, end, pairLevel );
	_sortFordJohnson( data, pairLevel * 2 );
	_insertion( data, end, pairLevel, elementsNum );

//////////////////////////////////////////////////////////////////////
	// std::cout << "After in pair level " << pairLevel << " : ";
	// for ( const auto& num : data ) std::cout << num << " ";
	// std::cout << std::endl;
//////////////////////////////////////////////////////////////////////
}

// template <typename Container>
// void	PmergeMe::_sortFordJohnson( Container& data, int pairLevel ) {

// 	using Iterator = typename Container::iterator;

// 	size_t	elementsNum = data.size() / pairLevel;
// 	if ( elementsNum < 2 ) return;

// 	bool	isOdd = elementsNum % 2 == 1;

// 	Iterator start = data.begin();
// 	Iterator last = std::next( data.begin(), pairLevel * elementsNum );
// 	Iterator end = std::next( last, -( isOdd * pairLevel ) );

// 	for ( Iterator it = start; it != end; std::advance( it, 2 * pairLevel ) ) {
// 		Iterator thisPair = std::next( it, pairLevel - 1 );
// 		Iterator nextPair = std::next( it, pairLevel * 2 - 1 );

// 		if ( *thisPair > *nextPair ) {
// 			_comparisonCounter++;
// 			_swapPair<Container>( thisPair, pairLevel );
// 		}
// 	}

// 	_sortFordJohnson( data, pairLevel * 2 );

// 	std::vector<Iterator>	mainChain;
// 	std::vector<Iterator>	pending;

// 	mainChain.push_back( std::next( data.begin(), pairLevel -1 ) );
// 	mainChain.push_back( std::next( data.begin(), pairLevel * 2 - 1 ) );

// 	for ( size_t i = 4; i <= elementsNum; i += 2 ) {
// 		pending.push_back( std::next( data.begin(), pairLevel * ( i - 1 ) - 1 ) );
// 		mainChain.push_back( std::next( data.begin(), pairLevel * i - 1 ) );
// 	}

// 	int prevJacobsthalNum = _numberJacobsthal( 1 );
// 	int insertedNums = 0;

// 	for ( int i = 2; ; i++ ) {
// 		int currJacobsthalNum = _numberJacobsthal( i );
// 		int diffJacobsthalNum = currJacobsthalNum - prevJacobsthalNum;

// 		if ( diffJacobsthalNum > static_cast<int>( pending.size() ) ) break;

// 		int numOfTimes = diffJacobsthalNum;
// 		auto pendingIt = std::next( pending.begin(), diffJacobsthalNum - 1 );
// 		auto boundIt = std::next( mainChain.begin(), currJacobsthalNum + insertedNums );

// 		while ( numOfTimes ) {
// 			auto idx = std::upper_bound( mainChain.begin(), boundIt, *pendingIt, _compare<Iterator> );
// 			// auto idx = std::upper_bound(
//             //     mainChain.begin(), boundIt, *pendingIt,
//             //     [this](const Iterator& a, const Iterator& b) {
//             //         _comparisonCounter++;
//             //         return *a < *b;
//             //     });
// 			mainChain.insert( idx, *pendingIt );
// 			numOfTimes--;
// 			pendingIt = pending.erase( pendingIt );
// 			std::advance( pendingIt, -1 );
// 			boundIt = std::next( mainChain.begin(), currJacobsthalNum + insertedNums );
// 		}

// 		prevJacobsthalNum = currJacobsthalNum;
// 		insertedNums += diffJacobsthalNum;
// 	}

// 	for ( size_t i = 0; i < pending.size(); i++ ) {
// 		auto currPending = std::next( pending.begin(), i );
// 		auto currBound = std::next( mainChain.begin(), mainChain.size() - pending.size() + i );
// 		auto idx = std::upper_bound( mainChain.begin(), currBound, *currPending, _compare<Iterator> );
// 		// auto idx = std::upper_bound(
//         //     mainChain.begin(), currBound, *currPending,
//         //     [this](const Iterator& a, const Iterator& b) {
//         //         _comparisonCounter++;
//         //         return *a < *b;
//         //     });
// 		mainChain.insert( idx, *currPending );
// 	}

// 	if ( isOdd ) {
// 		auto oddPair = std::next( end, pairLevel - 1 );
// 		auto idx = std::upper_bound( mainChain.begin(), mainChain.end(), oddPair, _compare<Iterator> );
// 		// auto idx = std::upper_bound(
//         //     mainChain.begin(), mainChain.end(), oddPair,
//         //     [this](const Iterator& a, const Iterator& b) {
//         //         _comparisonCounter++;
//         //         return *a < *b;
//         //     });
// 		mainChain.insert( idx, oddPair );
// 	}

// 	std::vector<int> copy;
// 	copy.reserve( data.size() );

// 	for ( auto it = mainChain.begin(); it != mainChain.end(); it++ ) {
// 		for ( int i = 0; i < pairLevel; i++ ) {
// 			Iterator pairStart = *it;
// 			std::advance( pairStart, -pairLevel + i + 1 );
// 			copy.push_back( *pairStart );
// 		}
// 	}

// 	Iterator dataIt = data.begin();
// 	for ( auto copyIt = copy.begin(); copyIt != copy.end(); copyIt++ ) {
// 		*dataIt = *copyIt;
// 		dataIt++;
// 	}
// }

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

	start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _dequeData, 1 );
	end = std::chrono::high_resolution_clock::now();
	_dequeTime = std::chrono::duration<double, std::micro>(end - start).count();

	_printResult( AFTER );
}

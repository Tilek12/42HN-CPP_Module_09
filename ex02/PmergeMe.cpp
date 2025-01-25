/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkubanyc <tkubanyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 19:28:41 by tkubanyc          #+#    #+#             */
/*   Updated: 2025/01/25 09:05:19 by tkubanyc         ###   ########.fr       */
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

template <typename Container>
void	PmergeMe::_sortPairs( Container& data, Container& smaller, Container& larger ) {

	for ( size_t i = 0; i + 1 < data.size(); i += 2 ) {
		if ( data[i] > data[i + 1] ) {
			smaller.push_back( data[i + 1] );
			larger.push_back( data [i] );
		} else {
			smaller.push_back( data[i] );
			larger.push_back( data[i + 1] );
		}
	}

	if ( data.size() % 2 != 0 )
		smaller.push_back( data.back() );

	std::sort( smaller.begin(), smaller.end() );
}

template <typename Container>
void	PmergeMe::_insertLargerElements( Container& sorted, const Container& larger ) {

	for ( size_t i = 0; i < larger.size(); i++ ) {
		auto pos = std::upper_bound( sorted.begin(), sorted.end(), larger[i] );
		sorted.insert( pos, larger[i] );
	}
}

template <typename Container>
void	PmergeMe::_sortFordJohnson( Container& data ) {

	if ( data.size() <= 1 ) return;

	Container smaller, larger;
	_sortPairs( data, smaller, larger );

	Container sorted = smaller;
	_insertLargerElements( sorted, larger );

	data = sorted;
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
	_sortFordJohnson( _vectorData );
	auto end = std::chrono::high_resolution_clock::now();
	vectorTime = std::chrono::duration<double, std::micro>(end - start).count();

	start = std::chrono::high_resolution_clock::now();
	_sortFordJohnson( _dequeData );
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

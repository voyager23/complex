/*
 * sieve.cpp
 * 
 * Copyright 2015 Mike <mike@fc21>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * Implement the Sieve of Eratosthenes as described in Wikipedia
 */

#include <iostream>
#include <vector>
#include <cmath>

void sieve(std::vector<int> *p, int n) {
	int limit = (int)sqrt((double)n);
	std::vector<bool> A(n,true);
	for(int i = 2; i < limit; ++i) {
		if(A[i]) {
			for(int j = i*i; j < n; j += i) A[j] = false;
		}
	}
	// scan the sieve and store primes in p
	p->clear();
	for(int i = 2; i != n; ++i)	if(A[i]) p->push_back(i);		
}

void primes3mod4(std::vector<int> *primes, std::vector<int> *p34) {
	p34->clear();
	for(auto a = primes->begin(); a != primes->end(); ++a)
		if( ((*a)%4)==3 ) p34->push_back(*a);
}

int main(int argc, char **argv)
{
	
	std::vector<int> primes;	// vector of real primes	
	std::vector<int> p34;		// vector of real primes congruent to 3 mod 4
	
	sieve( &primes, 100);		// establish vector of real primes
	
	for(auto a = primes.begin(); a != primes.end(); ++a) std::cout << *a << ",";	
	std::cout << "\b " << std::endl;
	
	std::cout << std::endl;
	
	primes3mod4(&primes, &p34);	// find subset of real primes congruent to 3 mod 4
	for(auto a = p34.begin(); a != p34.end(); ++a) std::cout << *a << ",";	
	std::cout << "\b " << std::endl;
	
	return 0;
}


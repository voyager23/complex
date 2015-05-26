/*
 * cpg.cpp
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
 * Complex Prime Generator
 * 
 * Gaussian Prime z = a + bi
 * 
 * Case 1) Both a and b are non-zero
 * 		iff a^2 + b^2 is an ordinary prime
 * Case 2) a = 0
 * 		iff |b| is prime AND |b| = 3 mod 4
 * Case 3) b = 0
 * 		iff |a| is prime AND |a| = 3 mod 4
 * 
 */


#include <iostream>
#include <array>
#include <vector>
#include <complex>
#include <algorithm>
#include <cmath>

using namespace std;

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

int vect_gaussprimes(std::vector< std::complex<int> > *gpv, int n) {
	
	// establish a list of primes up to 2*n*n
	std::vector<int> primes;	// vector of real primes	
	std::vector<int> p34;		// vector of real primes congruent to 3 mod 4

	
	sieve( &primes, (2*n*n) );	// establish vector of real primes
	primes3mod4(&primes, &p34);			// find subset of real primes congruent to 3 mod 4
	
	for(int a = 1; a <= n; ++a) {
		for(int b = 0; b <= a; ++b) {
			if(b != 0) {	//testcase 1 [a^2 + b^2 is prime]
				int gp = ((a*a)+(b*b));
				// if gp in primes a+bi is gaussian prime
				if(binary_search(primes.begin(), primes.end(), gp)) {
					// gp is gaussian
					std::complex<int> wspace (a,b);	// workspace for complex prime
					gpv->push_back(wspace);
					cout << a;
					if(b>0) cout << "+i";
					if(b>1) cout << b;
					cout << std::endl;
				}
			} else {		//testcase 3 [|a| is prime AND |a| mod 4 = 3]
				int gp = abs(a);
				if ( binary_search(p34.begin(), p34.end(), gp) ) {
					// gp is gaussian
					std::complex<int> wspace (a,b);	// workspace for complex prime
					gpv->push_back(wspace);
					cout << a;
					if(b>0) cout << "+i";
					if(b>1) cout << b;
					cout << std::endl;
				}
			}
		}			
	}
	return gpv->size();
}

int main(int argc, char **argv)
{
	const int Limit = 50;
	
	std::vector< std::complex<int> > gaussian;
	vect_gaussprimes(&gaussian, Limit);
	

	cout << std::endl << "Found " << gaussian.size() << " complex primes." << std::endl;
	return 0;
}


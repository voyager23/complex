/*
 * cts.cpp
 * 
 * Copyright 2015 mike <mike@localhost.localdomain>
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
 * 1) Input a complex target.
 * 2) Assess number of unique gPrimes required to sum to target.
 * 3) If number of unique gPrimes >=12:
 *		Generate a Nodelist from the restricted list of gPrimes.
 * 		Do 4 index search for solutions which sum to target.
 * 		Save results.
 * 4) Done
 */


#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

using gPrime = std::complex<double>;
using cNode = std::vector<gPrime>;
using NodeList = std::vector<cNode>;
using KeyValue = std::pair<gPrime,cNode>;

//----------------------------------------------------------------------
struct hash_X{
  size_t operator()(const complex<int> &x) const{
	size_t hashcode = 23;
	hashcode = (hashcode * 37) + x.real();
	hashcode = (hashcode * 37) + x.imag();
	//hash<int> f;
	//hashcode = f( x.real() ) ^ f( x.imag() );
  return hashcode;
  };
};
//----------------------------------------------------------------------
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
//----------------------------------------------------------------------
void primes3mod4(std::vector<int> *primes, std::vector<int> *p34) {
	p34->clear();
	for(auto a = primes->begin(); a != primes->end(); ++a)
		if( ((*a)%4)==3 ) p34->push_back(*a);
}
//----------------------------------------------------------------------
int vect_gaussprimes(cNode *gpv, int n) {

	// establish a list of primes up to 2*n*n
	// this allows search for prime in test case 1
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
					gPrime wspace (a,b);	// workspace for complex prime
					gpv->push_back(wspace);
#if(0)					
					cout << a;
					if(b>0) cout << "+i";
					if(b>1) cout << b;
					cout << std::endl;
#endif
				}
			} else {		//testcase 3 [|a| is prime AND |a| mod 4 = 3]
				int gp = abs(a);
				if ( binary_search(p34.begin(), p34.end(), gp) ) {
					// gp is gaussian
					gPrime wspace (a,b);	// workspace for complex prime
					gpv->push_back(wspace);
#if(0)					
					cout << a;
					if(b>0) cout << "+i";
					if(b>1) cout << b;
					cout << std::endl;
#endif					
				}
			}
		}
	}
	return gpv->size();
}
//----------------------------------------------------------------------
bool gprime_lt(gPrime i, gPrime j) {
	// return true if i < j
	return ((i.real()<j.real())||((i.real()==j.real())&&(i.imag()<j.imag())));
}
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
	double real,imag;
	std::complex<double> Target;
	int Limit;
	
	cout << "Complex Tocta search program." << std::endl;
	cout << "Enter real part of target:";
	cin >> real;
	cout << "Enter imag part of target:";
	cin >> imag;
	Target = {real,imag};
	cout << "Target requested:" << Target << std::endl;
	
	cNode gaussian;		// cNode is defined as a vector of gPrimes
	(int(abs(real)))>=(int(abs(imag))) ? Limit=(int(abs(real))) : Limit=(int(abs(imag)));

	vect_gaussprimes(&gaussian, Limit); // generate a vector of Gaussian primes

	std::vector<cNode> sums;	// vector of cNodes which sum to target value
	
    for(auto a=gaussian.begin(); a != gaussian.end(); ++a) {
    	for(auto b=gaussian.begin(); b != gaussian.end(); ++b) {
    		if(b==a) continue;
    		for(auto c=gaussian.begin(); c != gaussian.end(); ++c) {
    			if((c==a)||(c==b)) continue;
    			for(auto d=gaussian.begin(); d != gaussian.end(); ++d) {
    				if((d==a)||(d==b)||(d==c)) continue;
    				gPrime foo {(*a+*b+*c+*d)};
    				if(Target==foo) {

						sums.push_back({ *a, *b, *c, *d });
					}
    			}
    		}
    	}
    }
    
    std::vector<gPrime> gprimes;
    cNode cn;
	
	for(auto b = sums.begin(); b != sums.cend(); ++b) {
		cn = *b;	// vector of 4 gPrimes.
		// copy the 4 gPrimes into a vector for sorting
		for(auto c = cn.begin(); c != cn.end(); ++c) gprimes.push_back(*c);
	}
	// sort the node elements (ascending)
	std::sort(gprimes.begin(), gprimes.end(), gprime_lt );
	// delete duplicate entries
	std::vector<gPrime>::iterator it;
	it = std::unique(gprimes.begin(), gprimes.end());
	gprimes.resize( std::distance(gprimes.begin(),it) );
	// Report final size and associated sum
	cout << "Target sum:" << Target << " number of unique gprimes:" << gprimes.size() << std::endl;
	if(gprimes.size() < 12)	{
		cout << "Not enough gPrimes!";
	} else {
		cout << "Sufficient to contine.";
	}
	cout << std::endl;
	return 0;
}


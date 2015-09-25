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

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "./fasthash.h"

using namespace std;

using gPrime = std::complex<double>;
using cNode = std::vector<gPrime>;
using NodeList = std::vector<cNode>;
using KeyValue = std::pair<gPrime,cNode>;

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
int vect_gaussprimes(cNode *gpv, std::complex<double> target) {
	
	int real_limit = (int(abs(target.real())));
	int imag_limit = (int(abs(target.imag())));
	
	// setup the loop limits
	int n;
	(real_limit)>(imag_limit) ? n=real_limit : n=imag_limit;
	
	// largest complex prime required is (Target - 1+i - 2+i - 3)
	// modify the real & imag limit values
	real_limit -= 6;
	imag_limit -= 2;
	
	// establish a list of primes up to 2*n*n
	//		(this allows for a prime test in case 1)
	std::vector<int> primes;	// vector of real primes
	std::vector<int> p34;		// vector of real primes congruent to 3 mod 4
	
	sieve( &primes, (2*n*n) );	// establish vector of real primes
	primes3mod4(&primes, &p34);			// find subset of real primes congruent to 3 mod 4

	for(int a = 1; a <= n; ++a) {
		for(int b = 0; b <= a; ++b) {
			gPrime wspace (a,b);	// workspace for complex prime
			
			// limit the gPrimes list to Target - (6+i2)
			if(a > real_limit) break;
			if((a == real_limit)&&(b > imag_limit)) break;			
			
			if(b != 0) {	//testcase 1 [a^2 + b^2 is prime]
				int gp = ((a*a)+(b*b));
				// if gp in primes a+bi is gaussian prime
				if(binary_search(primes.begin(), primes.end(), gp)) {
					// gp is gaussian
					// gPrime wspace (a,b);	// workspace for complex prime
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
					// gPrime wspace (a,b);	// workspace for complex prime
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

//======================================================================

int main(int argc, char **argv)
{
	// Data block of possible target values
	std::vector<std::complex<double>> fozboz = { 
		std::complex<double>(23,18),
		std::complex<double>(24,3),
		std::complex<double>(24,4),
		std::complex<double>(24,5),
		std::complex<double>(24,6),
		std::complex<double>(24,7),
		std::complex<double>(24,8),
		std::complex<double>(24,9),
		std::complex<double>(24,10),
		std::complex<double>(24,11),
		std::complex<double>(24,12),
		std::complex<double>(24,13),
		std::complex<double>(24,14),
		std::complex<double>(24,15),
		std::complex<double>(24,16),
		std::complex<double>(24,17),
		std::complex<double>(24,18),
		std::complex<double>(24,19),
		std::complex<double>(25,3),
		std::complex<double>(25,4),
		std::complex<double>(25,5),
		std::complex<double>(25,6),
		std::complex<double>(25,7),
		std::complex<double>(25,8),
		std::complex<double>(25,9),
		std::complex<double>(25,10),
		std::complex<double>(25,11),
		std::complex<double>(25,12),
		std::complex<double>(25,13),
		std::complex<double>(25,14),
		std::complex<double>(25,15),
		std::complex<double>(25,16),
		std::complex<double>(25,17),
		std::complex<double>(25,18),
		std::complex<double>(25,19),
		std::complex<double>(25,20),
		std::complex<double>(26,3),
		std::complex<double>(26,4),
		std::complex<double>(26,5),
		std::complex<double>(26,6),
		std::complex<double>(26,7),
		std::complex<double>(26,8),
		std::complex<double>(26,9),
		std::complex<double>(26,10),
		std::complex<double>(26,11),
		std::complex<double>(26,12),
		std::complex<double>(26,13),
		std::complex<double>(26,14),
		std::complex<double>(26,15),
		std::complex<double>(26,16),
		std::complex<double>(26,17),
		std::complex<double>(26,18),
		std::complex<double>(26,19),
		std::complex<double>(26,20),
		std::complex<double>(26,21),
		std::complex<double>(27,3),
		std::complex<double>(27,4),
		std::complex<double>(27,5),
		std::complex<double>(27,6),
		std::complex<double>(27,7),
		std::complex<double>(27,8),
		std::complex<double>(27,9),
		std::complex<double>(27,10),
		std::complex<double>(27,11),
		std::complex<double>(27,12),
		std::complex<double>(27,13),
		std::complex<double>(27,14),
		std::complex<double>(27,15),
		std::complex<double>(27,16),
		std::complex<double>(27,17),
		std::complex<double>(27,18),
		std::complex<double>(27,19),
		std::complex<double>(27,20),
		std::complex<double>(27,21),
		std::complex<double>(27,22)
	};
	
	std::complex<double> Target;
	NodeList nl;
	
	for(auto w = fozboz.begin(); w != fozboz.end(); ++w) {	
		Target = *w;
		cout << std::endl << "Target requested:" << Target << std::endl;
		
		// cNode is a vector of gPrimes
		cNode gaussian;		
		// generate a vector of Gaussian primes limited to (Target - (6+i2));
		vect_gaussprimes(&gaussian, Target); 

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
			exit(0);
		} else {
			cout << "Sufficient to contine.";
		}
		cout << std::endl;
		// From the revised gPrime vector - generate a new Nodelist
		nl.clear();
		for(auto a = gprimes.begin(); a != gprimes.end(); ++a) {
			for(auto b = gprimes.begin(); b != gprimes.end(); ++b) {
				if(a==b) continue;
				for(auto c = gprimes.begin(); c != gprimes.end(); ++c) {
					if((c==b)||(c==a)) continue;
					for(auto d = gprimes.begin(); d != gprimes.end(); ++d) {
						if((d==c)||(d==b)||(d==a)) continue;
						gPrime sum =(*a+*b+*c+*d);
						if(sum==Target) nl.push_back({*a,*b,*c,*d});
					}
				}
			}
		}
		
		cout << "Nodelist size:" << nl.size() << std::endl;
		
	// -----------standard 4 index search-------------------------------
	
		int count = 0;
		bool fail_flag;
		for(auto a = nl.begin(); a != nl.end(); ++a) {		
			for( auto b = nl.begin(); b != nl.end(); ++b) {
				if(b==a) continue;
				// test diagonal match here
				if((*a)[1] != (*b)[0]) continue;
				// test for unique values
				fail_flag = 0;
				for(int y=1; y!=4; ++y) {	// index to 'b' vector
					for(int x=0; x!=4; ++x) {	// index to 'a' vector
						fail_flag = ((*a)[x] == (*b)[y]);
						if(fail_flag) break;
					}
					if(fail_flag) break;	// break out of xy loop on fail
				}
				if(fail_flag) continue;		// get next b value
				
				for(auto c = nl.begin(); c != nl.end(); ++c) {
					if((c==a)||(c==b)) continue;
					// test diagonal match here
					if((*b)[1] != (*c)[0]) continue;
					
					// test for unique values between c and a
					fail_flag = 0;
					for(int y=1; y!=4; ++y) {	// index to 'c' vector
						for(int x=0; x!=4; ++x) {	// index to 'a' vector
							fail_flag = ((*a)[x] == (*c)[y]);
							if(fail_flag) break;
						}
						if(fail_flag) break;	// break out of xy loop on fail
					}
					if(fail_flag) continue;		// get next c value
					
					// test for unique values between c and b
					for(int y=1; y!=4; ++y) {	// index to 'c' vector
						for(int x=1; x!=4; ++x) {	// index to 'b' vector
							fail_flag = ((*b)[x] == (*c)[y]);
							if(fail_flag) break;
						}
						if(fail_flag) break;	// break out of xy loop on fail
					}
					if(fail_flag) continue;		// get next c value
					
					for(auto d = nl.begin(); d != nl.end(); ++d) {
						if((d==c)||(d==b)||(d==a)) continue;
						// test 2 diagonal matches here
						if((*c)[1] != (*d)[0]) continue;
						if((*d)[1] != (*a)[0]) continue;
						// test for unique values between d and a
						fail_flag = 0;
						for(int y=2; y!=4; ++y) {	// index to 'd' vector
							for(int x=0; x!=4; ++x) {	// index to 'a' vector
								fail_flag = ((*a)[x] == (*d)[y]);
								if(fail_flag) break;
							}
							if(fail_flag) break;	// break out of xy loop on fail
						}
						if(fail_flag) continue;		// get next d value
						
						// test for unique values between d and b
						for(int y=2; y!=4; ++y) {	// index to 'd' vector
							for(int x=1; x!=4; ++x) {	// index to 'b' vector
								fail_flag = ((*b)[x] == (*d)[y]);
								if(fail_flag) break;
							}
							if(fail_flag) break;	// break out of xy loop on fail
						}
						if(fail_flag) continue;		// get next d value
						
						// test for unique values between d and c
						for(int y=2; y!=4; ++y) {	// index to 'd' vector
							for(int x=1; x!=4; ++x) {	// index to 'c' vector
								fail_flag = ((*c)[x] == (*d)[y]);
								if(fail_flag) break;
							}
							if(fail_flag) break;	// break out of xy loop on fail
						}
						if(fail_flag) continue;		// get next d value
						
						// Column sum check to target value
						if(( (*a)[2]+(*b)[2]+(*c)[2]+(*d)[2] ) != Target ) continue;
						if(( (*a)[3]+(*b)[3]+(*c)[3]+(*d)[3] ) != Target ) continue;
						
						++count; // Found a Solution - inc count
						
						std::vector<gPrime> sig_vector = {(*a)[0],(*a)[1],(*a)[2],(*a)[3],\
						(*b)[1],(*b)[2],(*b)[3],\
						(*c)[1],(*c)[2],(*c)[3],\
						(*d)[2],(*d)[3]};					
						std::sort(sig_vector.begin(), sig_vector.end(), gprime_lt);	// key value for multi_map
											
						size_t hash = 0x12345678;
						for(auto x=sig_vector.begin(); x!=sig_vector.end(); ++x) {
							std::array<double,2> values = { (*x).real(), (*x).imag() };
							// size_t is 4 bytes so use fasthash32
							hash = fasthash32((const void*)&values, sizeof(double)*2, hash);
						}
						
						std::vector<gPrime> tocta_vector = {
						(*a)[0],(*a)[1],(*a)[2],(*a)[3],\
						(*b)[0],(*b)[1],(*b)[2],(*b)[3],\
						(*c)[0],(*c)[1],(*c)[2],(*c)[3],\
						(*d)[0],(*a)[0],(*d)[2],(*d)[3]};	// value for multimap				
						cout << "Target:" << Target << "\tHash:" << hash << std::endl;
						
						// print 4 vectors and blank line
						for(auto z = (*a).begin(); z != (*a).end(); ++z) {
							cout << *z << "\t";
						}
						cout << std::endl;
						for(auto z = (*b).begin(); z != (*b).end(); ++z) {
							cout << *z << "\t";
						}
						cout << std::endl;
						for(auto z = (*c).begin(); z != (*c).end(); ++z) {
							cout << *z << "\t";
						}
						cout << std::endl;
						for(auto z = (*d).begin(); z != (*d).end(); ++z) {
							cout << *z << "\t";
						}
						cout << std::endl << std::endl;
						
					} // end d
				} // end c			
			} // end b
		} // end a
		cout << "Found " << count << " solutions.\n";
	}	// fozboz
	return 0;
}


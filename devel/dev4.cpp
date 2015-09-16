/*
 * dev4.cpp
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
 * Analysis of possible target values for a Tocta node of 4 complex gaussian primes
 *
 */

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
struct hash_X{
  size_t operator()(const complex<double> &x) const{
	size_t hashcode = 23.0;
	hashcode = (hashcode * 37.0) + x.real();
	hashcode = (hashcode * 37.0) + x.imag();
  return hashcode;
  }
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
					cout << a;
					if(b>0) cout << "+i";
					if(b>1) cout << b;
					cout << std::endl;
				}
			} else {		//testcase 3 [|a| is prime AND |a| mod 4 = 3]
				int gp = abs(a);
				if ( binary_search(p34.begin(), p34.end(), gp) ) {
					// gp is gaussian
					gPrime wspace (a,b);	// workspace for complex prime
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
//----------------------------------------------------------------------
struct compgprimes {
	bool operator() (gPrime i, gPrime j) {
        bool result = ((i.real()<j.real())||((i.real()==j.real())&&(i.imag()<j.imag())));
        cout << std::endl << "testing " << i << "and" << j << "result " << result <<std::endl;
		// return true if i < j
		return result;
	};
} cxCompare;

bool gprime_lt(gPrime i, gPrime j) {
	// return true if i < j
	return ((i.real()<j.real())||((i.real()==j.real())&&(i.imag()<j.imag())));
}

bool cnode_lt(cNode i, cNode j) {
	// return true if vector i is 'less than' vector j
	if (gprime_lt(i[3],j[3])) return true;
	if (i[3] != j[3]) return false;
	// 1st terms ==
	if (gprime_lt(i[2],j[2])) return true;
	if (i[2] != j[2]) return false;
	// 2nd terms ==
	if (gprime_lt(i[1],j[1])) return true;
	if (i[1] != j[1]) return false;
	// 3rd terms ==
	if (gprime_lt(i[0],j[0])) return true;
	return false;
}

bool cnode_eq(cNode i, cNode j) {
	// return true if vector i == vector j
	return (i == j);
	}

//----------Read/Write Functions-----------

int write_nl(NodeList &nl);
int read_nl(NodeList &nl);

int write_nl(NodeList &nl) {

	std::string path = "./nodelist.dat";
	ofstream fout (path, ios::out|ios::binary);
	std::array<double,8> wspace;
	char * memblock = (char*)&wspace[0];

	if (fout.is_open()) {
		// serialise each cNode into an array of 4*8 bytes;
		for(auto a = nl.begin(); a != nl.end(); ++a) {
			auto it = wspace.begin();
			*it++ = (*a)[0].real();
			*it++ = (*a)[0].imag();
			*it++ = (*a)[1].real();
			*it++ = (*a)[1].imag();
			*it++ = (*a)[2].real();
			*it++ = (*a)[2].imag();
			*it++ = (*a)[3].real();
			*it++ = (*a)[3].imag();
			fout.write(memblock, sizeof(double)*8);
		}
		fout.close();
	}
	return 0;
}

int read_nl(NodeList &nl) {

	std::string path = "./nodelist.dat";
	streampos size;
	ifstream fin (path, ios::in|ios::binary|ios::ate);
	std::array<int,8> int_buffer;	// input buffer for 8 ints
	char * memblock;	// imput buffer location
	cNode cnode;		// workspace to create a node of 4 complex int

	if(fin.is_open()) {
		memblock = (char*)&int_buffer[0];
		size = fin.tellg();
		fin.seekg(0, ios::beg);
		while(size>0) {
			fin.read(memblock, sizeof(int)*8);
			std::complex<double> *it = (std::complex<double>*)&int_buffer[0];
			cnode.clear();
			for(auto x=0;x!=4;++x) {
				cnode.push_back(*it);
				cout << *it++;
			}
			cout << std::endl;
			nl.push_back(cnode);
			size -= sizeof(int)*8;
		}
		fin.close();
	} else {
		cout << "Error: could not open " << path << std::endl;
	}
	return 0;
}

//======================================================================

int main() {

	const int Limit = 10;
	gPrime Target;
	cNode gaussian;			// Storage for Gausian primes
	cNode::iterator rai;	//random access iterator
    std::unordered_multimap<gPrime,cNode, hash_X > umm_sums;
    
    cout << "Integer:" << sizeof(int) << std::endl;
    cout << "Double:"  << sizeof(double) << std::endl;
    cout << "Long:"    << sizeof(long) << std::endl;
    cout << "Size_t:"    << sizeof(size_t) << std::endl;

	// generate Gaussian primes up to Limit
	vect_gaussprimes(&gaussian, Limit);	
	// generate complex nodes and store in unordered multimap
    for(auto a=gaussian.begin(); a != gaussian.end(); ++a) {
    	for(auto b=gaussian.begin(); b != gaussian.end(); ++b) {
    		if(b==a) continue;
    		for(auto c=gaussian.begin(); c != gaussian.end(); ++c) {
    			if((c==a)||(c==b)) continue;
    			for(auto d=gaussian.begin(); d != gaussian.end(); ++d) {
    				if((d==a)||(d==b)||(d==c)) continue;
    				gPrime foo {(*a+*b+*c+*d)};
    				cNode bar { *a, *b, *c, *d };
    				KeyValue foobar = std::make_pair(foo,bar);
    				umm_sums.insert(foobar);
    			}
    		}
    	}
    }
 
	unsigned nbuckets = umm_sums.bucket_count();
	unsigned count = 0;
	unsigned idx;
	
	//Determine the size and index of biggest bucket
	for(unsigned i=0; i<nbuckets; ++i) {
		if(umm_sums.bucket_size(i) >= count) {
			idx = i;
			count = umm_sums.bucket_size(i);
		}
	}
	
	cout << "Bucket count:" << umm_sums.bucket_count() << " Bucket " << idx << " contains " << count << " entries.\n";
	auto b = umm_sums.cbegin(idx);
	Target = b->first;
	cout << "Associated sum:" << Target << std::endl;
	
	// populate the Nodelist with the selected entry from umm_sums.
	NodeList nl;
	while( b != umm_sums.cend(idx) ) {
		nl.push_back(b->second);
		++b;
	}

	// print final Nodelist
	cout << "==================" << std::endl;
	count = 0;
	for(auto a = nl.begin(); a != nl.end(); ++a) {
		for(auto b = (*a).begin(); b != (*a).end(); ++b) {
			cout << *b;
		}
		cout << std::endl;
		++count;
	}
	cout << "Nodelist size: " << count << std::endl;
	
	// DEBUG EXIT
	// exit(0);
	//===========
	
	// -----------------------------------------------------------------
	count = 0;
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
					
					++count; // Solution count
					
					std::array<gPrime,12> sig_array = {(*a)[0],(*a)[1],(*a)[2],(*a)[3],\
					(*b)[1],(*b)[2],(*b)[3],\
					(*c)[1],(*c)[2],(*c)[3],\
					(*d)[2],(*d)[3]};
					
					std::sort(&sig_array[0], &sig_array[12], gprime_lt);
					
					size_t hash = 0x12345678;
					for(auto x=0; x<12; ++x) {
						std::array<double,2> values = { sig_array[x].real(), sig_array[x].imag() };
						hash = fasthash64((const void*)&values, sizeof(double)*2, hash);
					}
					
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
} //end

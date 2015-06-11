/*
 * sums.cpp
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
#include <complex>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

using gPrime = std::complex<int>;
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
		// return true if i < j
		return ((i.real()<j.real())||((i.real()==j.real())&&(i.imag()<j.imag())));
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
	return (i==j);
	}
//======================================================================

int main() {
	
	const int Limit = 10;
	
	cNode gaussian;
	
	cNode::iterator rai;	//random access iterator
	
	vect_gaussprimes(&gaussian, Limit);    
    
    std::unordered_multimap<gPrime,cNode, hash_X > umm_sums;
    
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
    
    NodeList nl;
	unsigned nbuckets = umm_sums.bucket_count();
	unsigned count = 0;
	for(unsigned i=0; i<nbuckets; ++i) {
		// the sum 26 + i12 decomposes to 29 groups of 24. 1176 elements
		// bucket #32461
		//if(umm_sums.bucket_size(i) >= (24*49)) {
		if(i==32461) {
			++count;
			for(auto b = umm_sums.cbegin(i); b != umm_sums.cend(i); ++b) {
				gPrime gp = b->first;
				cNode cn = b->second;
				cout << gp.real() << "+" << gp.imag() << "\t";
				// sort the node elements (ascending)
				std::sort(cn.begin(), cn.end(), gprime_lt );
				// print the contents of the cNode (4 values)				
				for(auto c = cn.begin(); c != cn.end(); ++c) {
					cout << c->real() << "+i" << c->imag() << "\t";
				}
				cout << std::endl;
				// add the cNode to a vector of cNodes (NodeList)
				nl.push_back(cn);
			}			
			std::cout << "bucket #" << i << " has " << umm_sums.bucket_size(i) << " elements.\n";
			break;	// for(unsigned....
		}
	}
	// break jumps here
	// sort the cNodes in ascending order
	std::sort(nl.begin(), nl.end(), cnode_lt);
	for(auto a = nl.begin(); a != nl.end(); ++a) {
		for(auto b = (*a).begin(); b != (*a).end(); ++b) {
			cout << *b;
		}
		cout << std::endl;
	}	
	
	cout << count << " active buckets." << std::endl;
	// delete duplicate entries
	NodeList::iterator it;
	it = std::unique(nl.begin(), nl.end(), cnode_eq);
	nl.resize( std::distance(nl.begin(),it) );
	// print final vector
	cout << "==================" << std::endl;
	count = 0;
	for(auto a = nl.begin(); a != nl.end(); ++a) {
		for(auto b = (*a).begin(); b != (*a).end(); ++b) {
			cout << *b;
		}
		cout << std::endl;
		++count;
	}
	cout << "Final vector size: " << count << std::endl;
}

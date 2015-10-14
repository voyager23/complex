/*
 * analyse.cpp
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
 * 
 */


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <typeinfo>

#include "./fasthash.h"

//-----Declarations-----
using namespace std;
using gPrime = std::complex<double>;
using cNode = std::vector<gPrime>;		// normally 4 complex<double>
using cTocta = std::vector<cNode>;	// normally 4 cNodes

using NodeRing = std::array<gPrime,7>;	// a,b,c,d,a,b,c

void prt_ctocta(cTocta,gPrime);
size_t hash32(cTocta, int);
bool gprime_lt(gPrime,gPrime);

struct cToctaInfo {
	cTocta ctoc;
	int family;
};

void testfunction(const std::vector<cTocta>& vct, std::vector<std::array<NodeRing,3>>& nr);

//-----Definitions-----
void prt_ctocta(cTocta ct, gPrime target) {
	// print 4 rows of 4 complex primes and 1 blank line
	cout << "Target:" << target << std::endl;
	for(auto z = ct.begin(); z != ct.end(); ++z) {
		for(auto y = (*z).begin(); y != (*z).end(); ++y) cout << *y << "\t";
		cout << std::endl;
	}
	cout << std::endl;
}

bool gprime_lt(gPrime i, gPrime j) {
	// return true if i < j
	return ((i.real()<j.real())||((i.real()==j.real())&&(i.imag()<j.imag())));
}

size_t hash64(cTocta ct,int mode) {
	// mode 0 - calc hash for 12 sorted unique gPrimes.
	// mode 1 - calc hash for 16 unsorted unique gPrimes.
	
	std::vector<gPrime> sig_vector;
	
	if(mode==0) {
	// form sig_vector - a vector of gPrimes from the complex tocta
	sig_vector = {
	ct[0][0],ct[0][1],ct[0][2],ct[0][3],\
	ct[1][1],ct[1][2],ct[1][3],\
	ct[2][1],ct[2][2],ct[2][3],\
	ct[3][2],ct[3][3]
	};
	
	// sort the sig_vector				
	std::sort(sig_vector.begin(), sig_vector.end(), gprime_lt);
	} else {
		cout << "hash64 error - mode " << mode << " not implemented." << std::endl;
		exit(1);
	}
	
	// calculate the hash value as size_t								
	size_t hash = 0x12345678;
	for(auto x=sig_vector.begin(); x!=sig_vector.end(); ++x) {
		std::array<double,2> values = { (*x).real(), (*x).imag() };
		hash = fasthash64((const void*)&values, sizeof(double)*2, hash);
	}
	
	// return hash value
	return hash;
}

void testfunction(const std::vector<cTocta> &vct, std::vector<std::array<NodeRing,3>> &nr) {
	// first index selects an array of NodeRings
	// second index selects an array of gPrimes
	// third index selects a gPrime
	
	bool a0_found, a2_found, b2_found;
		
	nr.clear();
	for(auto a = vct.begin(); a != vct.end(); ++a) {
		// setup 3 NodeRing for comparison
		NodeRing a0 = { (*a)[0][0],(*a)[1][0],(*a)[2][0],(*a)[3][0], (*a)[0][0],(*a)[1][0],(*a)[2][0] };
		NodeRing a2 = { (*a)[0][2],(*a)[2][2],(*a)[2][3],(*a)[0][3], (*a)[0][2],(*a)[2][2],(*a)[2][3] };
		NodeRing b2 = { (*a)[1][2],(*a)[3][2],(*a)[3][3],(*a)[1][3], (*a)[1][2],(*a)[3][2],(*a)[3][3] };
		for(auto b = nr.begin(); b != nr.end(); ++b) {
			// scan for a0 ring
			for(int q = 0; q < 4; ++q) {
				a0_found = ( 
					(*b)[0][0] == a0[q+0] &&
					(*b)[0][1] == a0[q+1] &&
					(*b)[0][2] == a0[q+2] &&
					(*b)[0][3] == a0[q+3] 
					);
				if(a0_found)  { break; }
			}
			// a0_found breaks to here...
			if(a0_found) { // scan for a2 ring
				for(int q = 0; q < 4; ++q) {
					a2_found = ( 
						(*b)[1][0] == a2[q+0] &&
						(*b)[1][1] == a2[q+1] &&
						(*b)[1][2] == a2[q+2] &&
						(*b)[1][3] == a2[q+3] 
						);
					if(a2_found) { break; }
				}
			}
			// a2_found breaks to here...
			if(a0_found && a2_found) { // scan for b2 ring
				for(int q = 0; q < 4; ++q) {
					b2_found = ( 
						(*b)[2][0] == b2[q+0] &&
						(*b)[2][1] == b2[q+1] &&
						(*b)[2][2] == b2[q+2] &&
						(*b)[2][3] == b2[q+3] 
						);
					if(b2_found) { break; }
				}
			}
			// stop scanning if match found
			if(a0_found && a2_found && b2_found) break; 
		}
		// test for update
		if(a0_found && a2_found && b2_found) break;	// next cTocta 
		// else update the node rings list with new config
		// cout << "adding new config\n";
		nr.push_back(std::array<NodeRing,3> { a0, a2, b2 } );	
	}				
	cout << "nr has " << nr.size() << " entries." << std::endl;		
	cout << "\ntestfunction complete\n";
}

//-----Main Code-----
int main(int argc, char **argv)
{
	std::string path = "./cxtocta_real_imag.dat";
	std::ifstream is (path, std::ifstream::binary);
	
	// get length of file
	is.seekg(0,is.end);
	int length = is.tellg();
	is.seekg(0,is.beg);
	
	// sanity checks
	if((length % 256)!=0) {
		cout << "Warning: Length mod 256 = " << (length%256) << std::endl;
		exit(1);
	} else {	
		char *isbuffer = new char [length];
		cTocta ct;
		cNode cn;
		gPrime gp,target;
		std::unordered_multimap<size_t, cTocta> umm_families;
		
		int read_count = 0;
		int insert_count = 0;
		
		// reassemble cToctas from 16 std::complex<double>
		while(1) {
			is.read(isbuffer, sizeof(double) * 32);
			if(is.eof()) break;
			double *ri = (double*)isbuffer;	++read_count;
			ct.clear();			
			for(auto x=0; x<4; x++) {
				cn.clear();
				target = {0,0};
				for(auto y=0; y<4; ++y) {
					gp={*(ri), *(ri+1)};
					ri++; ri++;
					cn.push_back(gp);
					target += gp;
				}
				ct.push_back(cn);
			}
			size_t signature = hash64(ct,0);
	
			// construct key - value pair  insert into unordered_multimap
			umm_families.insert(std::pair<size_t, cTocta> { signature,ct } );	++insert_count;
		} // while ...
		
		int ndatablocks = length / 256;
		int nfamilies = ndatablocks / 48;
		cout << "File length consistent.\t";
		cout << "DataBlocks=" << ndatablocks << "\tFamilies=" << nfamilies;
		cout << std::endl;
		
		cout << "read_count:" << read_count << std::endl;
		cout << "insert_count:" << insert_count << std::endl;
		
		std::vector<cTocta> vct;
		std::vector<cTocta>& vct_ref = vct;
		
		std::vector<std::array<NodeRing,3>> node_rings;
		std::vector<std::array<NodeRing,3>>& nr_ref = node_rings;
		
		std::vector<NodeRing> a0_rings;
		NodeRing a0_current;
		
		// start by generating a vector of cToctas from a family
		for(unsigned a = 0; a < umm_families.bucket_count(); ++a) {
			if(umm_families.bucket_size(a) > 48) {
				vct.clear();
				for(auto b = umm_families.cbegin(a); b != umm_families.cend(a); ++b) {
					vct.push_back(b->second);
				}
				cout << std::endl << "cTocta count:" << vct.size() << std::endl;
				
				//------------------------------------------------------
				
				testfunction(vct_ref, nr_ref);
				// print the contents of node rings
#if(0)				
				for(auto a = node_rings.begin(); a != node_rings.end(); ++a) {
					for(int p = 0; p < 3; ++p) {
						for(int q = 0; q < 4; ++q) cout << (*a)[p][q] << "\t";
						cout << std::endl;
					}
					cout << std::endl;
				}
#endif		
				//------------------------------------------------------
				
			} // if size > 0...
		} // for a...
	} // else ...
	is.close();
	return 0;
}


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
size_t hash32(cTocta);
bool gprime_lt(gPrime,gPrime);

void testfunction(const std::vector<cTocta>, std::vector<NodeRing>);

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

size_t hash64(cTocta ct) {
	// form sig_vector - a vector of gPrimes from the complex tocta
	
	std::vector<gPrime> sig_vector = 
	{ct[0][0],ct[0][1],ct[0][2],ct[0][3],\
	ct[1][1],ct[1][2],ct[1][3],\
	ct[2][1],ct[2][2],ct[2][3],\
	ct[3][2],ct[3][3]};
	
	// sort the sig_vector				
	std::sort(sig_vector.begin(), sig_vector.end(), gprime_lt);
	
	// calculate the hash value as size_t								
	size_t hash = 0x12345678;
	for(auto x=sig_vector.begin(); x!=sig_vector.end(); ++x) {
		std::array<double,2> values = { (*x).real(), (*x).imag() };
		hash = fasthash64((const void*)&values, sizeof(double)*2, hash);
	}
	
	// return hash value
	return hash;
}

void testfunction(const std::vector<cTocta> vct, std::vector<NodeRing> nr) {

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
			size_t signature = hash64(ct);
			
			// determine family from fasthash32
			if( signature == 11653120196325356709U) {
				prt_ctocta(ct,target);
				cout << signature << std::endl;
			}
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
		
		std::vector<std::array<NodeRing,3>> noderings;
		
		std::vector<NodeRing> a0_rings;
		NodeRing a0_current;
		bool found;
		
		testfunction(vct, a0_rings);
		
		// start by generating a vector of cToctas from a family
		for(unsigned a = 0; a < umm_families.bucket_count(); ++a) {
			if(umm_families.bucket_size(a) > 0) {
				vct.clear();
				for(auto b = umm_families.cbegin(a); b != umm_families.cend(a); ++b) {
					vct.push_back(b->second);
				}
				cout << std::endl << "cTocta count:" << vct.size() << std::endl;
				// analyse each cTocta in terms of the a0_ring configuration
				a0_rings.clear();
				for(auto c = vct.begin(); c != vct.end(); ++c) {
					a0_current = { (*c)[0][0],(*c)[1][0],(*c)[2][0],(*c)[3][0], (*c)[0][0],(*c)[1][0],(*c)[2][0] };
					// search a0_rings
					found = false;
					for(auto p = a0_rings.begin(); p != a0_rings.end(); ++p) {
						for(int q = 0; q < 4; ++q) {
							found = ( 
							(*p)[q+0] == a0_current[0] &&
							(*p)[q+1] == a0_current[1] &&
							(*p)[q+2] == a0_current[2] &&
							(*p)[q+3] == a0_current[3] 
							);
							if(found) break;
						}
						if(found) break;
					}
					if(!(found)) a0_rings.push_back(a0_current);
				}
				// report size() of a0_rings
				cout << "a0_rings - found: " << a0_rings.size() << std::endl;
				// print out the contents of a0_rings
				for(auto p = a0_rings.begin(); p != a0_rings.end(); ++p) {
					for(int q = 0; q < 4; ++q) cout << (*p)[q] << "\t";
					cout << std::endl;
				}			
			} // if size > 0...
		} // for a...
	} // else ...
	is.close();
	return 0;
}


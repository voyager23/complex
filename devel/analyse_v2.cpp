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
	std::complex<double> group;
	int species;
	cTocta ctocta;
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

//-----Main Code-----
int main(int argc, char **argv)
{
	std::string path = "./cxtocta_real_imag.dat";
	std::ifstream is (path, std::ifstream::binary);

	std::vector<cToctaInfo> vcti;
	
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
		cToctaInfo cti;
		
		int read_count = 0;
		int insert_count = 0;
		
		// reassemble cToctas from 16 std::complex<double>
		vcti.clear();
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
			// place all cToctas into a single vector
			cti.ctocta = ct;
			cti.species = -1;
			vcti.push_back(cti);	++insert_count;
		} // while ...
		
		int ndatablocks = length / 256;
		int nfamilies = ndatablocks / 48;
		cout << "File length consistent.\t";
		cout << "DataBlocks=" << ndatablocks << "\tFamilies=" << nfamilies;
		cout << std::endl;		
		cout << "read_count:" << read_count << std::endl;
		cout << "insert_count:" << insert_count << std::endl;
		
		int species_id = 0;
		for(auto a = vcti.begin(); a != vcti.end(); ++a) {
			if(a->species == -1) continue;
		}
				
	} // else ...
	is.close();
	return 0;
}


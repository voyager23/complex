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

#include "./fasthash.h"

//-----Declarations-----
using namespace std;
using gPrime = std::complex<double>;
using cNode = std::vector<gPrime>;		// normally 4 complex<double>
using cTocta = std::vector<cNode>;	// normally 4 cNodes

void prt_ctocta(cTocta,gPrime);
size_t hash32(cTocta);

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

size_t hash32(cTocta) {
	return 0;
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
		int ndatablocks = length / 256;
		int nfamilies = ndatablocks / 48;
		cout << "File length consistent." << std::endl;
		cout << "DataBlocks=" << ndatablocks << "\tFamilies=" << nfamilies;
		cout << std::endl;
		
		char *isbuffer = new char [length];
		cTocta ct;
		cNode cn;
		gPrime gp,target;
		std::unordered_multimap<size_t, cTocta> umm_families;
		while(!(is.eof())) {
			is.read(isbuffer, sizeof(double) * 32);
			double *ri = (double*)isbuffer;
			ct.clear();			
			for(auto x=0; x<4; x++) {
				cn.clear();
				target = {0,0};
				for(auto y=0; y<4; ++y) {
					gp={*(ri++), *(ri++)};
					cn.push_back(gp);
					target += gp;
				}
				ct.push_back(cn);
			}
			prt_ctocta(ct,target);
			// determine family from fasthash32
			size_t signature = hash32(ct);
			// construct key - value pair  insert into unordered_multimap
			umm_families.insert(std::make_pair<size_t,cTocta> (signature,ct) );
		}
	}
	is.close();
	return 0;
}


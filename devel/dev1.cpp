/*
 * dev1.cpp
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
 * Develop a method to write and read a list of nodes to a binary file
 * 
 */


#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

using gPrime = std::complex<int>;
using cNode = std::vector<gPrime>;
using NodeList = std::vector<cNode>;
using KeyValue = std::pair<gPrime,cNode>;


int main(int argc, char **argv)
{
	gPrime p0 (4,8);
	gPrime p1 (2,5);
	gPrime p2 (3,6);
	gPrime p3 (1,7);
	
	cNode cn;
	NodeList nl;
	// construct a Nodelist of two cNodes
	cn.push_back(p0);
	cn.push_back(p1);
	cn.push_back(p2);
	cn.push_back(p3);
	nl.push_back(cn);
	
	cn.clear();
	cn.push_back(p3);
	cn.push_back(p2);
	cn.push_back(p1);
	cn.push_back(p0);
	nl.push_back(cn);
	
	std::string path = "./nodelist.dat";
	ofstream fout (path, ios::out|ios::binary);
	std::array<int,8> wspace;
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

			
			fout.write(memblock, sizeof(int)*8);			
		}
		fout.close();
	}
	// read the nodelist back
	streampos size;
	ifstream fin (path, ios::in|ios::binary|ios::ate);
	if(fin.is_open()) {
		memblock = (char*)&wspace[0];
		size = fin.tellg();
		fin.seekg(0, ios::beg);
		while(size>0) {
			fin.read(memblock, sizeof(int)*8);
			std::complex<int> *it = (std::complex<int>*)&wspace[0];
			for(auto x=0;x!=4;++x) cout << *it++;
			cout << std::endl;
			size -= sizeof(int)*8;
		}
		fin.close();		
	}
	cout << "Complete\n";
	return 0;
}


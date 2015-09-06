/*
 * dev3.cpp
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
 * Replace gPrime definition with a class
 *
 */


#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>

using namespace std;


//======================================================================
class gPrime {
	// gPrime is a complex integer
	private:
	int real,imag;
	double modulus;

	public:
	gPrime () { real = 0; imag = 0; modulus = 0.0; };
	gPrime (int i, int j) {
		real = i; imag = j;
		modulus = sqrt((double)(real*real) + (double)(imag*imag));
		};
	int get_real() { return real; };
	int get_imag() { return imag; };
	double get_mod() { return modulus; };
	bool gprime_lt(gPrime j) {
		// return true if (this-gPrime < j)
		return ((this->real<j.real)||((this->real==j.real)&&(this->imag<j.imag)));
	}
	bool gprime_eq(gPrime j) {
		return ((this->real==j.real)&&(this->imag==j.imag)&&(this->modulus==j.modulus));
	};
	void prt_gprime() {
		cout << "(" << this->real << "," << this->imag << ")";
	};
};

//======================================================================
class cNode {
	// cNode is an -array- of 4 gPrimes
	private:
		std::array<gPrime,4> cn;
	public:
		cNode ();
		cNode (gPrime,gPrime,gPrime,gPrime);
		bool cnodes_lt(cNode i, cNode j);
		bool cnodes_eq(cNode i, cNode j);
		gPrime operator[](int n) { return cn[n]; };
		void prt_cnode();
};

// constructors
cNode::cNode () {  };
cNode::cNode (gPrime a, gPrime b, gPrime c, gPrime d) {
	cn[0]=a; cn[1]=b; cn[2]=c; cn[3]=d;
};
// compare functions
bool cNode::cnodes_lt(cNode i, cNode j) {
	// return true if cNode i is 'less than' cNode j

	if (i[3].gprime_lt(j[3])) return true;
	if (!(i[3].gprime_eq(j[3]))) return false;

	// 1st terms ==
	if (i[2].gprime_lt(j[2])) return true;
	if (!(i[2].gprime_eq(j[2]))) return false;

	// 1st terms ==
	if (i[1].gprime_lt(j[1])) return true;
	if (!(i[1].gprime_eq(j[1]))) return false;

	// 3rd terms ==
	if (i[0].gprime_lt(j[0])) return true;
	return false;
}

bool cNode::cnodes_eq(cNode i, cNode j) {
	// return true if cNode i == cNode j
	return  (i[0].gprime_eq(j[0]) && i[0].gprime_eq(j[0]) && i[0].gprime_eq(j[0]) && i[0].gprime_eq(j[0]));
}
//output
void cNode::prt_cnode() {
	for(auto a = this->cn.begin(); a != this->cn.end(); ++a) a->prt_gprime();
	cout << std::endl;
}

//=====================Read/Write Functions=============================
using NodeList = std::vector<cNode>;

int write_nl(NodeList &nl);
int read_nl(NodeList &nl);
std::string path = "./nodelist.dat";

int write_nl(NodeList &nl) {

	ofstream fout (path, ios::out|ios::binary);
	std::array<int,8> wspace;
	char * memblock = (char*)&wspace[0];

	if (fout.is_open()) {
		// serialise each cNode into an array of 4*8 bytes;

		for(auto a = nl.begin(); a != nl.end(); ++a) {
			auto it = wspace.begin();

			*it++ = (*a)[0].get_real();
			*it++ = (*a)[0].get_imag();
			*it++ = (*a)[1].get_real();
			*it++ = (*a)[1].get_imag();
			*it++ = (*a)[2].get_real();
			*it++ = (*a)[2].get_imag();
			*it++ = (*a)[3].get_real();
			*it++ = (*a)[3].get_imag();

			fout.write(memblock, sizeof(int)*8);
		}
		fout.close();
	}

	return 0;
}

int read_nl(NodeList &nl) {

	streampos size;
	ifstream fin (path, ios::in|ios::binary|ios::ate);
	std::array<int,8> wspace;
	char * memblock = (char*)&wspace[0];
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
	return 0;
}

//=============================Main=====================================
int main(int argc, char **argv)
{
	gPrime p0(-4,8);
	gPrime p1(3,6);
	gPrime p2(3,6);
	gPrime p3(1,-7);

	cNode foo (gPrime(9,9),p1,p2,p3);
	cNode bar = foo;

	foo.prt_cnode();
	bar.prt_cnode();

	cout << p0.get_real() << "," << p0.get_imag() << ","<< p0.get_mod() << std::endl;

	if (p1.gprime_eq(p2)) {
		cout << "gPrimes equal\n";
	} else {
		cout << "gPrimes not equal\n";
	};

	return 0;
}

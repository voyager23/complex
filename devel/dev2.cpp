/*
 * dev2.cpp
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
using cNode_t = std::vector<gPrime>;

class cNode {
	// cNode is a vector of 4 gPrimes
	private:
	
	bool gprime_lt(gPrime i, gPrime j) {
	// return true if i < j
	return ((i.real()<j.real())||((i.real()==j.real())&&(i.imag()<j.imag())));
	}
	
	cNode_t cn;
	
	public:
		cNode ();
		cNode (gPrime,gPrime,gPrime,gPrime);

		bool cnodes_lt(cNode i, cNode j);
		bool cnodes_eq(cNode i, cNode j);
		gPrime operator[](int n) { return cn[n]; };
		size_t size() { return cn.size(); };
		cNode_t cnode_get();
		void prt_cnode();
};

// default constructor
cNode::cNode () { cn.clear(); };

cNode::cNode (gPrime p,gPrime q,gPrime r,gPrime s) {
	cn.clear(); 
	cn.push_back(p);
	cn.push_back(q);
	cn.push_back(r);
	cn.push_back(s);
};



bool cNode::cnodes_lt(cNode i, cNode j) {
	// return true if cNode i is 'less than' cNode j
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

bool cNode::cnodes_eq(cNode i, cNode j) {
	// return true if cNode i == cNode j
	return ((i[0] == j[0])&&(i[1] == j[1])&&
			(i[2] == j[2])&&(i[3] == j[3])&&
			(i.size() == j.size()));
}

cNode_t cNode::cnode_get() { return cn; };

void cNode::prt_cnode() {
	for(auto a = this->cn.begin(); a != this->cn.end(); ++a) cout << *a << ",";
	cout << "\b \n";
}


int main(int argc, char **argv)
{
	gPrime p0(-4,8);
	gPrime p1(2,5);
	gPrime p2(3,6);
	gPrime p3(1,-7);
	
	cNode foo (gPrime(9,9),p1,p2,p3);
	cNode_t bar = foo.cnode_get();
	cNode biz = foo;
	biz.prt_cnode();
	foo.prt_cnode();	

	return 0;
}


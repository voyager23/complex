/*
 * analyse.hpp
 * 
 * Copyright 2015 Mike <mike@fc22>
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


#ifndef __CTOCTA__
	#define __CTOCTA__
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
	using cTocta = std::vector<cNode>;		// normally 4 cNodes
	using NodeRing = std::array<gPrime,7>;	// a,b,c,d,a,b,c

	struct cToctaInfo {	
		cTocta ctocta;
		int group, sub_group;
		size_t signature;
	};

	void prt_ctocta(cTocta);
	void prt_ctoctainfo(cToctaInfo);
	size_t hash64(cTocta, int);
	bool gprime_lt(gPrime,gPrime);
	void testfunction(const std::vector<cTocta>& vct, std::vector<std::array<NodeRing,3>>& nr);
	bool compare_cTocta(cTocta &l, cTocta &r);
	
	void complex_apply_abcd(cTocta &t);
	void complex_apply_ef(cTocta &t);
	void complex_apply_bd(cTocta &t);
	void complex_apply_ac(cTocta &t);

#endif


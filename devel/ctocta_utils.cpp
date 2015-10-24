/*
 * ctocta_utils.cpp
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
 * This file contains the routines for reflecting and rotating a complex tocta
 */


#include "analyse.hpp"

//--------------------------Definitions---------------------------------

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

//----------------------------------------------------------------------
bool compare_cTocta(cTocta &l, cTocta &r) {
	// return false if cToctas are not equal
	for(int x=0; x<4; x++) 
		for(int y=0; y<4; ++y) 
			if (l[x][y] != r[x][y]) return false;
	// matching cToctas
	return true;
}

void prt_ctocta(cTocta ct) {
	
	// derive  and print target value
	gPrime target = {0,0};
	auto z = ct.begin();
	for(auto y = (*z).begin(); y != (*z).end(); ++y) target += *y;	
	cout << "Target:" << target << std::endl;
	
	// print 4 rows of 4 complex primes and 1 blank line
	for(auto z = ct.begin(); z != ct.end(); ++z) {
		for(auto y = (*z).begin(); y != (*z).end(); ++y) cout << *y << "\t";
		cout << std::endl;
	}
	//cout << std::endl;
}

void prt_ctoctainfo(cToctaInfo cti) {
	prt_ctocta(cti.ctocta);
	cout << "Group:" << cti.group << "\tSub_group:" << cti.sub_group << std::endl;
	cout << "Signature:" << cti.signature << std::endl;
}

//----------------------------------------------------------------------
void complex_apply_abcd(cTocta &t) {
	// apply reflection about horizontal plane abcd
	// modifies data pointed to by t
	gPrime primes[16];	
	// record the new config in array
	primes[0]=t[0][0];
	primes[1]=t[0][1];
	primes[2]=t[0][3];
	primes[3]=t[0][2];
	
	primes[4]=t[1][0];
	primes[5]=t[1][1];
	primes[6]=t[1][3];
	primes[7]=t[1][2];
	
	primes[8]=t[2][0];
	primes[9]=t[2][1];
	primes[10]=t[2][3];
	primes[11]=t[2][2];
	
	primes[12]=t[3][0];
	primes[13]=t[3][1];
	primes[14]=t[3][3];
	primes[15]=t[3][2];
	// copy back to source tocta
	for(int i=0;i<16;i++) t[(i/4)][(i%4)] = primes[i];
	// complex_apply_abcd
}
//----------------------------------------------------------------------
void complex_apply_ef(cTocta &t){
	// apply rotation around vertical e-f axis
	// modifies the data pointed to by t
	gPrime primes[16];	
	// record the new config in array
	primes[0]=t[3][0];
	primes[1]=t[3][1];
	primes[2]=t[3][2];
	primes[3]=t[3][3];
	
	primes[4]=t[0][0];
	primes[5]=t[0][1];
	primes[6]=t[0][2];
	primes[7]=t[0][3];
	
	primes[8]=t[0][1];
	primes[9]=t[1][1];
	primes[10]=t[1][2];
	primes[11]=t[1][3];
	
	primes[12]=t[2][0];
	primes[13]=t[2][1];
	primes[14]=t[2][2];
	primes[15]=t[2][3];
	// copy back to source tocta
	for(int i=0;i<16;i++) t[(i/4)][(i%4)] = primes[i];
	// complex_apply_ef
}
//----------------------------------------------------------------------
void complex_apply_bd(cTocta &t){
	// apply rotation around diagonal b-d axis
	// modifies the data pointed to by t
	gPrime primes[16];
	// record the new config in array	
	primes[0]=t[3][3];	// 15
	primes[1]=t[1][3];	// 7
	primes[2]=t[0][3];	// 3
	primes[3]=t[2][3];	// 11
	
	primes[4]=t[1][3];	// 7
	primes[5]=t[1][2];	// 6
	primes[6]=t[1][0];	// 4
	primes[7]=t[1][1];	// 5
	
	primes[8]=t[1][2];	// 6
	primes[9]=t[3][2];	// 14
	primes[10]=t[0][2];	// 2
	primes[11]=t[2][2];	// 10
	
	primes[12]=t[3][2];	// 14
	primes[13]=t[3][3];	// 15
	primes[14]=t[0][0];	// 13 (0)
	primes[15]=t[2][1];	// 12 (9)
	// copy back to source tocta
	for(int i=0;i<16;i++) t[(i/4)][(i%4)] = primes[i];
	// complex_apply_bd
}
//----------------------------------------------------------------------
void complex_apply_ac(cTocta &t) {
	// apply rotation around diagonal a_c axis
	// modifies the data pointed to by t
	gPrime primes[16];
	// record the new config in array	
	primes[0]=t[0][3];
	primes[1]=t[0][2];
	primes[2]=t[0][0];
	primes[3]=t[0][1];
	
	primes[4]=t[0][2];
	primes[5]=t[2][2];
	primes[6]=t[3][2];
	primes[7]=t[1][2];
	
	primes[8]=t[2][2];
	primes[9]=t[2][3];
	primes[10]=t[2][1];
	primes[11]=t[1][1];
	
	primes[12]=t[2][3];
	primes[13]=t[0][3];
	primes[14]=t[3][3];
	primes[15]=t[1][3];	
	// copy back to source tocta
	for(int i=0;i<16;i++) t[(i/4)][(i%4)] = primes[i];
}




/*
 * cts.cpp
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
 * 1) Input a complex target.
 * 2) Assess number of unique gPrimes required to sum to target.
 * 3) If number of unique gPrimes >=12:
 *		Generate a Nodelist from the restricted list of gPrimes.
 * 		Do 4 index search for solutions which sum to target.
 * 		Save results.
 * 4) Done
 */


#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

using gPrime = std::complex<int>;
using cNode = std::vector<gPrime>;
using NodeList = std::vector<cNode>;
using KeyValue = std::pair<gPrime,cNode>;

int main(int argc, char **argv)
{
	double real,imag;
	std::complex<double> Target;
	
	cout << "Complex Tocta search program." << std::endl;
	cout << "Enter real part of target:";
	cin >> real;
	cout << "Enter imag part of target:";
	cin >> imag;
	Target = {real,imag};
	cout << "Target requested:" << Target << std::endl;
	
	
	
	return 0;
}


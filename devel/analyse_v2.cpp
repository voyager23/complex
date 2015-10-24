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

#include "analyse.hpp"


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
		std::unordered_multimap<size_t, cToctaInfo> umm_input;
		std::unordered_multimap<size_t, cToctaInfo> umm_groups;
		
		int read_count = 0;
		int insert_count = 0;
		int group_count = 1;
		
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
			// place all cToctas into a single vector
			cti.ctocta = ct;
			cti.group = group_count++;
			cti.sub_group = 0;
			cti.signature = hash64(ct,0);
			// form a key:value pair and insert
			umm_input.insert(std::make_pair(cti.signature,cti));
			++insert_count;
		} // while ...
		
		int ndatablocks = length / 256;
		int nfamilies = ndatablocks / 48;
		cout << "File length consistent.\n";
		cout << "DataBlocks=" << ndatablocks << "\tFamilies?=" << nfamilies << std::endl;		
		cout << "read_count:" << read_count << "\tinsert_count:" << insert_count << std::endl;
		
		// scan the vector of cToctaInfo and assign to sub_groups
		for(unsigned a = 0; a != umm_input.bucket_count(); ++a) {
		
			if(umm_input.bucket_size(a) >= 48) {
				cout << "\nBucket size:" << umm_input.bucket_size(a) << std::endl;
				cToctaInfo workspace;
				int sub_group_idx = 0;
				
				for(auto it = umm_input.begin(a); it != umm_input.end(a); ++it) {
					workspace = it->second;
					if(workspace.sub_group != 0) continue;	// already processed
					//prt_ctoctainfo(workspace);
					// This cTocta is not assigned to a sub_group						
					sub_group_idx += 1;			// new sub_group index
					cTocta working = workspace.ctocta;									
//...................First Search Section......................................
					// calc and process the first 16 rotations
					for(int bd=0; bd<4; bd++) {
						for(int ef=0; ef<4; ef++) {
							// find this cTocta in container
							bool flag;
							for(auto p = umm_input.begin(a); p != umm_input.end(a); ++p) {
								flag = false;
								if(compare_cTocta(working, p->second.ctocta)) {
									//cout << "Found\n";
									flag = true;
									if(p->second.sub_group != 0) {
										cout << "Warning: none-zero sub_group value " << p->second.sub_group << std::endl;
									} else {
										p->second.sub_group = sub_group_idx;
										p->second.group = a;
									}
									// prt_ctoctainfo(p->second); cout << std::endl;
									break;
								}
							}
							// 		error if not found
							if(!flag)  cout << "Warning: Working cTocta not found in container." << std::endl;
							// rotate to next config
							complex_apply_ef(working);
						} // for ef...
						complex_apply_bd(working);
					} // for bd...
				
					// get next 4 configurations
					complex_apply_ac(working);
					
					for(int ef=0; ef<4; ef++) {
						// find this cTocta in container
						bool flag;
						for(auto p = umm_input.begin(a); p != umm_input.end(a); ++p) {
							cToctaInfo local = p->second;
							flag = false;
							if(compare_cTocta(working, p->second.ctocta)) {
								//cout << "Found\n";
								flag = true;
								if(p->second.sub_group != 0) {
									cout << "Warning: none-zero sub_group value " << p->second.sub_group << std::endl;
								} else {
									p->second.sub_group = sub_group_idx;
									p->second.group = a;
								}
								// prt_ctoctainfo(p->second); cout << std::endl;
								break;
							}
						}
						// 		error if not found
						if(!flag)  cout << "Warning: Working cTocta not found in container." << std::endl;
						// rotate to next config
						complex_apply_ef(working);
					} // for ef...
					
					// get last 4 configurations
					complex_apply_ac(working);
					complex_apply_ac(working);
				
					for(int ef=0; ef<4; ef++) {
						// find this cTocta in container
						bool flag;
						for(auto p = umm_input.begin(a); p != umm_input.end(a); ++p) {
							cToctaInfo local = p->second;
							flag = false;
							if(compare_cTocta(working, p->second.ctocta)) {
								//cout << "Found\n";
								flag = true;
								if(p->second.sub_group != 0) {
									cout << "Warning: none-zero sub_group value " << p->second.sub_group << std::endl;
								} else {
									p->second.sub_group = sub_group_idx;
									p->second.group = a;
								}
								// prt_ctoctainfo(p->second); cout << std::endl;
								break;
							}
						}
						// error if not found
						if(!flag)  cout << "Warning: Working cTocta not found in container." << std::endl;
						// rotate to next config
						complex_apply_ef(working);
					} // for ef...
					
					// rotate to initial config
					complex_apply_ac(working);
					
					// Reflect about horizontal plane abcd for next 24 configurations
					complex_apply_abcd(working);
					
// ..................Search reflections...............................
					// calc and process the first 16 rotations
					for(int bd=0; bd<4; bd++) {
						for(int ef=0; ef<4; ef++) {
							// find this cTocta in container
							bool flag;
							for(auto p = umm_input.begin(a); p != umm_input.end(a); ++p) {
								flag = false;
								if(compare_cTocta(working, p->second.ctocta)) {
									//cout << "Found\n";
									flag = true;
									if(p->second.sub_group != 0) {
										cout << "Warning: none-zero sub_group value " << p->second.sub_group << std::endl;
									} else {
										p->second.sub_group = sub_group_idx;
										p->second.group = a;
									}
									//prt_ctoctainfo(p->second); cout << std::endl;
									break;
								}
							}
							// 		error if not found
							if(!flag)  cout << "Warning: Working cTocta not found in container." << std::endl;
							// rotate to next config
							complex_apply_ef(working);
						} // for ef...
						complex_apply_bd(working);
					} // for bd...
				
					// get next 4 configurations
					complex_apply_ac(working);
					
					for(int ef=0; ef<4; ef++) {
						// find this cTocta in container
						bool flag;
						for(auto p = umm_input.begin(a); p != umm_input.end(a); ++p) {
							cToctaInfo local = p->second;
							flag = false;
							if(compare_cTocta(working, p->second.ctocta)) {
								//cout << "Found\n";
								flag = true;
								if(p->second.sub_group != 0) {
									cout << "Warning: none-zero sub_group value " << p->second.sub_group << std::endl;
								} else {
									p->second.sub_group = sub_group_idx;
									p->second.group = a;
								}
								//prt_ctoctainfo(p->second); cout << std::endl;
								break;
							}
						}
						// 		error if not found
						if(!flag)  cout << "Warning: Working cTocta not found in container." << std::endl;
						// rotate to next config
						complex_apply_ef(working);
					} // for ef...
					
					// get last 4 configurations
					complex_apply_ac(working);
					complex_apply_ac(working);
				
					for(int ef=0; ef<4; ef++) {
						// find this cTocta in container
						bool flag;
						for(auto p = umm_input.begin(a); p != umm_input.end(a); ++p) {
							cToctaInfo local = p->second;
							flag = false;
							if(compare_cTocta(working, p->second.ctocta)) {
								//cout << "Found\n";
								flag = true;
								if(p->second.sub_group != 0) {
									cout << "Warning: none-zero sub_group value " << p->second.sub_group << std::endl;
								} else {
									p->second.sub_group = sub_group_idx;
									p->second.group = a;                                
									// TODO: MODIFY SIGNATURE - 6 PLACES- SEE NOTES
								}
								//prt_ctoctainfo(p->second); cout << std::endl;
								break;
							}
						}
						// error if not found
						if(!flag)  cout << "Warning: Working cTocta not found in container." << std::endl;
						// rotate to next config
						complex_apply_ef(working);
					} // for ef...
					
					// rotate to initial config
					complex_apply_ac(working);

				}// for bucket iterator
				
				// print the unsorted bucket contents
				cout << "======================================\n";
				for(auto p = umm_input.begin(a); p != umm_input.end(a); ++p) prt_ctoctainfo(p->second);
				
			} // if bucket size ...
		}// get next bucket			
	} // sanity check ok
	is.close();
	return 0;
}


#include <iostream>
#include <complex>
#include <vector>
#include <array>
#include <unordered_map>

using namespace std;
typedef std::complex<int> C;
struct hash_X{
  size_t operator()(const complex<int> &x) const{
	size_t hashcode;
	
	//hashcode = (hashcode * 37) + x.real();
	//hashcode = (hashcode * 37) + x.imag();
	
	hash<int> f;
	hashcode = f( x.real() ) ^ f( x.imag() );
	
  return hashcode;
  };
};

int main() {

    vector< complex<int> >  gprimes
     { 
    std::complex<int>(1,1),std::complex<int>(7,0),std::complex<int>(10,9),
    std::complex<int>(14,1),std::complex<int>(17,2),std::complex<int>(19,10),
    std::complex<int>(2,1),std::complex<int>(7,2),std::complex<int>(11,0),
    std::complex<int>(14,9),std::complex<int>(17,8),std::complex<int>(19,14),
    std::complex<int>(3,0),std::complex<int>(8,3),std::complex<int>(11,4),
    std::complex<int>(14,11),std::complex<int>(17,10),std::complex<int>(19,16),
    std::complex<int>(3,2),std::complex<int>(8,5),std::complex<int>(11,6),
    std::complex<int>(15,2),std::complex<int>(17,12),std::complex<int>(20,1),
    std::complex<int>(4,1),std::complex<int>(8,7),std::complex<int>(12,7),
    std::complex<int>(15,4),std::complex<int>(18,5),std::complex<int>(20,3),
    std::complex<int>(5,2),std::complex<int>(9,4),std::complex<int>(13,2),
    std::complex<int>(15,14),std::complex<int>(18,7),std::complex<int>(20,7),
    std::complex<int>(5,4),std::complex<int>(10,1),std::complex<int>(13,8),
    std::complex<int>(16,1),std::complex<int>(18,17),std::complex<int>(20,11),
    std::complex<int>(6,1),std::complex<int>(10,3),std::complex<int>(13,10),
    std::complex<int>(16,5),std::complex<int>(19,0),std::complex<int>(20,13),
    std::complex<int>(6,5),std::complex<int>(10,7),std::complex<int>(13,12),
    std::complex<int>(16,9),std::complex<int>(19,6),std::complex<int>(20,19)
    };
    
    std::unordered_multimap<std::complex<int>,std::vector<std::complex<int>>, hash_X > umm_sums;
    
    for(auto a=gprimes.begin(); a != gprimes.end(); ++a) {
    	for(auto b=gprimes.begin(); b != gprimes.end(); ++b) {
    		if(b==a) continue;
    		for(auto c=gprimes.begin(); c != gprimes.end(); ++c) {
    			if((c==a)||(c==b)) continue;
    			for(auto d=gprimes.begin(); d != gprimes.end(); ++d) {
    				if((d==a)||(d==b)||(d==c)) continue;
    				std::complex<int> foo(*a+*b+*c+*d);
    				std::vector<std::complex<int>> bar { *a, *b, *c, *d };
    				std::pair< std::complex<int>, std::vector<std::complex<int> > > foobar;
    				foobar = std::make_pair(foo,bar);
    				umm_sums.insert(foobar);
    			}
    		}
    	}
    }
}

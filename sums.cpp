#include <iostream>
#include <complex>
#include <vector>
#include <array>
#include <unordered_map>

using namespace std;
struct hash_X{
  size_t operator()(const std::complex<int> &x) const{
	size_t hashcode = 23;
	hashcode = (hashcode * 37) + x.real();
	hashcode = (hashcode * 37) + x.imag();
  return hashcode;
  };
};



int main() {

    std::complex<int> gp1 (-7,3);
    std::complex<int> gp2 (-7,3);
    
    if(gp1==gp2) {
    	cout << "Primes match" << endl;
    } else {
    	cout << "No match" << endl;
    }

    std::cout << "Real part " << std::real(gp1) << std::endl;
    std::cout << "Imag part " << std::imag(gp1) << std::endl;

    std::vector<std::complex<int>> gprimes 
    { 
    (1,1),(7,0),(10,9),(14,1),(17,2),(19,10),
    (2,1),(7,2),(11,0),(14,9),(17,8),(19,14),
    (3,0),(8,3),(11,4),(14,11),(17,10),(19,16),
    (3,2),(8,5),(11,6),(15,2),(17,12),(20,1),
    (4,1),(8,7),(12,7),(15,4),(18,5),(20,3),
    (5,2),(9,4),(13,2),(15,14),(18,7),(20,7),
    (5,4),(10,1),(13,8),(16,1),(18,17),(20,11),
    (6,1),(10,3),(13,10),(16,5),(19,0),(20,13),
    (6,5),(10,7),(13,12),(16,9),(19,6),(20,19)
    };

    std::vector<std::complex<int>>::iterator a,b,c,d;    
    std::unordered_multimap<std::complex<int>,std::vector<std::complex<int>>, hash_X > umm_sums;

    for(a=gprimes.begin(); a != gprimes.end(); ++a) {
    	for(b=gprimes.begin(); b != gprimes.end(); ++b) {
    		if(b==a) continue;
    		for(c=gprimes.begin(); c != gprimes.end(); ++c) {
    			if((c==a)||(c==b)) continue;
    			for(d=gprimes.begin(); d != gprimes.end(); ++d) {
    				if((d==a)||(d==b)||(d==c)) continue;
    				std::complex<int> foo (*a+*b+*c+*d);
    				std::vector<std::complex<int>> bar { *a, *b, *c, *d };
    				std::pair<std::complex<int>, std::vector<std::complex<int>>> foobar;
    				foobar = std::make_pair(foo,bar);
    				umm_sums.insert(foobar);
    			}
    		}
    	}
    }

}
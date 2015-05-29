#include <iostream>
#include <complex>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

typedef std::complex<int> C;

struct hash_X{
  size_t operator()(const complex<int> &x) const{
	size_t hashcode = 23;	
	hashcode = (hashcode * 37) + x.real();
	hashcode = (hashcode * 37) + x.imag();
	//hash<int> f;
	//hashcode = f( x.real() ) ^ f( x.imag() );	
  return hashcode;
  };
};


void sieve(std::vector<int> *p, int n) {
	int limit = (int)sqrt((double)n);
	std::vector<bool> A(n,true);
	for(int i = 2; i < limit; ++i) {
		if(A[i]) {
			for(int j = i*i; j < n; j += i) A[j] = false;
		}
	}
	// scan the sieve and store primes in p
	p->clear();
	for(int i = 2; i != n; ++i)	if(A[i]) p->push_back(i);		
}

void primes3mod4(std::vector<int> *primes, std::vector<int> *p34) {
	p34->clear();
	for(auto a = primes->begin(); a != primes->end(); ++a)
		if( ((*a)%4)==3 ) p34->push_back(*a);
}

int vect_gaussprimes(std::vector< std::complex<int> > *gpv, int n) {
	
	// establish a list of primes up to 2*n*n
	std::vector<int> primes;	// vector of real primes	
	std::vector<int> p34;		// vector of real primes congruent to 3 mod 4

	
	sieve( &primes, (2*n*n) );	// establish vector of real primes
	primes3mod4(&primes, &p34);			// find subset of real primes congruent to 3 mod 4
	
	for(int a = 1; a <= n; ++a) {
		for(int b = 0; b <= a; ++b) {
			if(b != 0) {	//testcase 1 [a^2 + b^2 is prime]
				int gp = ((a*a)+(b*b));
				// if gp in primes a+bi is gaussian prime
				if(binary_search(primes.begin(), primes.end(), gp)) {
					// gp is gaussian
					std::complex<int> wspace (a,b);	// workspace for complex prime
					gpv->push_back(wspace);
					cout << a;
					if(b>0) cout << "+i";
					if(b>1) cout << b;
					cout << std::endl;
				}
			} else {		//testcase 3 [|a| is prime AND |a| mod 4 = 3]
				int gp = abs(a);
				if ( binary_search(p34.begin(), p34.end(), gp) ) {
					// gp is gaussian
					std::complex<int> wspace (a,b);	// workspace for complex prime
					gpv->push_back(wspace);
					cout << a;
					if(b>0) cout << "+i";
					if(b>1) cout << b;
					cout << std::endl;
				}
			}
		}			
	}
	return gpv->size();
}

using gPrime = std::complex<int>;
using cNode = std::vector<gPrime>;
using KeyValue = std::pair<gPrime,cNode>;
using NodeList = std::vector<cNode>;


int main() {
	
	const int Limit = 10;
	
	cNode gaussian;
	
	cNode::iterator rai;	//random access iterator
	
	vect_gaussprimes(&gaussian, Limit);    
    
    std::unordered_multimap<gPrime,cNode, hash_X > umm_sums;
    
    for(auto a=gaussian.begin(); a != gaussian.end(); ++a) {
    	for(auto b=gaussian.begin(); b != gaussian.end(); ++b) {
    		if(b==a) continue;
    		for(auto c=gaussian.begin(); c != gaussian.end(); ++c) {
    			if((c==a)||(c==b)) continue;
    			for(auto d=gaussian.begin(); d != gaussian.end(); ++d) {
    				if((d==a)||(d==b)||(d==c)) continue;
    				gPrime foo {(*a+*b+*c+*d)};
    				cNode bar { *a, *b, *c, *d };
    				KeyValue foobar = std::make_pair(foo,bar);
    				umm_sums.insert(foobar);
    			}
    		}
    	}
    }
    
	unsigned nbuckets = umm_sums.bucket_count();
	
	unsigned count = 0;
	
	for(unsigned i=0; i<nbuckets; ++i) {
		// the sum 26 + i12 decomposes to 29 groups of 24. 1176 elements
		if(umm_sums.bucket_size(i) >= (24*49)) {
			++count;
			for(auto b = umm_sums.cbegin(i); b != umm_sums.cend(i); ++b) {
				gPrime gp = b->first;
				cNode cn = b->second;
				cout << gp.real() << "+" << gp.imag() << "\t";
				for(auto c = cn.begin(); c != cn.end(); ++c) {
					cout << c->real() << "+i" << c->imag() << "\t";
				}
				cout << std::endl;	
			}			
			std::cout << "bucket #" << i << " has " << umm_sums.bucket_size(i) << " elements.\n";
			break;
		}
	}
	cout << count << " active buckets." << std::endl;
}

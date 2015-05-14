#include <iostream>
#include <complex>
#include <vector>
#include <array>
#include <unordered_map>

using namespace std;

int main() {

    std::complex<int> gp1 (7,3);

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

}

    

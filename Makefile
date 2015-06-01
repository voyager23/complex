# Makefile for complex

dev1:	dev1.cpp
	g++ -std=gnu++11 -g -Wall dev1.cpp -o dev1

# --------------------------------------------

cpg:	cpg.cpp
	g++ -std=gnu++11 -g cpg.cpp -o cpg

sums:	sums.cpp
	g++ -std=gnu++11 -g -Wall sums.cpp -o sums
	

# Makefile for complex

# --------------------------------------------

cpg:	cpg.cpp
	g++ -std=gnu++11 -g cpg.cpp -o cpg

sums:	sums.cpp
	g++ -std=gnu++11 -g -Wall sums.cpp -o sums
	

# Makefile for complex/devel

TARGET = analyse

$(TARGET):	$(TARGET).cpp fasthash.o
	g++ -std=gnu++11 -g -Wall  $(TARGET).cpp fasthash.o -o $(TARGET)
	
cts:	cts.cpp fasthash.o
	g++ -std=gnu++11 -g -Wall -fmax-errors=1 cts.cpp fasthash.o -o cts	

fashhash.o:	fasthash.c	fasthash.h
	g++ -std=gnu++11 -g -Wall  -c fasthash.c
	
sums2:	sums2.cpp
	g++ -std=gnu++11 -g -Wall sums2.cpp -o sums2



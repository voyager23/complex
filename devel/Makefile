# Makefile for complex/devel

TARGET = cts

$(TARGET):	$(TARGET).cpp fasthash.c
	g++ -std=gnu++11 -g -Wall  $(TARGET).cpp fasthash.c -o $(TARGET)
	
fashhash.o:	fasthash.c	fasthash.h
	g++ -std=gnu++11 -g -Wall  -c fasthash.c
	
sums2:	sums2.cpp
	g++ -std=gnu++11 -g -Wall sums2.cpp -o sums2


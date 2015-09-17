# Makefile for complex/devel

TARGET = dev4

$(TARGET):	$(TARGET).cpp fasthash.c
	g++ -std=gnu++11 -g -Wall  $(TARGET).cpp fasthash.c -o $(TARGET)
	
fashhash.o:	fasthash.c	fasthash.h
	g++ -std=gnu++11 -g -Wall  -c fasthash.c

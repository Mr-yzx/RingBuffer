CXX ?= g++

MyApp: main.cpp RingBuffer.h
	$(CXX) -std=c++11  $^ -lpthread  -o MyApp   

clean:
	rm  -r MyApp


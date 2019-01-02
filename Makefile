CXX = g++
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
CXXFLAGS = -std=c++11

p1: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o p1

*.o: src/*.cpp src/*.h
	$(CXX) $(CXXFLAGS) -c src/*.cpp

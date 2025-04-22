
CXX = g++
CXXFLAGS = -Wall -std=c++11

all: main

main: db_driver.cpp AVL_Database.cpp
	$(CXX) $(CXXFLAGS) -o main db_driver.cpp AVL_Database.cpp

clean:
	rm -f main *.o

debug: CXXFLAGS += -g
debug: main

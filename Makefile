# A simple makefile for CSE 100 P3

CC=g++
CXXFLAGS=-std=c++11 -g -Wall -gdwarf-2
LDFLAGS=-g

all: pathfinder actorconnections

actorconnections: ActorGraph.o

pathfinder: ActorGraph.o

ActorGraph.o: ActorGraph.hpp




clean:
	rm -f pathfinder *.o core* *~


# /**
# * @Author:              aravind
# * @Email:               arvindsuresh2009@gmail.com
# * @Github:              [Aravind-Suresh](https://github.com/Aravind-Suresh)
# * @Date:                2017-03-08T12:35:40+05:30
# * @Last modified by:   	aravind
# * @Last modified time: 	2017-03-08T12:36:38+05:30
# */
CXX = g++ -I./include

CXXFLAGS = -ggdb

all: bin/main

bin/main: obj/util.o obj/main.o
	$(CXX) $^ -o $@

obj/main.o: src/main.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

obj/util.o: src/util.cpp
	mkdir -p obj bin
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -rf obj/*.o
	rm -rf bin/*

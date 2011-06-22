CXX=g++
CXXFLAGS=-O2 -lpthread -lm -Wno-write-strings

main:
	${CXX} ${CXXFLAGS} -o bot *.cpp IRC.cc

clean:
	rm *.o bot

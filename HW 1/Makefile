CXXFLAGS = -Wall -Werror -pedantic
DEBUGFLAGS = -g -DDEBUG
STANDARD = -std=c++14

CLEANFILES = p1.exe p2.exe p3.exe p3_debug.exe
CC = g++

p1: p1.cpp
	${CC} ${CXXFLAGS} ${STANDARD} p1.cpp -o p1.exe

p2: p2.cpp
	${CC} ${CXXFLAGS} ${STANDARD} p2.cpp -o p2.exe

p3: p3.cpp
	${CC} ${CXXFLAGS} ${STANDARD} p3.cpp -o p3.exe

p3_debug: p3.cpp
	${CC} ${CXXFLAGS} ${STANDARD} ${DEBUGFLAGS} p3.cpp -o p3_debug.exe

p4: p4.cpp 
	${CC} ${CXXFLAGS} ${STANDARD} p4.cpp -o p4.exe

memory: memory.cpp
	${CC} ${CXXFLAGS} ${STANDARD} ${DEBUGFLAGS} memory.cpp -o memory.exe

array: two_dimensional_array.cpp
	${CC} ${STANDARD} ${DEBUGFLAGS} two_dimensional_array.cpp -o array.exe

.phone: clean

clean:
	del ${CLEANFILES}
SRC:=$(wildcard . *.cpp)

all: libmll.so

check:
	./check.m

libmll.so: ${SRC} ${SRC:%.cpp=%.h}
	./crtlib.m

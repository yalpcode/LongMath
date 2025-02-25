PRECISION ?= 100

default_target:
	cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ . -B build && cd build && make

main:
	cd build && ./main

pi:
	cd build && ./pitest --precision=$(PRECISION)

test:
	cd build && ./mathtest
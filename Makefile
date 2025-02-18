PRECISION ?= 100

default_target:
	cmake . -B build && cd build && make

main:
	cd build && ./main

pi:
	cd build && ./pitest --precision=$(PRECISION)

test:
	cd build && ./mathtest
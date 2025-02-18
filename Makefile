default_target:
	cmake . -B build && cd build && make && ./main

start:
	cd build && ./main

pi:
	cmake . -B build && cd build && make && ./pitest

test:
	cmake . -B build && cd build && make && ./mathtest
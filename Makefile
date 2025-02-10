default_target:
	cmake . -B build && cd build && make && ./main

start:
	cd build && ./main
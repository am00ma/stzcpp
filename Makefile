.PHONY: init all install test clean

init:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..

all:
	cd build && make -j6

install:
	rm -f -r build/
	mkdir -p build
	cd build && \
		cmake -DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_INSTALL_PREFIX=/usr/local \
		..
	cd build && make -j6

test:
	./build/tests/test_slice

clean:
	rm -f -r build/

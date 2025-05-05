PACKAGE  = stu
VERSION  = 0.0.0

lib_libs   =  -ljack -lsndfile -lSDL2 -lSDL2_image
prefix     ?= build
extra_dist =  Makefile README.md

CXX      ?= clang++
CXXFLAGS += -Iinclude -DVERSION=\"$(VERSION)\"
CXXFLAGS += -std=c++23
CXXFLAGS += -Wall -pedantic -Wno-address-of-packed-member
LDFLAGS  +=

headers    = $(wildcard include/*.h)
lib_hdr    = $(wildcard src/*.h)
lib_src    = $(wildcard src/*.cpp)

test_libs  = $(lib_libs)
test_src   = $(wildcard test/*.cpp)

example_libs = $(lib_libs)
example_src  = $(wildcard example/*.cpp)

dist_files = $(headers) $(lib_hdr) $(lib_src) $(test_src) $(extra_dist)

LIBRARY  = build/$(PACKAGE).so
OBJS     = $(patsubst src/%.cpp, build/obj/%.o, $(lib_src))
TESTS    = $(patsubst test/%.cpp, build/test/%, $(test_src))
EXAMPLES = $(patsubst example/%.cpp, build/example/%, $(example_src))

.PHONY: all test init vars clean install uninstall dist

all: $(LIBRARY) test example

test: $(TESTS)

example: $(EXAMPLES)

init:
	mkdir -p include
	mkdir -p src
	mkdir -p test
	mkdir -p example
	mkdir -p build/obj
	mkdir -p build/test
	mkdir -p build/example

vars:
	@echo "LIBRARY  : " $(LIBRARY)
	@echo "OBJS     : " $(OBJS)
	@echo "TESTS    : " $(TESTS)
	@echo "EXAMPLES : " $(EXAMPLES)
	@echo ""
	@echo "lib_libs : " $(lib_libs)
	@echo "headers  : " $(headers)
	@echo "lib_src  : " $(lib_src)
	@echo "lib_hdr  : " $(lib_hdr)
	@echo ""
	@echo "test_libs : " $(test_libs)
	@echo "test_src  : " $(test_src)
	@echo ""
	@echo "example_libs : " $(example_libs)
	@echo "example_src  : " $(example_src)

clean:
	rm -f build/obj/*.o build/obj/*.d $(LIBRARY) build/lib$(PACKAGE).a $(TESTS) $(EXAMPLES)

install: $(LIBRARY)
	mkdir -p $(prefix)/include/$(PACKAGE) && cp $(headers) $(prefix)/include/$(PACKAGE)
	mkdir -p $(prefix)/lib && cp build/lib$(PACKAGE).a $(prefix)/lib

uninstall:
	rm -r $(prefix)/include/$(PACKAGE)
	rm $(prefix)/lib/lib$(PACKAGE).a

dist:
	mkdir $(PACKAGE)-$(VERSION)
	cp --parents $(dist_files) $(PACKAGE)-$(VERSION)
	tar -czvf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION)
	rm -rf $(PACKAGE)-$(VERSION)

$(LIBRARY): $(OBJS)
	$(CXX) -shared -fPIC $(CXXFLAGS) $(LDFLAGS) $^ $(lib_libs) -Wl,--out-implib=$(patsubst build/%.so,build/lib%.a, $@) -o $@

build/obj/%.o : src/%.cpp include/%.h
	$(CXX) -fPIC $(CXXFLAGS) -MD -c $< -o $(patsubst src/%.cpp, build/obj/%.o, $<)	

build/test/% : test/%.cpp $(LIBRARY)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(test_libs) -o $@

build/example/% : example/%.cpp $(LIBRARY)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(example_libs) -o $@

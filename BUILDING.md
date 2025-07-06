# Building

- [Creating a C++ library with CMake](https://decovar.dev/blog/2021/03/08/cmake-cpp-library/)

Here are some of the things you need to take care of:

- what artifacts should the library produce at install step
- where install artifacts should be placed
- how other applications can find the library
  - when they are using it pre-built as an external dependency
  - when its sources are nested in their source tree
- will it be static or shared library
  - will you need to have it as DLL on Windows

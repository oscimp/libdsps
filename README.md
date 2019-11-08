Digital Signal Processing Simulation C++ library (libdsps)
==========================================================

**Digital Signal Processing Simulation (dsps)** is a library to build your own digital signal processing simulation.

Installation
------------

You have to install the following dependencies:

- [FFTW3](http://www.fftw.org/)
- [CMake](https://cmake.org/) >= 3.1
- [Boost](http://www.boost.org/)
- [libdsac](https://github.com/oscimp/libdsac)

For Debian/Ubuntu:

```sh
apt-get install libfftw3-dev libboost-all-dev cmake
```

Then:

```sh
git clone https://github.com/oscimp/libdsps.git
cd dsps_lib
mkdir build
cd build
cmake ..
make
make install # may require root permissions
```

Available Options
-----------------

- DSPS_DEBUG: Enable the debug build type
- DSPS_TESTS: Enable the test suite
- DSPS_EXAMPLES: Compile some examples using libdsps

Options are used with the -D flag, for example
```sh
cmake -DDSPS_DEBUG=ON|OFF ..
```

Run tests
=========
Running the unit tests is achieved by executing:
```sh
git submodule update --init
mkdir build
cd build
cmake [YOUR-OPTIONS] -DDSPS_TESTS=ON ..
make
make run_tests
```

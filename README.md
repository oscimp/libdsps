Digital Signal Processing Simulation C++ library (libdsps)
==========================================================

**Digital Signal Processing Simulation (dsps)** is a library to build your own digital signal processing simulation.

Installation
------------

You have to install the following dependencies:

- [FFTW3](http://www.fftw.org/)
- [CMake](https://cmake.org/) >= 3.1
- [Boost](http://www.boost.org/)
- [libdsac](https://lxsd.femto-st.fr/gitlab/ahugeat/libdsac)

For Debian/Ubuntu:

```sh
apt-get install libfftw3-dev libboost-all-dev cmake
```

Then:

```sh
git clone https://lxsd.femto-st.fr/gitlab/ahugeat/libdsps.git
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
- DSPS_TESTS: Enable the tests suite
- DSPS_EXAMPLES: Compile some examples using libdsps

To use a option use this command by example
```sh
cmake -DDSPS_DEBUG=ON|OFF ..
```

Run tests
=========
For un the unit tests, you must execute:
```sh
git submodule update --init
mkdir build
cd build
cmake [YOUR-OPTIONS] -DDSPS_TESTS=ON ..
make
make run_tests
```
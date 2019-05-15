# Include header
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

# Depends
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../modules")
find_package(FFTW3 REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system iostreams)

# Create library dynamic
add_library(dsps SHARED
  Abs.cc
  ADC.cc
  Atan2.cc
  Channel.cc
  ConvertType.cc
  CrossSpectrum.cc
  Decimation.cc
  Demodulation.cc
  Detrend.cc
  Fft.cc
  FileSource.cc
  Fir.cc
  Gain.cc
  Hanning.cc
  Mixer.cc
  Mean.cc
  Nco.cc
  NoiseGenerator.cc
  NormalizePsddBc.cc
  Random.cc
  Shifter.cc
  SignalFromFile.cc
  SignalGenerator.cc
  Sum.cc
  Task.cc
  Unwrap.cc
  Utils.cc
  WrapperFFTW.cc
)

# Link libraries
target_link_libraries(dsps
  PUBLIC ${CMAKE_THREAD_LIBS_INIT}
  PUBLIC ${Boost_LIBRARIES}
  PRIVATE ${FFTW_LIBRARIES}
  PRIVATE dsac
)

set_target_properties(dsps
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

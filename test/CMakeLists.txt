# Include header
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../modules")
find_package(FFTW3 REQUIRED)

# Generate oracle path dir
set(ORACLE_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/oracle")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/local/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/local/config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/local)

# Generate test libraries
add_library(gtest SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest/googletest/src/gtest-all.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest/googlemock/src/gmock-all.cc
)
target_link_libraries(gtest
    ${FFTW_LIBRARIES}
)
target_include_directories(gtest
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest/googletest/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest/googletest
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest/googlemock/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest/googlemock
)

# Custom macro to add a test
macro(add_unit_test TARGET_NAME TARGET_FILE)
    # Get the executable name
    get_filename_component(TARGET_EXEC ${TARGET_FILE} NAME_WE)

    # Update the test list
    set(UNIT_TEST_TARGETS ${UNIT_TEST_TARGETS} ${TARGET_EXEC})

    # Create the executable
    add_executable(${TARGET_EXEC} ${TARGET_FILE})
    target_include_directories(${TARGET_EXEC}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/googletest/googlemock/include
    )
    target_link_libraries(${TARGET_EXEC} dsps gtest ${FFTW_LIBRARIES})

    # Add the test to ctest
    add_test(NAME ${TARGET_NAME} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_EXEC})
endmacro(add_unit_test)

# Core tests
add_unit_test("Test-channel" ${CMAKE_CURRENT_SOURCE_DIR}/ChannelTest.cc)
add_unit_test("Test-queue" ${CMAKE_CURRENT_SOURCE_DIR}/QueueTest.cc)
add_unit_test("Test-utlis" ${CMAKE_CURRENT_SOURCE_DIR}/UtilsTest.cc)
add_unit_test("Test-task" ${CMAKE_CURRENT_SOURCE_DIR}/TaskTest.cc)

# Task tests
add_unit_test("Test-abs" ${CMAKE_CURRENT_SOURCE_DIR}/AbsTest.cc)
add_unit_test("Test-adc" ${CMAKE_CURRENT_SOURCE_DIR}/ADCTest.cc)
add_unit_test("Test-atan2" ${CMAKE_CURRENT_SOURCE_DIR}/Atan2Test.cc)
add_unit_test("Test-convert-type" ${CMAKE_CURRENT_SOURCE_DIR}/ConvertTypeTest.cc)
add_unit_test("Test-cross-spectrum" ${CMAKE_CURRENT_SOURCE_DIR}/CrossSpectrumTest.cc)
add_unit_test("Test-decimation" ${CMAKE_CURRENT_SOURCE_DIR}/DecimationTest.cc)
add_unit_test("Test-demodulation" ${CMAKE_CURRENT_SOURCE_DIR}/DemodulationTest.cc)
add_unit_test("Test-detrend" ${CMAKE_CURRENT_SOURCE_DIR}/DetrendTest.cc)
add_unit_test("Test-fft" ${CMAKE_CURRENT_SOURCE_DIR}/FftTest.cc)
add_unit_test("Test-file-sink" ${CMAKE_CURRENT_SOURCE_DIR}/FileSinkTest.cc)
add_unit_test("Test-file-source" ${CMAKE_CURRENT_SOURCE_DIR}/FileSourceTest.cc)
add_unit_test("Test-fir" ${CMAKE_CURRENT_SOURCE_DIR}/FirTest.cc)
add_unit_test("Test-gain" ${CMAKE_CURRENT_SOURCE_DIR}/GainTest.cc)
add_unit_test("Test-hanning" ${CMAKE_CURRENT_SOURCE_DIR}/HanningTest.cc)
add_unit_test("Test-mean" ${CMAKE_CURRENT_SOURCE_DIR}/MeanTest.cc)
add_unit_test("Test-mixer" ${CMAKE_CURRENT_SOURCE_DIR}/MixerTest.cc)
add_unit_test("Test-nco" ${CMAKE_CURRENT_SOURCE_DIR}/NcoTest.cc)
add_unit_test("Test-noise-generator" ${CMAKE_CURRENT_SOURCE_DIR}/NoiseGeneratorTest.cc)
add_unit_test("Test-normalize-dBc" ${CMAKE_CURRENT_SOURCE_DIR}/NormalizePsddBcTest.cc)
add_unit_test("Test-shifter" ${CMAKE_CURRENT_SOURCE_DIR}/ShifterTest.cc)
add_unit_test("Test-signal-generator" ${CMAKE_CURRENT_SOURCE_DIR}/SignalGeneratorTest.cc)
add_unit_test("Test-splitter" ${CMAKE_CURRENT_SOURCE_DIR}/SplitterTest.cc)
add_unit_test("Test-sum" ${CMAKE_CURRENT_SOURCE_DIR}/SumTest.cc)
add_unit_test("Test-unwrap" ${CMAKE_CURRENT_SOURCE_DIR}/UnwrapTest.cc)

# Integrate test
add_unit_test("Test-DSP" ${CMAKE_CURRENT_SOURCE_DIR}/DSPTest.cc)

add_custom_target(run_tests
    DEPENDS ${UNIT_TEST_TARGETS}
)
add_custom_command(TARGET run_tests
   COMMENT "Run tests"
   COMMAND ctest ARGS --output-on-failure
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

#ifndef TESTING_UTILS_H
#define TESTING_UTILS_H

#include <complex>
#include <fstream>
#include <vector>
#include <random>

#include <gtest/gtest.h>

#include "config.h"

void expect_eq_double(double expected, double actual, double precision = 1e-9) {
    EXPECT_LE(std::abs(expected - actual), precision);
}

void compareChannelWithVector(std::vector<double> &expected, Channel &actual, double precision = 1e-9) {
    // The size must be match
    ASSERT_EQ(expected.size(), actual.size(sizeof(double)));

    std::vector<double> values(expected.size());
    actual.receive(values, expected.size());

    // Compare the values
    for (std::size_t i = 0; i < expected.size(); ++i) {
        expect_eq_double(expected[i], values[i], precision);
    }
}

void compareChannelWithVector(std::vector< std::complex<double> > &expected, Channel &actual, double precision = 1e-9) {
    // The size must be match
    ASSERT_EQ(expected.size(), actual.size(sizeof(std::complex<double>)));

    std::vector< std::complex<double> > values(expected.size());
    actual.receive(values, expected.size());

    // Compare the values
    for (std::size_t i = 0; i < expected.size(); ++i) {
        expect_eq_double(expected[i].real(), values[i].real(), precision);
        expect_eq_double(expected[i].imag(), values[i].imag(), precision);
    }
}

template <typename T>
std::vector<T> extractVectorFromOracleFile(std::ifstream &file, const std::uint64_t N) {
    std::vector<T> vec;

    for (std::size_t i = 0; i < N && !file.eof(); ++i) {
        T val;
        file.read(reinterpret_cast<char*>(&val), sizeof(T));
        vec.push_back(val);
    }

    return vec;
}

template <typename T>
void loadChannelFromOracleFile(Channel &channel, std::ifstream &file, const std::uint64_t N) {
    std::vector<T> values;
    for (std::size_t i = 0; i < N && !file.eof(); ++i) {
        T val;
        file.read(reinterpret_cast<char*>(&val), sizeof(T));
        values.push_back(val);
    }

    channel.send(values);
}

std::mt19937 createRandomEngine() {
    std::time_t seed = std::time(nullptr);
    std::cout << "Random seed: " << seed << std::endl;

    return std::mt19937(seed);
}

template <typename T>
void computeUniformIntegerVector(std::mt19937 &engine, std::vector<T> &vector, T min, T max) {
    std::uniform_int_distribution<T> dist(min, max);

    for (T &value: vector) {
        value = dist(engine);
    }
}

template <typename T>
void computeUniformFloatVector(std::mt19937 &engine, std::vector<T> &vector, T min, T max) {
    std::uniform_real_distribution<T> dist(min, max);

    for (T &value: vector) {
        value = dist(engine);
    }
}

#endif // TESTING_UTILS_H

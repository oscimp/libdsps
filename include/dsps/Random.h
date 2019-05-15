#ifndef RANDOM_H
#define RANDOM_H

#include <complex>
#include <cstdint>
#include <random>

class Random {
public:
    Random();
    Random(std::uint64_t seed);

    void computeNormalPlage(const double mean, const double stddev, std::vector< std::complex<double> > &data);

private:
    std::mt19937 m_engine;
};

#endif // RANDOM_H

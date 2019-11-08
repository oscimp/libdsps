/* DSPS - library to build a digital signal processing simulation
 * Copyright (C) 2019  Arthur HUGEAT
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 */

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

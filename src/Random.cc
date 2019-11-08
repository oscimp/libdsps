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

#include <dsps/Random.h>

#include <algorithm>
#include <functional>

// see http://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
// and http://www.pcg-random.org/posts/cpp-seeding-surprises.html
static std::mt19937 getCorrectlyInitializedEngine() {
    std::mt19937::result_type data[std::mt19937::state_size];
    std::random_device source;
    std::generate(std::begin(data), std::end(data), std::ref(source));
    std::seed_seq seeds(std::begin(data), std::end(data));
    return std::mt19937(seeds);
}

Random::Random()
: m_engine(getCorrectlyInitializedEngine()) {
}

Random::Random(std::uint64_t seed)
: m_engine(seed) {
}

void Random::computeNormalPlage(const double mean, const double stddev, std::vector< std::complex<double> > &data) {
    std::normal_distribution<double> dist(mean, stddev);
    for (auto &d: data) {
        d.real(dist(m_engine));
        d.imag(0.0);
    }
}

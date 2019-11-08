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

#include <dsps/Nco.h>

#include <iostream>

#include <dsps/Channel.h>

#warning "The LUT version is not 100% equals with Octave"

Nco::Nco(double amplitude, const double signalFrequency, const double sampleFrequency)
: Task(ChannelType::Double, 1, ChannelType::Double, 2)
, m_amplitude(amplitude)
, m_signalFrequency(signalFrequency)
, m_sampleFrequency(sampleFrequency)
, m_LUT_SIZE(findCorrectLUTSize(sampleFrequency / signalFrequency))
, m_cosLUT(m_LUT_SIZE)
, m_sinLUT(m_LUT_SIZE)
, m_index(m_indexStep) {
    // Compute the LUT
    const double STEP = 2.0 * M_PI * m_signalFrequency * (1.0 / m_sampleFrequency) / m_indexStep;
    for (std::size_t i = 0; i < m_LUT_SIZE; ++i) {
        m_cosLUT[i] = std::cos(i * STEP);
        m_sinLUT[i] = std::sin(i * STEP);
    }
}

void Nco::compute(const std::uint64_t N) {
    std::vector<double> cosValues(N);
    std::vector<double> sinValues(N);

    // Generate a perfect signal
    for (std::size_t i = 0; i < N; ++i) {
        cosValues[i] = m_amplitude * m_cosLUT[m_index];
        sinValues[i] = m_amplitude * m_sinLUT[m_index];
        m_index = (m_index + m_indexStep) % m_LUT_SIZE;
    }

    m_outputChannels[0].send(cosValues);
    m_outputChannels[1].send(sinValues);

    /// 100% equal to Unit test
    // static double STEP = 2.0 * M_PI * m_signalFrequency * (1.0 / m_sampleFrequency);
    // for (std::uint64_t i = 0; i < N; ++i) {
    //     cosValues[i] = m_amplitude * std::cos(m_index * STEP);
    //     sinValues[i] = m_amplitude * std::sin(m_index * STEP);
    //     m_index = (m_index + 1);
    // }
}

bool Nco::isReady(const std::uint64_t N) const {
    USELESS_PARAMETER(N);
    return true;
}

bool Nco::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N && m_outputChannels[1].size(sizeof(double)) >= N;
}

std::size_t Nco::findCorrectLUTSize(const double POINTS_BY_PERIOD) {
    std::uint64_t factor = 1;

    double intPart = 0.0;
    while (std::modf(factor * POINTS_BY_PERIOD, &intPart) != 0) {
        ++factor;
    }

    // Total points
    std::uint64_t nb_points = static_cast<std::uint64_t>(std::round(factor * POINTS_BY_PERIOD));
    m_indexStep = factor;

    // If the lut ask too memory (more 2 Go with cos and sin lut)
    if ((nb_points * sizeof(double)) > 1e9) {
        std::cerr << "Nco::findCorrectLUTSize(): Too memory allocated!" << std::endl;
        std::exit(-1);
    }

    return nb_points;
}

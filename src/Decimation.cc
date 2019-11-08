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

#include <dsps/Decimation.h>

#include <dsps/Channel.h>

Decimation::Decimation(std::uint64_t decimationFactor)
: Task(ChannelType::Double, 1, ChannelType::Double, 1)
, m_decimationFactor(decimationFactor) {
}

void Decimation::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Decimation: No input task is connected");

    // Get the input data
    const std::uint64_t LIMIT = N * m_decimationFactor;
    std::vector<double> inValues(LIMIT);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(inValues, LIMIT);

    // Decim the flux
    for (std::size_t i = 0, j = 0; i < LIMIT; i += m_decimationFactor, ++j) {
        outValues[j] = inValues[i];
    }

    m_outputChannels[0].send(outValues);
}

bool Decimation::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Decimation: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N * m_decimationFactor;
}

bool Decimation::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

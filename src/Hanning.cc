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

#include <dsps/Hanning.h>

#include <vector>

#include <dsac/t_pnm.h>

#include <dsps/Channel.h>

Hanning::Hanning()
: Task(ChannelType::Double, 1, ChannelType::Double, 1) {
}

void Hanning::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Hanning: No input task is connected");

    std::vector<double> inValues(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(inValues, N);

    // Compute the hanning window
    hanning_window(inValues.data(), outValues.data(), N);

    m_outputChannels[0].send(outValues);
}

bool Hanning::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Hanning: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N;
}

bool Hanning::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

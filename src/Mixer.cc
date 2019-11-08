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

#include <dsps/Mixer.h>

#include <dsps/Channel.h>

Mixer::Mixer()
: Task(ChannelType::Double, 3, ChannelType::Double, 2) {
}

void Mixer::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr && m_inputChannels[2] != nullptr) && "Mixer: No input task is connected");

    // Get the input data
    std::vector<double> signalValues(N);
    std::vector<double> ncoCosValues(N);
    std::vector<double> ncoSinValues(N);
    std::vector<double> iValues(N);
    std::vector<double> qValues(N);

    m_inputChannels[0]->receive(signalValues, N);
    m_inputChannels[1]->receive(ncoCosValues, N);
    m_inputChannels[2]->receive(ncoSinValues, N);

    // Compute
    for (std::uint64_t i = 0; i < N; ++i) {
        double value = signalValues[i];
        iValues[i] = value * ncoCosValues[i];
        qValues[i] = value * ncoSinValues[i];
    }

    // Send the results
    m_outputChannels[0].send(iValues);
    m_outputChannels[1].send(qValues);
}

bool Mixer::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr && m_inputChannels[2] != nullptr) && "Mixer: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N && m_inputChannels[1]->size(sizeof(double)) >= N && m_inputChannels[2]->size(sizeof(double)) >= N ;
}

bool Mixer::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N && m_outputChannels[1].size(sizeof(double)) >= N;
}

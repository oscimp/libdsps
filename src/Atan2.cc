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

#include <dsps/Atan2.h>

#include <cmath>

#include <dsps/Channel.h>

Atan2::Atan2()
: Task(ChannelType::Double, 2, ChannelType::Double, 1) {
}

void Atan2::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr) && "Atan2: No input task is connected");

    std::vector<double> in1Values(N);
    std::vector<double> in2Values(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(in1Values, N);
    m_inputChannels[1]->receive(in2Values, N);

    for (std::size_t i = 0; i < N; ++i) {
        outValues[i] = std::atan2(in1Values[i], in2Values[i]);
    }

    m_outputChannels[0].send(outValues);
}

bool Atan2::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr) && "Atan2: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N && m_inputChannels[1]->size(sizeof(double)) >= N;
}

bool Atan2::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

void Atan2::connectIChannel(Task &task, std::size_t index) {
    Task::connect(task, index, *this, 1);
}

void Atan2::connectQChannel(Task &task, std::size_t index) {
    Task::connect(task, index, *this, 0);
}

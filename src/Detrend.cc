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

#include <dsps/Detrend.h>

#include <dsac/reg_lin.h>

#include <dsps/Channel.h>


Detrend::Detrend()
: Task(ChannelType::Double, 1, ChannelType::Double, 1)
, m_currentWindowSize(0)
, m_fs(0.0)
, m_currentIndexTime(0) {
}

Detrend::Detrend(double currentSampleFrequency)
: Task(ChannelType::Double, 1, ChannelType::Double, 2)
, m_currentWindowSize(0)
, m_fs(currentSampleFrequency)
, m_currentIndexTime(0) {
}

void Detrend::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Detrend: No input task is connected");

    // Init the reglin if needed
    if (m_currentWindowSize != N) {
        m_currentWindowSize = N;
    }

    initLinReg();

    // Get the values
    std::vector<double> values(N);
    m_inputChannels[0]->receive(values, N);

    double a = 0.0;
    double b = 0.0;

    reg_lin(&a, &b, values.data(), m_xMxB.data(), N, m_xBarre, N, m_xSquare);

    // If a output compensation is avaible
    if (m_outputChannels.size() == 2) {
        const double STEP = 1 / m_fs;
        std::vector<double> valuesComp(N);

        for (std::size_t i = 0; i < N; ++i) {

            valuesComp[i] = a * STEP * m_currentIndexTime + b;
            ++ m_currentIndexTime;
        }

        m_outputChannels[1].send(valuesComp);
    }

    for (std::size_t i = 0; i < N; ++i) {
        values[i] = values[i] - (a * i + b);
    }

    m_outputChannels[0].send(values);
}

bool Detrend::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Detrend: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N;
}

bool Detrend::hasFinished(const std::uint64_t N) const {
    if (m_outputChannels.size() == 2) {
        return m_outputChannels[0].size(sizeof(double)) >= N && m_outputChannels[1].size(sizeof(double)) >= N;
    }

    return m_outputChannels[0].size(sizeof(double)) >= N;
}

void Detrend::initLinReg() {
    std::vector<double> x(m_currentWindowSize);
    m_xMxB.resize(m_currentWindowSize);

    // Initialize the reglin
    for (std::size_t i = 0; i < m_currentWindowSize; ++i) {
        x[i] = i;
    }

    compute_xBarre_xMxB_xSquare(x.data(), m_currentWindowSize, &m_xBarre, m_xMxB.data(), &m_xSquare);
}

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

#include <dsps/Gain.h>

#include <complex>

#include <dsps/Channel.h>

template<typename T>
Gain<T>::Gain(const double gain)
: Task(getChannelType<T>(), 1, getChannelType<T>(), 1)
, m_gain(gain) {
}

template<typename T>
void Gain<T>::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Gain: No input task is connected");

    std::vector<T> inValues(N);
    std::vector<T> outValues(N);

    m_inputChannels[0]->receive(inValues, N);

    for (std::size_t i = 0; i < N; ++i) {
        outValues[i] = inValues[i] * m_gain;
    }

    m_outputChannels[0].send(outValues);
}

template<typename T>
bool Gain<T>::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Gain: No input task is connected");

    return m_inputChannels[0]->size(sizeof(T)) >= N;
}

template<typename T>
bool Gain<T>::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(T)) >= N;
}

template class Gain<double>;
template class Gain<std::complex<double>>;

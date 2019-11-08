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

#include <dsps/Abs.h>

#include <cmath>
#include <complex>

#include <dsps/Channel.h>

template<typename InputType, typename OutputType>
Abs<InputType, OutputType>::Abs()
: Task(getChannelType<InputType>(), 1, getChannelType<OutputType>(), 1) {
}

template<typename InputType, typename OutputType>
void Abs<InputType, OutputType>::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Abs: No input task is connected");

    // Alias variable
    Channel &in = *m_inputChannels[0];
    Channel &out = m_outputChannels[0];

    std::vector<InputType> inValues(N);
    std::vector<OutputType> outValues(N);

    in.receive(inValues, N);

    for (std::size_t i = 0; i < N; ++i) {
        outValues[i] = std::abs(inValues[i]);
    }

    out.send(outValues);
}

template<typename InputType, typename OutputType>
bool Abs<InputType, OutputType>::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Abs: No input task is connected");

    return m_inputChannels[0]->size(sizeof(InputType)) >= N;
}

template<typename InputType, typename OutputType>
bool Abs<InputType, OutputType>::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(OutputType)) >= N;
}

template class Abs<double, double>;
template class Abs<float, float>;
template class Abs<std::complex<double>, double>;
template class Abs<std::complex<float>, float>;

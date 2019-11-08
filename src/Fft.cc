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

#include <dsps/Fft.h>

#include <complex>
#include <mutex>

#include <dsps/Channel.h>
#include <dsps/Utils.h>

template<>
Fft<double>::Fft()
: Task(ChannelType::Double, 1, ChannelType::ComplexDouble, 1)
, m_wrapperFFTW(FFTDirection::Forward) {
}

template<>
Fft<float>::Fft()
: Task(ChannelType::Float, 1, ChannelType::ComplexFloat, 1)
, m_wrapperFFTW(FFTDirection::Forward) {
}

template<typename InputType>
void Fft<InputType>::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "FFT: No input task is connected");

    // Call the right method
    computeFFT(N);
}

template<typename InputType>
void Fft<InputType>::computeFFT(const std::uint64_t N) {
    std::vector<InputType> inValues(N);
    std::vector< std::complex<InputType> > outValues(N);

    // Get the input values
    m_inputChannels[0]->receive(inValues, N);

    // Compute fft
    m_wrapperFFTW.compute(inValues, outValues, N);

    // Return the result
    for (std::size_t i = 0; i < N; ++i) {
        if (i < N/2) {
            // Send the result
            outValues[i] = outValues[i];
        }
        else {
            // 0 padding
            outValues[i] = std::complex<InputType>(0.0, 0.0);
        }
    }
    m_outputChannels[0].send(outValues);
}

template<typename InputType>
bool Fft<InputType>::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "FFT: No input task is connected");

    return m_inputChannels[0]->size(sizeof(InputType)) >= N;
}

template<typename InputType>
bool Fft<InputType>::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(std::complex<InputType>)) >= N / 2;
}

template class Fft<double>;
template class Fft<float>;

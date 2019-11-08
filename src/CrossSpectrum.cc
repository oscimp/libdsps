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

#include <dsps/CrossSpectrum.h>

#include <complex>

#include <dsps/Channel.h>

CrossSpectrum::CrossSpectrum()
: Task(ChannelType::ComplexDouble, 2, ChannelType::Double, 1) {
}

void CrossSpectrum::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr) && "CrossSpectrum: No input task is connected");

    std::vector< std::complex<double> > in1Values(N);
    std::vector< std::complex<double> > in2Values(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(in1Values, N);
    m_inputChannels[1]->receive(in2Values, N);

    // Compute the data
    for (std::size_t i = 0; i < N; ++i) {
        std::complex<double> tmp1 = in1Values[i];
        std::complex<double> tmp2 = in2Values[i];

        double a = tmp1.real();
        double b = tmp1.imag();
        double c = tmp2.real();
        double d = tmp2.imag();

        if (i < N/2) {
            // Compute the data
            outValues[i] = a * c + b * d;
        }
        else {
            // 0 padding
            outValues[i] = 0.0;
        }
    }

    m_outputChannels[0].send(outValues);
}

bool CrossSpectrum::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr) && "CrossSpectrum: No input task is connected");

    return m_inputChannels[0]->size(sizeof(std::complex<double>)) >= N && m_inputChannels[1]->size(sizeof(std::complex<double>)) >= N;
}

bool CrossSpectrum::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

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

#include <dsps/NormalizePsddBc.h>

#include <complex>
#include <iostream>
#include <vector>

#include <dsac/t_pnm.h>

#include <dsps/Channel.h>

NormalizePsddBc::NormalizePsddBc(const double fs)
: Task(ChannelType::Double, 1, ChannelType::Double, 1)
, m_FS(fs) {
}

void NormalizePsddBc::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "NormalizePsddBc: No input task is connected");

    std::vector<double> inValues(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(inValues, N);

    // Normalize
    normalize_psd_dBc(inValues.data(), N, m_FS, outValues.data());

    m_outputChannels[0].send(outValues);
}

bool NormalizePsddBc::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "NormalizePsddBc: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N;
}

bool NormalizePsddBc::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

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

#include <dsps/Mean.h>

#include <complex>
#include <typeinfo>

#include <dsps/Channel.h>

template<typename T>
Mean<T>::Mean(const std::uint64_t LIMIT_ORDER)
: Task(getChannelType<T>(), 1, getChannelType<T>(), 1)
, m_order(0)
, m_LIMIT_ORDER(LIMIT_ORDER) {
}

template<typename T>
void Mean<T>::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Mean: No input task is connected");

    // Init the accum
    if (m_accum.size() != N) {
        initAccum(N);
    }

    // Update the order
    ++m_order;

    std::vector<T> inValues(N);
    std::vector<T> outValues(N);

    m_inputChannels[0]->receive(inValues, N);

    // Accum new input
    for (std::size_t i = 0; i < N; ++i) {
        T y = inValues[i] - m_kahanCompensation[i];
        T t = m_accum[i] + y;
        m_kahanCompensation[i] = (t - m_accum[i]) - y;
        m_accum[i] = t;
        outValues[i] = m_accum[i] / static_cast<double>(m_order);
    }

    // Discard N previous values
    if (m_outputChannels[0].size(sizeof(T)) >= N) {
        std::vector<T> tmpValues(N);
        m_outputChannels[0].receive(tmpValues, N);
    }

    // Return result
    if (m_order >= m_LIMIT_ORDER) {
        m_outputChannels[0].send(outValues);
    }
}

template<typename T>
bool Mean<T>::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Mean: No input task is connected");

    return m_inputChannels[0]->size(sizeof(T)) >= N;
}

template<typename T>
bool Mean<T>::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(T)) >= N && m_order >= m_LIMIT_ORDER;
}

template<typename T>
std::uint64_t Mean<T>::getNumberOfMean() const {
    return m_order;
}

template<typename T>
void Mean<T>::clearAccum() {
    m_accum.clear();
    m_kahanCompensation.clear();
    m_order = 0;
}

template<typename T>
void Mean<T>::initAccum(const std::uint64_t N) {
    std::string error = std::string(typeid(T).name()) + " wasn't a supported type.";
    assert(false && error.data());
}

template<>
void Mean<double>::initAccum(const std::uint64_t N) {
    m_accum.clear();
    m_kahanCompensation.clear();
    for (std::size_t i = 0; i < N; ++i) {
        m_accum.push_back(0.0);
        m_kahanCompensation.push_back(0.0);
    }
}

template<>
void Mean<float>::initAccum(const std::uint64_t N) {
    m_accum.clear();
    m_kahanCompensation.clear();
    for (std::size_t i = 0; i < N; ++i) {
        m_accum.push_back(0.0f);
        m_kahanCompensation.push_back(0.0f);
    }
}

template<>
void Mean<std::complex<double>>::initAccum(const std::uint64_t N) {
    m_accum.clear();
    m_kahanCompensation.clear();
    for (std::size_t i = 0; i < N; ++i) {
        m_accum.push_back(std::complex<double>(0.0, 0.0));
        m_kahanCompensation.push_back(std::complex<double>(0.0, 0.0));
    }
}

template class Mean<double>;
template class Mean<float>;
template class Mean<std::complex<double>>;

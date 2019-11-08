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

#ifndef SPLITTER_H
#define SPLITTER_H

#include <vector>

#include "Task.h"

template<typename T>
class Splitter : public Task {
public:
    /// Constructor
    Splitter(const std::size_t numberInput)
    : Task(getChannelType<T>(), 1, getChannelType<T>(), numberInput) {
    }

    /// \brief Split the input to many output (real or complex)
    /// This is an override of Task::compute.
    ///
    /// \param N The window size
    virtual void compute(const std::uint64_t N) override {
        // Check if the input task is connected
        assert(m_inputChannels[0] != nullptr && "Splitter: No input task is connected");

        std::vector<T> inValues(N);
        m_inputChannels[0]->receive(inValues, N);

        // Init the output channel
        std::vector< std::vector<T> > outValues;
        for (std::size_t i = 0; i < m_outputChannels.size(); ++i) {
            outValues.push_back(std::vector<T>(N));
        }

        // Add the value to next output
        for (std::size_t i = 0; i < N; ++i) {
            T tmp = inValues[i];

            for (std::size_t j = 0; j < m_outputChannels.size(); ++j) {
                outValues[j][i] = tmp;
            }
        }

        // Send all data
        for (std::size_t i = 0; i < m_outputChannels.size(); ++i) {
            outValues.push_back(std::vector<T>(N));
            m_outputChannels[i].send(outValues[i]);
        }
    }

    /// \brief Indicate if the task was ready for the compute
    /// This is an override of Task::compute.
    ///
    /// \param N The window size
    /// \return True if the task was ready else false
    virtual bool isReady(const std::uint64_t N) const override {
        // Check if the input task is connected
        assert(m_inputChannels[0] != nullptr && "Splitter: No input task is connected");

        return m_inputChannels[0]->size(sizeof(T)) >= N;
    }

    /// \brief Indicate if the task was finished the compute
    /// This is an override of Task::hasFinished.
    ///
    /// \param N The window size
    /// \return True if the task was finished else false
    virtual bool hasFinished(const std::uint64_t N) const override {
        bool finished = true;
        for (std::size_t i = 0; i < m_outputChannels.size() && finished; ++i) {
            // Channel &out = ;
            if (m_outputChannels[i].size(sizeof(T)) < N) {
                finished = false;
            }
        }
        return finished;
    }
};

#endif // SPLITTER_H

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

#ifndef _FILE_SINK_H
#define _FILE_SINK_H

#include "Channel.h"
#include "Task.h"
#include "Utils.h"

#include <iostream>
#include <fstream>

template <typename T>
class FileSink: public Task {
public:
    /// Constructor
    ///
    /// \param filename Path od data file
    /// \param override Indicate if we keep only one windows points or if we accumulate the data
    FileSink(const std::string filename, bool override = false)
        : Task(getChannelType<T>(), 1, ChannelType::None, 0)
        , m_filename(filename)
        , m_finished(false)
        , m_mustOverride(override) {
    }

    /// \brief Write the data into binary file
    /// This is an override of Task::compute.
    ///
    /// \param N The window size
    virtual void compute(const std::uint64_t N) override {
        // Check if the input task is connected
        assert(m_inputChannels[0] != nullptr && "FileSink: No input task is connected");

        // Open the file
        safeOpen();

        // Get the new burst
        std::vector<T> inValues(N);
        m_inputChannels[0]->receive(inValues, N);

        // Write file
        m_outFile.write(reinterpret_cast<char*>(inValues.data()), N * sizeof(T));

        m_finished = true;
    }

    /// \brief Indicate if the task was ready for the compute
    /// This is an override of Task::compute.
    ///
    /// \param N The window size
    /// \return True if the task was ready else false
    virtual bool isReady(const std::uint64_t N) const override {
        // Check if the input task is connected
        assert(m_inputChannels[0] != nullptr && "FileSink: No input task is connected");

        return m_inputChannels[0]->size(sizeof(T)) >= N;
    }

    /// \brief Indicate if the task was finished the compute
    /// This is an override of Task::hasFinished.
    ///
    /// \param N The window size
    /// \return True if the task was finished else false
    virtual bool hasFinished(const std::uint64_t N) const override {
        USELESS_PARAMETER(N);
        return m_finished;
    }

private:
    void safeOpen() {
        // If the file was already open and we don't override the data
        if (!m_mustOverride && m_outFile.is_open()) {
            return;
        }

        // If we override the data, we close the file
        if (m_mustOverride && m_outFile.is_open()) {
            m_outFile.close();
        }

        m_outFile.open(m_filename, std::ios_base::out|std::ios_base::binary);
        if (!m_outFile.good()) {
            std::cerr << "FileSink::safeOpen(): The file '" << m_filename << "' wasn't open: " << std::strerror(errno) << std::endl;
            std::exit(-1);
        }
    }

private:
    std::ofstream m_outFile;
    std::string m_filename;
    bool m_finished;
    bool m_mustOverride;
};

#endif // _FILE_SINK_H

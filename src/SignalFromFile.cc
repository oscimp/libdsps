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

#include <dsps/SignalFromFile.h>

#include <iostream>

#include <dsps/Channel.h>
#include <dsps/Utils.h>

SignalFromFile::SignalFromFile(const std::string &path)
: Task(ChannelType::None, 0, ChannelType::Double, 1)
, m_path(path)
, m_reader(ReaderType::RAM)
, m_repeat(false)
, m_currentIndex(0)
, m_overlap(0) {
    // Check if the file exists
    checkFile();
    // Load the data
    loadToRam();
}

SignalFromFile::SignalFromFile(const std::string &path, bool repeat)
: Task(ChannelType::None, 0, ChannelType::Double, 1)
, m_path(path)
, m_reader(ReaderType::STREAM)
, m_repeat(repeat)
, m_currentIndex(0)
, m_overlap(0) {
    // Check if the file exists
    checkFile();
}

SignalFromFile::SignalFromFile(const std::string &path, bool repeat, std::uint64_t overlap)
: Task(ChannelType::None, 0, ChannelType::Double, 1)
, m_path(path)
, m_reader(ReaderType::OVERLAPS)
, m_repeat(repeat)
, m_currentIndex(0)
, m_overlap(overlap) {
    // Check if the file exists
    checkFile();
}

void SignalFromFile::compute(const std::uint64_t N) {
    // Read the data
    switch (m_reader) {
    case ReaderType::RAM:
    {
        std::vector<double> outValues(N);

        // Send the data
        for (std::size_t i = 0; i < N; ++i) {
            outValues[i] = m_buffer[(m_currentIndex + i) % m_buffer.size()];
        }

        m_outputChannels[0].send(outValues);

        // Update the current index
        m_currentIndex = (m_currentIndex + N) % m_buffer.size();
        break;
    }
    case ReaderType::STREAM:
        m_buffer.clear();
        readBuffer(N);
        sendBuffer(N);
        break;
    case ReaderType::OVERLAPS:
        readBuffer(N);
        sendBuffer(N);

        // Shift the buffer
        for (std::size_t i = 0; i < m_overlap; ++i) {
            m_buffer.pop_front();
        }
        break;
    }
}

bool SignalFromFile::isReady(const std::uint64_t N) const {
    USELESS_PARAMETER(N);

    return true;
}

bool SignalFromFile::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

void SignalFromFile::readBuffer(const std::uint64_t N) {
    double d = 0.0;
    for (std::size_t i = m_buffer.size(); i < N; ++i) {
        m_file >> d;
        if (m_file.eof()) {
            if (m_repeat) {
                m_file.clear();
                m_file.seekg(0);
                m_file >> d;
            }
            else {
                std::cerr << "Data input missing" << std::endl;
                std::exit(1);
            }
        }
        m_buffer.push_back(d);
    }
}

void SignalFromFile::sendBuffer(const std::uint64_t N) {
    std::vector<double> outValues(N);

    // Send the data
    for (std::size_t i = 0; i < N; ++i) {
        outValues[i] = m_buffer[i];
    }

    m_outputChannels[0].send(outValues);
}

void SignalFromFile::checkFile() {
    m_file.open(m_path);
    if (!m_file.good()) {
        std::cerr << "Error: no data file \"" << m_path << "\" not found!" << std::endl;
        std::exit(1);
    }
}

void SignalFromFile::loadToRam() {
    double d;
    while (m_file >> d) {
        m_buffer.push_back(d);
    }

    // Check if the data was not empty
    if (m_buffer.size() == 0) {
        std::cerr << "Error: Not recognized file format or file was empty!" << std::endl;
        std::exit(1);
    }

    m_file.close();
}

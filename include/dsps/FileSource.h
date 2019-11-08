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

#ifndef FILE_SOURCE_H
#define FILE_SOURCE_H

#include "Task.h"

class FileSource: public Task {
public:
    enum class FileFormat {
        PlainInteger,
        PlainDouble,
        PlainComplex,
        BinaryInteger,
        BinaryDouble,
        BinaryComplex,
    };

public:
    /// Constructor
    FileSource(const std::string &filename, FileFormat format);

    /// \brief Read the signal form a file
    /// This is an override of Task::compute.
    ///
    /// \param N The window size
    virtual void compute(const std::uint64_t N) override;

    /// \brief Indicate if the task was ready for the compute
    /// This is an override of Task::compute.
    ///
    /// \param N The window size
    /// \return True if the task was ready else false
    virtual bool isReady(const std::uint64_t N) const override;

    /// \brief Indicate if the task was finished the compute
    /// This is an override of Task::hasFinished.
    ///
    /// \param N The window size
    /// \return True if the task was finished else false
    virtual bool hasFinished(const std::uint64_t N) const override;

private:
    template <typename T>
    void readPlainValues(std::vector<T> &values, std::uint64_t N);

    template <typename T>
    void readBinaryValues(std::vector<T> &values, std::uint64_t N) {
        values.clear();

        T value;
        while (values.size() < N) {
            m_file.read(reinterpret_cast<char*>(&value), sizeof(T));

            // If it's the end of file
            if (m_file.eof()) {
                m_file.clear();
                m_file.seekg(0);
                continue;
            }

            values.push_back(value);
        }
    }

private:
    FileFormat m_format;
    std::ifstream m_file;
};

#endif // FILE_SOURCE_H

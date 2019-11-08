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

#ifndef CONVERT_TYPE_H
#define CONVERT_TYPE_H

#include "Task.h"

class Channel;

template <typename InputType, typename OutputType>
class ConvertType: public Task {
public:
    /// Constructor
    ConvertType();

    /// Constructor
    ///
    /// \param nob Number of bit for the integer
    /// \param maxValue The max value from input (must be determinated by the user)
    ConvertType(std::uint8_t nob, InputType maxValue);

    /// \brief Compute the covertion of input
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
    const InputType m_MAX_VALUE;
    const std::uint64_t m_POWER_NOB;
    const double m_HOMOTHECY_FACTOR;
};

#endif // CONVERT_TYPE_H

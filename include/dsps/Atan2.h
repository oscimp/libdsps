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

#ifndef ATAN2_H
#define ATAN2_H

#include "Task.h"

class Atan2: public Task {
public:
    /// Constructor
    Atan2();

    /// \brief Compute the atan2 between the two input channels
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

    /// \brief Helper method to connect the I data
    /// By the way, the compute is atan2(q, i)
    ///
    /// \param task Task to get the I values
    /// \param index Index of input channel
    void connectIChannel(Task &task, std::size_t index = 0);

    /// \brief Helper method to connect the Q data
    /// By the way, the compute is atan2(q, i)
    ///
    /// \param task Task to get the Q values
    /// \param index Index of input channel
    void connectQChannel(Task &task, std::size_t index = 0);
};

#endif // ATAN2_H

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

#ifndef _NCO_H
#define _NCO_H

#include <vector>

#include "Task.h"

class Nco: public Task {
public:
    /// Constructor
    ///
    /// \param amplitude Signal amplitude
    /// \param signalFrequency Signal frequency
    /// \param sampleFrequency Sampling rate
    Nco(double amplitude, const double signalFrequency, const double sampleFrequency);

    /// \brief Compute the unwrap of each input channel
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
    std::size_t findCorrectLUTSize(const double POINTS_BY_PERIOD);

private:
    double m_amplitude;
    double m_signalFrequency;
    double m_sampleFrequency;
    const std::uint64_t m_LUT_SIZE;
    std::vector<double> m_cosLUT;
    std::vector<double> m_sinLUT;
    std::uint64_t m_index;
    std::uint64_t m_indexStep;
};

#endif // _NCO_H

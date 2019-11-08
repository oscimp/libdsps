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

#ifndef DEMODULATION_H
#define DEMODULATION_H

#include <vector>

#include "Task.h"

class Demodulation: public Task {
public:
    /// Constructor
    ///
    /// \param freqencySignal Frequency of signal to be demodulate
    /// \param sampleFrequency Sampling rate
    /// \param phaseOffset Offset phase between main signal and the demodulate signal
    Demodulation(double signalFrequency, double sampleFrequency, double phaseOffset);

    /// \brief Compute the demodulation of a signal
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
    double m_signalFrequency;
    double m_sampleFrequency;
    double m_phaseOffset;

    const std::size_t m_LUT_SIZE;
    std::uint64_t m_index;
    std::uint64_t m_indexStep;
    std::vector<double> m_cosLUT;
    std::vector<double> m_sinLUT;
};

#endif // DEMODULATION_H

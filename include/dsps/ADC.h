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

#ifndef ADC_H
#define ADC_H

#include <vector>

#include "Task.h"

class ADC: public Task {
public:
    /// Constructor
    ///
    /// \param signalFrequency Signal frequency
    /// \param samplingFrequency Sampling rate
    /// \param Vfsr Volt full scale range theorical
    /// \param powerScale Ratio between 1.0 and 0.0 to set the amplitude depending Vfsr
    ADC(double signalFrequency, double samplingFrequency, double Vfsr, double powerScale = 1.0);

    /// \brief Generate a signal like a ADC
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
    const double m_V_FSR;
    const double m_POWER_SCALE;
    const double m_FS;
    const double m_FC;
    const double m_DPN;
    const std::uint64_t m_POINTS_BY_PERIOD;
    std::uint64_t m_index;
    std::uint64_t m_indexStep;
};

#endif // ADC_H

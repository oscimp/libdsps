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

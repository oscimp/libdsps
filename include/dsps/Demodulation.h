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

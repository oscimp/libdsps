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

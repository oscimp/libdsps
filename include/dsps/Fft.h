#ifndef FFT_H
#define FFT_H

#include "WrapperFFTW.h"
#include "Task.h"


template<typename InputType>
class Fft: public Task {
public:
    /// Constructor
    Fft();

    /// \brief Compute the FFT of one input signal
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
    void computeFFT(const std::uint64_t N);

private:
    WrapperFFTW m_wrapperFFTW;
};

#endif // FFT_H

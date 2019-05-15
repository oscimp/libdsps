#ifndef _DETREND_H
#define _DETREND_H

#include <vector>

#include "Task.h"

class Detrend: public Task {
public:
    /// Constructor
    Detrend();

    /// Constructor to inject phase form the previous decade
    ///
    /// \param currentSampleFrequency Frequency sampling
    Detrend(double currentSampleFrequency);

    /// \brief Compute the linear regression and remove it
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
    void initLinReg();

private:
    std::size_t m_currentWindowSize;
    double m_xBarre;
    double m_xSquare;
    std::vector<double> m_xMxB;
    double m_fs;
    std::uint64_t m_currentIndexTime;
};

#endif // _DETREND_H

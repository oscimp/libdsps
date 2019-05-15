#ifndef CROSS_SPECTRUM_H
#define CROSS_SPECTRUM_H

#include <complex>

#include "Task.h"

class CrossSpectrum: public Task {
public:
    /// Constructor
    CrossSpectrum();

    /// \brief Compute a cross-spectrum with two inputs channels
    /// This is an override of Task::compute.
    /// Caution! This task return only N/2 data because the computing is symetric.
    ///
    /// \param N The window size
    virtual void compute(const std::uint64_t N) override;

    /// \brief Indicate if the task was ready for the compute
    /// This is an override of Task::compute.
    /// Caution! For a window size of N, the task was ready for N/2 input data.
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
};

#endif // CROSS_SPECTRUM_H

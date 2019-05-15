#ifndef _DECIMATION_H
#define _DECIMATION_H

#include <vector>

#include "Task.h"

class Decimation: public Task {
public:
    /// Constructor
    ///
    /// \param decimationFactor Decimation factor
    Decimation(std::uint64_t decimationFactor);

    /// \brief Decime the input flux by decimationFactor
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
    std::uint64_t m_decimationFactor;
};

#endif // _DECIMATION_H

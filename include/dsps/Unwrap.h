#ifndef _UNWRAP_H
#define _UNWRAP_H

#include "Task.h"

class Unwrap: public Task {
public:
    /// Constructor
    Unwrap();

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
};

#endif // _UNWRAP_H

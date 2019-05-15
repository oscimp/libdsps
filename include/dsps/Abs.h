#ifndef ABS_H
#define ABS_H

#include <vector>

#include "Task.h"

template<typename InputType, typename OutputType>
class Abs: public Task {
public:
    /// Constructor
    Abs();

    /// \brief Compute the absolute value for real data or module for complex data
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

#endif // ABS_H

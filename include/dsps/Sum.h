#ifndef SUM_H
#define SUM_H

#include <vector>

#include "Task.h"

template<typename T>
class Sum : public Task {
public:
    /// Constructor
    ///
    /// \param numberInput Number of input channel
    Sum(const std::size_t numberInput);

    /// \brief Compute a sum with N entry to 1 output (real or complex)
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
    T initAccum() const;
};

#endif // SUM_H

#ifndef MEAN_H
#define MEAN_H

#include <vector>

#include "Task.h"

template<typename T>
class Mean: public Task {
public:
    /// Constructor
    Mean(const std::uint64_t LIMIT_ORDER = 1);

    /// \brief Compute a mean
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

    /// \brief Get the number of mean
    std::uint64_t getNumberOfMean() const;

    /// \brief Rest the accumulator of mean
    void clearAccum();

private:
    void initAccum(const std::uint64_t N);

private:
    std::vector<T> m_accum;
    std::vector<T> m_kahanCompensation;
    std::uint64_t m_order;
    const std::uint64_t m_LIMIT_ORDER;
};

#endif // MEAN_H

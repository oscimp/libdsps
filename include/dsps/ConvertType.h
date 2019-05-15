#ifndef CONVERT_TYPE_H
#define CONVERT_TYPE_H

#include "Task.h"

class Channel;

template <typename InputType, typename OutputType>
class ConvertType: public Task {
public:
    /// Constructor
    ConvertType();

    /// Constructor
    ///
    /// \param nob Number of bit for the integer
    /// \param maxValue The max value from input (must be determinated by the user)
    ConvertType(std::uint8_t nob, InputType maxValue);

    /// \brief Compute the covertion of input
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
    const InputType m_MAX_VALUE;
    const std::uint64_t m_POWER_NOB;
    const double m_HOMOTHECY_FACTOR;
};

#endif // CONVERT_TYPE_H

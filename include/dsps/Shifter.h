#ifndef SHIFTER_H
#define SHIFTER_H

#include <dsps/Task.h>

template<typename InputType>
class Shifter: public Task{
public:
    /// Constructor
    Shifter(const std::int16_t shift);

    /// \brief Shift the data
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
    /// \brief Function wrapper to be able to specialize the divide operation
    InputType divideBy(const InputType value);

private:
    const std::int16_t m_shift;
};

#endif // SHIFTER_H

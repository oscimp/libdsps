#ifndef FIR_H
#define FIR_H

#include <string>
#include <vector>

#include "Task.h"

template<typename T>
class Fir: public Task {
public:
    /// Constructor
    Fir(const std::string &coeffPath, const std::uint64_t DECIM_FACTOR);

    /// \brief Filter the signal
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
    void filter(std::vector<T> &outValues, const std::uint64_t N);

private:
    std::vector<T> m_coeff;
    std::uint64_t m_DECIM_FACTOR;
    std::vector<T> m_inputBuffer;
};

#endif // FIR_H

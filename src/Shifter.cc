#include <dsps/Shifter.h>

template<typename InputType>
Shifter<InputType>::Shifter(const std::int16_t shift)
: Task(getChannelType<InputType>(), 1, getChannelType<InputType>(), 1)
, m_shift(shift) {
}

template<typename InputType>
void Shifter<InputType>::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Shifter: No input task is connected");

    std::vector<InputType> inValues(N);
    std::vector<InputType> outValues(N);

    m_inputChannels[0]->receive(inValues, N);

    // Case for intger values
    for (std::size_t i = 0; i < N; ++i) {
        outValues[i] = divideBy(inValues[i]);
    }
    // Case for floating point values

    m_outputChannels[0].send(outValues);
}

template<typename InputType>
bool Shifter<InputType>::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Shifter: No input task is connected");

    return m_inputChannels[0]->size(sizeof(InputType)) >= N;
}

template<typename InputType>
bool Shifter<InputType>::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(InputType)) >= N;
}

template<typename InputType>
InputType Shifter<InputType>::divideBy(const InputType value) {
    return value / std::pow(2, m_shift);
}

// Specialization for int64
template<>
std::int64_t Shifter<std::int64_t>::divideBy(const std::int64_t value) {
    return value >> m_shift;
}

template class Shifter<float>;
template class Shifter<double>;
template class Shifter<std::int64_t>;

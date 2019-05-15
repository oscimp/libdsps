#include <dsps/ConvertType.h>

#include <cmath>
#include <iostream>

#include <dsps/Channel.h>
#include <dsps/Utils.h>

template <typename InputType, typename OutputType>
ConvertType<InputType, OutputType>::ConvertType()
: Task(getChannelType<InputType>(), 1, getChannelType<OutputType>(), 1)
, m_MAX_VALUE(1)
, m_POWER_NOB(1)
, m_HOMOTHECY_FACTOR(1.0) {
    if (!std::is_integral<InputType>::value) {
        std::cerr << "Error: This constructor of ConvertType must be only use to convert integer point type to floating type." << std::endl;
        std::cerr << "Please change the constructor." << std::endl;
        std::exit(-1);
    }
}

template <typename InputType, typename OutputType>
ConvertType<InputType, OutputType>::ConvertType(std::uint8_t nob, InputType maxValue)
: Task(getChannelType<InputType>(), 1, getChannelType<OutputType>(), 1)
, m_MAX_VALUE(maxValue)
, m_POWER_NOB(std::pow(2, nob - 1) - 1)
, m_HOMOTHECY_FACTOR(m_POWER_NOB / maxValue) {
    if (std::is_integral<InputType>::value) {
        std::cerr << "Error: This constructor of ConvertType must be only use to convert floating point type to integer type." << std::endl;
        std::cerr << "Please change the constructor." << std::endl;
        std::exit(-1);
    }
}

template <typename InputType, typename OutputType>
void ConvertType<InputType, OutputType>::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "ConvertType: No input task is connected");

    std::vector<InputType> inValues(N);
    std::vector<OutputType> outValues(N);

    m_inputChannels[0]->receive(inValues, N);

    for (std::size_t i = 0; i < N; ++i) {
        InputType value = inValues[i];

        // Apply the scal
        if (std::is_integral<OutputType>::value){
            // Apply the gain
            value = value * m_POWER_NOB;

            // Round off
            value = std::round(1 / m_MAX_VALUE * value);
        }

        // Cast and send the value
        outValues[i] = static_cast<OutputType>(value);
    }

    m_outputChannels[0].send(outValues);
}

template <typename InputType, typename OutputType>
bool ConvertType<InputType, OutputType>::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "ConvertType: No input task is connected");

    return m_inputChannels[0]->size(sizeof(InputType)) >= N;
}

template <typename InputType, typename OutputType>
bool ConvertType<InputType, OutputType>::hasFinished(const std::uint64_t N) const {

    return m_outputChannels[0].size(sizeof(OutputType)) >= N;
}

template class ConvertType<double, std::int64_t>;
template class ConvertType<float, std::int64_t>;
template class ConvertType<std::int64_t, double>;
template class ConvertType<std::int64_t, float>;

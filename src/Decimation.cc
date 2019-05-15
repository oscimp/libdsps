#include <dsps/Decimation.h>

#include <dsps/Channel.h>

Decimation::Decimation(std::uint64_t decimationFactor)
: Task(ChannelType::Double, 1, ChannelType::Double, 1)
, m_decimationFactor(decimationFactor) {
}

void Decimation::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Decimation: No input task is connected");

    // Get the input data
    const std::uint64_t LIMIT = N * m_decimationFactor;
    std::vector<double> inValues(LIMIT);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(inValues, LIMIT);

    // Decim the flux
    for (std::size_t i = 0, j = 0; i < LIMIT; i += m_decimationFactor, ++j) {
        outValues[j] = inValues[i];
    }

    m_outputChannels[0].send(outValues);
}

bool Decimation::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Decimation: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N * m_decimationFactor;
}

bool Decimation::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

#include <dsps/Hanning.h>

#include <vector>

#include <dsac/t_pnm.h>

#include <dsps/Channel.h>

Hanning::Hanning()
: Task(ChannelType::Double, 1, ChannelType::Double, 1) {
}

void Hanning::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Hanning: No input task is connected");

    std::vector<double> inValues(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(inValues, N);

    // Compute the hanning window
    hanning_window(inValues.data(), outValues.data(), N);

    m_outputChannels[0].send(outValues);
}

bool Hanning::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Hanning: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N;
}

bool Hanning::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

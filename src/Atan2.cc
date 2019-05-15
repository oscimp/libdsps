#include <dsps/Atan2.h>

#include <cmath>

#include <dsps/Channel.h>

Atan2::Atan2()
: Task(ChannelType::Double, 2, ChannelType::Double, 1) {
}

void Atan2::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr) && "Atan2: No input task is connected");

    std::vector<double> in1Values(N);
    std::vector<double> in2Values(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(in1Values, N);
    m_inputChannels[1]->receive(in2Values, N);

    for (std::size_t i = 0; i < N; ++i) {
        outValues[i] = std::atan2(in1Values[i], in2Values[i]);
    }

    m_outputChannels[0].send(outValues);
}

bool Atan2::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr) && "Atan2: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N && m_inputChannels[1]->size(sizeof(double)) >= N;
}

bool Atan2::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

void Atan2::connectIChannel(Task &task, std::size_t index) {
    Task::connect(task, index, *this, 1);
}

void Atan2::connectQChannel(Task &task, std::size_t index) {
    Task::connect(task, index, *this, 0);
}

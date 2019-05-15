#include <dsps/NormalizePsddBc.h>

#include <complex>
#include <iostream>
#include <vector>

#include <dsac/t_pnm.h>

#include <dsps/Channel.h>

NormalizePsddBc::NormalizePsddBc(const double fs)
: Task(ChannelType::Double, 1, ChannelType::Double, 1)
, m_FS(fs) {
}

void NormalizePsddBc::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "NormalizePsddBc: No input task is connected");

    std::vector<double> inValues(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(inValues, N);

    // Normalize
    normalize_psd_dBc(inValues.data(), N, m_FS, outValues.data());

    m_outputChannels[0].send(outValues);
}

bool NormalizePsddBc::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "NormalizePsddBc: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N;
}

bool NormalizePsddBc::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

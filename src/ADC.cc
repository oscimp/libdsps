#include <dsps/ADC.h>

#include <cassert>
#include <cmath>

#include <dsps/Channel.h>

ADC::ADC(double signalFrequency, double samplingFrequency, double Vfsr, double powerScale)
: Task(ChannelType::Double, 1, ChannelType::Double, 1)
, m_V_FSR(Vfsr)
, m_POWER_SCALE(powerScale)
, m_FS(samplingFrequency)
, m_FC(signalFrequency)
, m_DPN(2.0 * M_PI * signalFrequency)
, m_POINTS_BY_PERIOD(findCorrectLUTSize(samplingFrequency / signalFrequency))
, m_index(m_indexStep) {
}

void ADC::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "ADC: No input task is connected");

    std::vector<double> noiseValues(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(noiseValues, N);

    const double STEP = 2.0 * M_PI * m_FC * (1.0 / m_FS) / m_indexStep;
    for (std::size_t i = 0; i < N; ++i) {
        double cos = std::cos(STEP * m_index + noiseValues[i]);
        double signal = 0.5 * m_V_FSR * cos * m_POWER_SCALE;

        m_index = (m_index + m_indexStep) % m_POINTS_BY_PERIOD;

        outValues[i] = signal;
    }

    m_outputChannels[0].send(outValues);
}

bool ADC::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "ADC: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N;
}

bool ADC::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

std::size_t ADC::findCorrectLUTSize(const double POINTS_BY_PERIOD) {
    std::uint64_t factor = 1;

    double intPart = 0.0;
    while (std::modf(factor * POINTS_BY_PERIOD, &intPart) != 0) {
        ++factor;
    }

    m_indexStep = factor;

    return static_cast<std::size_t>(std::round(factor * POINTS_BY_PERIOD));
}

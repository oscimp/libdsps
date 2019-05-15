#include <dsps/Demodulation.h>

#include <cmath>
#include <iostream>

#include <dsps/Channel.h>

Demodulation::Demodulation(double signalFrequency, double sampleFrequency, double phaseOffset)
: Task(ChannelType::Double, 1, ChannelType::Double, 2)
, m_signalFrequency(signalFrequency)
, m_sampleFrequency(sampleFrequency)
, m_phaseOffset(phaseOffset)
, m_LUT_SIZE(findCorrectLUTSize(sampleFrequency / signalFrequency))
, m_index(m_indexStep)
, m_cosLUT(m_LUT_SIZE)
, m_sinLUT(m_LUT_SIZE) {
    // Compute the LUT
    const double STEP = 2.0 * M_PI * m_signalFrequency * (1.0 / m_sampleFrequency) / m_indexStep;
    for (std::size_t i = 0; i < m_LUT_SIZE; ++i) {
        m_cosLUT[i] = std::cos(i * STEP + m_phaseOffset);
        m_sinLUT[i] = std::sin(i * STEP + m_phaseOffset);
    }
}

void Demodulation::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Demodulation: No input task is connected");

    std::vector<double> inValues(N);
    std::vector<double> iValues(N);
    std::vector<double> qValues(N);

    m_inputChannels[0]->receive(inValues, N);

    for (std::size_t i = 0; i < N; ++i) {
        double tmp = inValues[i];
        iValues[i] = tmp * m_cosLUT[m_index];
        qValues[i] = tmp * m_sinLUT[m_index];

        m_index = (m_index + m_indexStep) % m_LUT_SIZE;
    }

    m_outputChannels[0].send(iValues);
    m_outputChannels[1].send(qValues);
}

bool Demodulation::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Demodulation: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N;
}

bool Demodulation::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N && m_outputChannels[1].size(sizeof(double)) >= N;
}

std::size_t Demodulation::findCorrectLUTSize(const double POINTS_BY_PERIOD) {
    std::uint64_t factor = 1;

    double intPart = 0.0;
    while (std::modf(factor * POINTS_BY_PERIOD, &intPart) != 0) {
        ++factor;
    }

    // Total points
    std::uint64_t nb_points = static_cast<std::uint64_t>(std::round(factor * POINTS_BY_PERIOD));
    m_indexStep = factor;

    // If the lut ask too memory (more 2 Go with cos and sin lut)
    if ((nb_points * sizeof(double)) > 1e9) {
        std::cerr << "Demodulation::findCorrectLUTSize(): Too memory allocated!" << std::endl;
        std::exit(-1);
    }

    return nb_points;
}

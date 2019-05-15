#include <dsps/SignalGenerator.h>

#include <cmath>
#include <iostream>

#include <dsps/Channel.h>

#warning "The LUT version is not 100% equals with Octave"


SignalGenerator::SignalGenerator(double amplitude, double signalFrequency, double sampleFrequency)
: Task(ChannelType::None, 0, ChannelType::Double, 1)
, m_amplitude(amplitude)
, m_signalFrequency(signalFrequency)
, m_sampleFrequency(sampleFrequency)
, m_LUT_SIZE(findCorrectLUTSize(sampleFrequency / signalFrequency))
, m_index(m_indexStep)
, m_cosLUT(m_LUT_SIZE) {
    // Compute the LUT
    const double STEP = 2.0 * M_PI * m_signalFrequency * (1.0 / m_sampleFrequency) / m_indexStep;
    for (std::size_t i = 0; i < m_LUT_SIZE; ++i) {
        m_cosLUT[i] = std::cos(i * STEP);
    }
}

void SignalGenerator::compute(const std::uint64_t N) {
    std::vector<double> outValues(N);

    // Generate a perfect signal
    for (std::size_t i = 0; i < N; ++i) {
        outValues[i] = m_amplitude * m_cosLUT[m_index];
        m_index = (m_index + m_indexStep) % m_LUT_SIZE;
    }

    m_outputChannels[0].send(outValues);
}

bool SignalGenerator::isReady(const std::uint64_t N) const {
    USELESS_PARAMETER(N);

    return true;
}

bool SignalGenerator::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

std::size_t SignalGenerator::findCorrectLUTSize(const double POINTS_BY_PERIOD) {
    std::uint64_t factor = 1;

    double intPart = 0.0;
    while (std::modf(factor * POINTS_BY_PERIOD, &intPart) != 0) {
        ++factor;
    }

    // Total points
    std::uint64_t nb_points = static_cast<std::uint64_t>(std::round(factor * POINTS_BY_PERIOD));
    m_indexStep = factor;

    // If the lut ask too memory (more 1 Go)
    if ((nb_points * sizeof(double)) > 1e9) {
        std::cerr << "SignalGenerator::findCorrectLUTSize(): Too memory allocated!" << std::endl;
        std::exit(-1);
    }

    return nb_points;
}

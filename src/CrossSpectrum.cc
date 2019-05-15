#include <dsps/CrossSpectrum.h>

#include <complex>

#include <dsps/Channel.h>

CrossSpectrum::CrossSpectrum()
: Task(ChannelType::ComplexDouble, 2, ChannelType::Double, 1) {
}

void CrossSpectrum::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr) && "CrossSpectrum: No input task is connected");

    std::vector< std::complex<double> > in1Values(N);
    std::vector< std::complex<double> > in2Values(N);
    std::vector<double> outValues(N);

    m_inputChannels[0]->receive(in1Values, N);
    m_inputChannels[1]->receive(in2Values, N);

    // Compute the data
    for (std::size_t i = 0; i < N; ++i) {
        std::complex<double> tmp1 = in1Values[i];
        std::complex<double> tmp2 = in2Values[i];

        double a = tmp1.real();
        double b = tmp1.imag();
        double c = tmp2.real();
        double d = tmp2.imag();

        if (i < N/2) {
            // Compute the data
            outValues[i] = a * c + b * d;
        }
        else {
            // 0 padding
            outValues[i] = 0.0;
        }
    }

    m_outputChannels[0].send(outValues);
}

bool CrossSpectrum::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert((m_inputChannels[0] != nullptr && m_inputChannels[1] != nullptr) && "CrossSpectrum: No input task is connected");

    return m_inputChannels[0]->size(sizeof(std::complex<double>)) >= N && m_inputChannels[1]->size(sizeof(std::complex<double>)) >= N;
}

bool CrossSpectrum::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

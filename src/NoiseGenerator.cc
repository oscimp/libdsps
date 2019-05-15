#include <dsps/NoiseGenerator.h>

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include <dsps/Channel.h>
#include <dsps/Utils.h>

template <typename T>
NoiseGenerator<T>::NoiseGenerator(Random &random, double freqSignal, double freqSamples, double hp2, OutputType output)
: Task(ChannelType::None, 0, getChannelType<T>(), 1)
, m_random(random)
, m_freqSignal(freqSignal)
, m_freqSamples(freqSamples)
, m_hp2(hp2 / (freqSignal * freqSignal))
, m_outputType(output)
, m_fftForward(FFTDirection::Forward)
, m_fftBackward(FFTDirection::Backward) {
}

template <typename T>
void NoiseGenerator<T>::compute(const std::uint64_t N) {
    // Generate the normal noise
    generateNoise(N);

    std::complex<double> cplx;

    // Convert into deque
    std::vector<double> outValues(N);
    switch (m_outputType) {
    case OutputType::XTT:
    case OutputType::PHI:
        {
            /// Do not remove the mean substraction because it's avoid to create artefact at FS/N frequency
            // Compute the mean
            double mean = 0.0;
            for (std::size_t i = 0; i < N; ++i) {
                mean += m_data[i].real();
            }
            mean /= m_data.size();

            // Substract the mean and return value
            for (std::size_t i = 0; i < N; ++i) {
                outValues[i] = m_data[i].real() - mean;
            }
        }
        break;
    case OutputType::ARBITRARY_UNIT:
        for (std::size_t i = 0; i < N; ++i) {
            outValues[i] = m_data[i].real();
        }
        break;
    case OutputType::YTT:
        assert(false && "No yet implemented!");
        break;
    }

    m_outputChannels[0].send(outValues);
}

template <typename T>
bool NoiseGenerator<T>::isReady(const std::uint64_t N) const {
    USELESS_PARAMETER(N);
    return true;
}

template <typename T>
bool NoiseGenerator<T>::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(T)) >= N;
}

template <typename T>
void NoiseGenerator<T>::filter(const std::uint64_t N) {
    const std::size_t LIMIT = N / 2;
    const double TAU0 = 1 / m_freqSamples;
    double Ri = 0.0;
    double R2i = 0.0;
    double Rx = 0.0;

    // Compute the direct FFT
    computeFFT(FFTDirection::Forward, N);

    double cor = sqrt(0.0 / TAU0); /// 0 au lieu de h0

    m_data[0].real(m_data[0].real() * cor); /* on filtre l'amplitude de la fréquence nulle à part :    */
    m_data[0].imag(m_data[0].imag() * cor); /* seul h0 intervient pour ne pas multiplier par 0 cette   */

    for(std::size_t i = 1; i < LIMIT; ++i) {
        cor = static_cast<double>(i) / static_cast<double>(N);
        Ri  = cor / TAU0;
        R2i = Ri * Ri;

        // Rx=sqrt(fabs((double)(hm3/R2i/Ri + hm2/R2i + hm1/Ri + h0 + hp1*Ri + hp2*R2i))/tau0);
        // au lieu de
        // std::sqrt(std::abs(static_cast<double>(hm3/R2i/Ri + hm2/R2i + hm1/Ri + h0 + hp1*Ri + hp2*R2i))/tau0)
        Rx = std::sqrt(static_cast<double>(0.0 / R2i / Ri + 0.0 / R2i + 0.0 / Ri + 0.0 + 0.0 * Ri + m_hp2 * R2i) / TAU0);

        m_data[i].real(m_data[i].real() * Rx);           /* fréquences "positives" */
        m_data[i].imag(m_data[i].imag() * Rx);
        m_data[N - i].real(m_data[N - i].real() * Rx);   /* fréquences "négatives" */
        m_data[N - i].imag(m_data[N - i].imag() * Rx);
    }

    Ri  = 0.5 / TAU0;
    R2i = Ri * Ri;
    // std::sqrt(std::abs(static_cast<double>(0.0 / R2i / Ri + 0.0 / R2i + 0.0 / Ri + 0.0 + 0.0 * Ri + m_hp2 * R2i)) / TAU0)
    // au lieu de
    // std::sqrt(std::abs(static_cast<double>(hm3/R2i/Ri + hm2/R2i + hm1/Ri + h0 + hp1*Ri + hp2*R2i))/tau0)
    Rx = std::sqrt(std::abs(static_cast<double>(0.0 / R2i / Ri + 0.0 / R2i + 0.0 / Ri + 0.0 + 0.0 * Ri + m_hp2 * R2i)) / TAU0);

    m_data[LIMIT].real(m_data[LIMIT].real() * Rx);
    m_data[LIMIT].imag(m_data[LIMIT].imag() * Rx);

    // Compute the inverse FFT
    computeFFT(FFTDirection::Backward, N);
}

template <typename T>
void NoiseGenerator<T>::convertXTT(double x0, const std::uint64_t N) {
    const double TAU0 = 1 / m_freqSamples;
    double xint, yint;

    yint = m_data[0].real();
    m_data[0].real(x0);

    for(std::size_t i = 0; i < N - 1; ++i) {
        xint = m_data[i].real() + yint * TAU0;
        yint = m_data[i+1].real();
        m_data[i+1].real(xint);
    }
}

template <typename T>
void NoiseGenerator<T>::convertPhase(const std::uint64_t N) {
    for(std::size_t i = 0; i < N; ++i) {
        m_data[i].real(2 * M_PI * m_freqSignal * m_data[i].real());
    }
}

template <typename T>
void NoiseGenerator<T>::computeFFT(FFTDirection sign, const std::uint64_t N) {
    // Compute the FFT
    switch(sign) {
    case FFTDirection::Forward:
        m_fftForward.compute(m_data, m_data, N);
        break;
    case FFTDirection::Backward:
        m_fftBackward.compute(m_data, m_data, N);
        break;
    }

    // Normalize
    for (std::size_t i = 0; i < N; ++i) {
       m_data[i].real(m_data[i].real() / std::sqrt(static_cast<double>(N)));
       m_data[i].imag(m_data[i].imag() / std::sqrt(static_cast<double>(N)));
    }
}

template <typename T>
void NoiseGenerator<T>::reset() {
    m_data.clear();
}

template <typename T>
void NoiseGenerator<T>::generateNoise(const std::uint64_t N) {
    // Reset previous value
    reset();

    // Get a normal plage
    m_data.resize(N);
    m_random.computeNormalPlage(0.0, std::sqrt(2.0) / 2.0, m_data);

    // Generate the noise
    filter(N);

    // Convert the result in xtt
    switch (m_outputType) {
    case OutputType::XTT:
        convertXTT(0.0, N);
        break;
    case OutputType::PHI:
        convertXTT(0.0, N);
        convertPhase(N);
        break;
    default:
        break;
    }
}

template class NoiseGenerator<double>;
template class NoiseGenerator<float>;

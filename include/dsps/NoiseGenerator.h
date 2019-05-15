#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Random.h"
#include "Task.h"
#include "WrapperFFTW.h"

template <typename T>
class NoiseGenerator : public Task {
public:
    enum OutputType {
        XTT,
        YTT,
        PHI,
        ARBITRARY_UNIT,
    };

public:
    /// \brief Constructor
    ///
    /// \param random Random engine
    /// \param freqSignal Frequency of signal
    /// \param freqSamples Sampling rate of signal
    /// \param hp2 Noise f2 factor
    /// \param output Specify the unit of result
    NoiseGenerator(Random &random, double freqSignal, double freqSamples, double hp2, OutputType output);

    /// \brief Generate a noise
    /// This is an override of Task::compute.
    ///
    /// \param N The window size
    virtual void compute(const std::uint64_t N) override;

    /// \brief Indicate if the task was ready for the compute
    /// This is an override of Task::compute.
    ///
    /// \param N The window size
    /// \return True if the task was ready else false
    virtual bool isReady(const std::uint64_t N) const override;

    /// \brief Indicate if the task was finished the compute
    /// This is an override of Task::hasFinished.
    ///
    /// \param N The window size
    /// \return True if the task was finished else false
    virtual bool hasFinished(const std::uint64_t N) const override;

private:
    /// \brief Filter the data with the noise factors
    void filter(const std::uint64_t N);

    /// \brief Convert the ytt value in xtt
    ///
    /// \param x0 ???
    void convertXTT(double x0, const std::uint64_t N);


    /// \brief Convert the xtt value in phase
    void convertPhase(const std::uint64_t N);

    /// \brief Compute the fft then normalize the data
    ///
    /// \param sign The direction of FFT (forward or backward)
    /// \param N The window size
    void computeFFT(FFTDirection sign, const std::uint64_t N);


    /// \brief Reset the values
    void reset();

    /// \brief Create the initial normal noise
    void generateNoise(const std::uint64_t N);

private:
    Random &m_random; /// Random engine

    double m_freqSignal; /// Frequency of signal

    double m_freqSamples; /// Sampling rate

    double m_hp2; /// Noise f2 factor

    OutputType m_outputType; /// Specify the unit of result

    std::vector< std::complex<double> > m_data;
    WrapperFFTW m_fftForward;
    WrapperFFTW m_fftBackward;
};

#endif // NOISE_GENERATOR_H

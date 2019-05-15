#ifndef WRAPPER_FFTW_H
#define WRAPPER_FFTW_H

#include <fftw3.h>

#include <complex>
#include <mutex>
#include <vector>

/// Enum to handle the direction of FFT, the values are based on FFTW values
enum class FFTDirection : int {
    Forward = FFTW_FORWARD, ///< Direct FFT
    Backward = FFTW_BACKWARD, ///< Inverse FFT
};

class WrapperFFTW {
public:
    /// Constructor
    ///
    /// \param direction Direction of FFT (direct or inverse)
    WrapperFFTW(FFTDirection direction);

    // Rule of three
    virtual ~WrapperFFTW();
    WrapperFFTW (const WrapperFFTW& other) = delete;
    WrapperFFTW& operator= (const WrapperFFTW& other) = delete;

    /// \brief Compute a FFT
    ///
    /// \tparam T Type of input data (expected float or double)
    /// \param inputData Real value for the input
    /// \param outputData Computed complex data
    /// \param windowSize Size of FFT window
    template <typename T>
    void compute(const std::vector<T> &inputData, std::vector< std::complex<T> > &outputData, const std::uint64_t windowSize) {
        // Cast data to complex
        std::vector< std::complex<T> > dataComplex(windowSize);

        for (std::uint64_t i = 0; i < windowSize; ++i) {
            dataComplex[i].real(inputData[i]);
            dataComplex[i].imag(0.0);
        }

        // Call the fft
        compute(dataComplex, outputData, windowSize);
    }

    /// \brief Compute a FFT
    ///
    /// \tparam T Type of input data (expected float or double)
    /// \param inputData Complex value for the input
    /// \param outputData Computed complex data
    /// \param windowSize Size of FFT window
    template <typename T>
    void compute(const std::vector< std::complex<T> > &inputData, std::vector< std::complex<T> > &outputData, const std::uint64_t windowSize) {
        // Create the plan
        initPlan(windowSize);

        // Copy data
        for (std::uint64_t i = 0; i < windowSize; ++i) {
            m_inputData[i][0] = inputData[i].real();
            m_inputData[i][1] = inputData[i].imag();
        }

        // Compute
        compute();

        // Send data
        outputData.resize(windowSize);
        for (std::uint64_t i = 0; i < windowSize; ++i) {
            outputData[i] = std::complex<T>(m_outputData[i][0], m_outputData[i][1]);
        }
    }

private:
    /// Call the FFTW routine (maybe useless)
    void compute();

    /// Create and alloc the fftw plan and data
    ///
    /// \param windowSize Size of window
    void initPlan(const std::uint64_t windowSize);

    /// Free the data and the plan
    void freePlan();

private:
    static std::mutex mutexFFTW;
    static bool alreadyInit;

    std::uint64_t m_windowSize;
    FFTDirection m_fftSign;
    fftw_plan m_fftPlan;
    fftw_complex *m_inputData;
    fftw_complex *m_outputData;
};

#endif // WRAPPER_FFTW_H

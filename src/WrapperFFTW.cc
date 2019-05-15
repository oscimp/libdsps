#include <dsps/WrapperFFTW.h>

#include <iostream>
#include <thread>

std::mutex WrapperFFTW::mutexFFTW;
bool WrapperFFTW::alreadyInit = false;

WrapperFFTW::WrapperFFTW(FFTDirection direction)
: m_windowSize(0)
, m_fftSign(direction)
, m_inputData(nullptr)
, m_outputData(nullptr) {
    if (!alreadyInit) {
        if (fftw_init_threads() == 0) {
            std::cerr << "FFTW threads initialisation failed" << std::endl;
            std::exit(-1);
        }

        unsigned nbThreads = std::thread::hardware_concurrency();
        fftw_plan_with_nthreads(nbThreads);
        alreadyInit = true;
    }
}

WrapperFFTW::~WrapperFFTW() {
    freePlan();
}

void WrapperFFTW::compute() {
    fftw_execute(m_fftPlan);
}

void WrapperFFTW::initPlan(const std::uint64_t windowSize) {
    if (m_windowSize == windowSize) {
        return;
    }

    // Reset the plan
    freePlan();

    // Init the size
    m_windowSize = windowSize;

    // Alloc the plan
    {
        std::lock_guard<std::mutex> lock(mutexFFTW);
        m_inputData = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * m_windowSize));
        m_outputData = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * m_windowSize));
        m_fftPlan = fftw_plan_dft_1d(m_windowSize, m_inputData, m_outputData, static_cast<int>(m_fftSign), FFTW_ESTIMATE);
    }
}

void WrapperFFTW::freePlan() {
    if (m_inputData != nullptr) {
        std::lock_guard<std::mutex> lock(mutexFFTW);
        // Free the memory
        fftw_destroy_plan(m_fftPlan);
        fftw_free(m_inputData);
        fftw_free(m_outputData);
    }
}

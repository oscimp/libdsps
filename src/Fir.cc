#include <dsps/Fir.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <typeinfo>

#include <dsac/fir.h>

#include <dsps/Channel.h>

template<typename T>
Fir<T>::Fir(const std::string &coeffPath, const std::uint64_t DECIM_FACTOR)
: Task(getChannelType<T>(), 1, getChannelType<T>(), 1)
, m_DECIM_FACTOR(DECIM_FACTOR) {
    // Load the coefficients
    std::ifstream inFile;
    inFile.open(coeffPath);

    if (inFile.fail()) {
        std::cerr << "Fir::Fir(): The file '" << coeffPath << "' wasn't open: " << std::strerror(errno) << std::endl;
        std::exit(1);
    }

    T coeff;
    while (inFile >> coeff) {
        m_coeff.insert(m_coeff.begin(), coeff);
    }
    inFile.close();

    if (m_coeff.size() == 0) {
        std::cerr << "Fir::Fir(): The file '" << coeffPath << "' is empty!" << std::endl;
        std::exit(1);
    }
}

template<typename T>
void Fir<T>::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "FIR: No input task is connected");

    std::vector<T> inValues(N);
    std::vector<T> outValues(N);

    // Load the input data
    const std::uint64_t INPUT_SIZE = N * m_DECIM_FACTOR + m_coeff.size();
    const std::uint64_t INPUT_SIZE_DIFF = INPUT_SIZE - m_inputBuffer.size();

    m_inputChannels[0]->receive(inValues, INPUT_SIZE_DIFF);
    m_inputBuffer.insert(m_inputBuffer.end(), inValues.begin(), inValues.end());

    // Call the right method
    filter(outValues, INPUT_SIZE);

    // Send result
    m_outputChannels[0].send(outValues);

    // Clear input data
    m_inputBuffer.erase(m_inputBuffer.begin(), m_inputBuffer.begin() + (N * m_DECIM_FACTOR));
}

template<typename T>
bool Fir<T>::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "FIR: No input task is connected");

    return (m_inputChannels[0]->size(sizeof(T)) + m_inputBuffer.size()) >= (N * m_DECIM_FACTOR + m_coeff.size());
}

template<typename T>
bool Fir<T>::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(T)) >= N;
}

template<>
void Fir<double>::filter(std::vector<double> &outValues, const std::uint64_t INPUT_SIZE) {
    // Compute the fir
    fir_double(m_inputBuffer.data(), INPUT_SIZE, m_coeff.data(), m_coeff.size(), m_DECIM_FACTOR, outValues.data());
}

template<>
void Fir<float>::filter(std::vector<float> &outValues, const std::uint64_t INPUT_SIZE) {
    // Compute the fir
    fir_float(m_inputBuffer.data(), INPUT_SIZE, m_coeff.data(), m_coeff.size(), m_DECIM_FACTOR, outValues.data());
}

template<>
void Fir<std::int64_t>::filter(std::vector<std::int64_t> &outValues, const std::uint64_t INPUT_SIZE) {
    // Compute the fir
    fir_int64(m_inputBuffer.data(), INPUT_SIZE, m_coeff.data(), m_coeff.size(), m_DECIM_FACTOR, outValues.data());
}

template<typename T>
void Fir<T>::filter(std::vector<T> &outValues, const std::uint64_t N) {
    std::string error = std::string(typeid(T).name()) + " wasn't a supported type.";
    assert(false && error.data());
}

template class Fir<double>;
template class Fir<float>;
template class Fir<std::int64_t>;

#include <dsps/Sum.h>

#include <complex>
#include <typeinfo>

#include <dsps/Channel.h>

template<typename T>
Sum<T>::Sum(const std::size_t numberInput)
: Task(getChannelType<T>(), numberInput, getChannelType<T>(), 1) {
}

template<typename T>
void Sum<T>::compute(const std::uint64_t N) {
    // Check if the input task is connected
    bool allInputDefined = true;
    for (std::size_t i = 0; i < m_inputChannels.size() && allInputDefined; ++i) {
        allInputDefined = m_inputChannels[i] != nullptr;
    }
    assert(allInputDefined && "Sum: No input task is connected");

    // Init the input channel
    std::vector< std::vector<T> > inValues;
    for (std::size_t i = 0; i < m_inputChannels.size(); ++i) {
        inValues.push_back(std::vector<T>(N));
        m_inputChannels[i]->receive(inValues.back(), N);
    }

    std::vector<T> outValues(N);

    // Compute the sum
    for (std::size_t i = 0; i < N; ++i) {
        T sum = initAccum();
        for (std::size_t j = 0; j < m_inputChannels.size(); ++j) {
            sum += inValues[j][i];
        }
        outValues[i] = sum;
    }

    // Send the result
    m_outputChannels[0].send(outValues);
}

template<typename T>
bool Sum<T>::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    bool allInputDefined = true;
    for (std::size_t i = 0; i < m_inputChannels.size() && allInputDefined; ++i) {
        allInputDefined = m_inputChannels[i] != nullptr;
    }
    assert(allInputDefined && "Sum: No input task is connected");

    for (auto in: m_inputChannels) {
        if (in->size(sizeof(T)) < N) {
            return false;
        }
    }

    return true;
}

template<typename T>
bool Sum<T>::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(T)) >= N;
}

template<typename T>
T Sum<T>::initAccum() const {
    std::string error = std::string(typeid(T).name()) + " wasn't a supported type.";
    assert(false && error.data());

    return T();
}

template<>
double Sum<double>::initAccum() const {
    return 0.0;
}

template<>
std::complex<double> Sum<std::complex<double>>::initAccum() const {
    return std::complex<double>(0.0, 0.0);
}

template class Sum<double>;
template class Sum<std::complex<double>>;

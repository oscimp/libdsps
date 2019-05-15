#include <dsps/FileSource.h>

#include <cassert>

#include <dsps/Channel.h>

template <>
void FileSource::readPlainValues(std::vector< std::complex<double> > &values, std::uint64_t N) {
    values.clear();

    double real = 0.0;
    double imag = 0.0;
    while (values.size() < N) {
        m_file >> real;
        m_file >> imag;

        // If it's the end of file
        if (m_file.eof()) {
            m_file.clear();
            m_file.seekg(0);
            continue;
        }

        values.push_back(std::complex<double>(real, imag));
    }
}

template <typename T>
void FileSource::readPlainValues(std::vector<T> &values, std::uint64_t N) {
    values.clear();

    T value;
    while (values.size() < N) {
        m_file >> value;

        // If it's the end of file
        if (m_file.eof()) {
            m_file.clear();
            m_file.seekg(0);
            continue;
        }

        values.push_back(value);
    }
}

FileSource::FileSource(const std::string &filename, FileFormat format)
: Task(ChannelType::None, 0, ChannelType::None, 1)
, m_format(format)
, m_file(filename) {
    // Set the right output type
    switch(m_format){
        case FileFormat::PlainInteger:
        case FileFormat::BinaryInteger:
            m_outputChannelType = ChannelType::Int64;
            break;

        case FileFormat::PlainDouble:
        case FileFormat::BinaryDouble:
            m_outputChannelType = ChannelType::Double;
            break;

        case FileFormat::PlainComplex:
        case FileFormat::BinaryComplex:
            m_outputChannelType = ChannelType::ComplexDouble;
            break;
    }

    // Check the state of file
    assert(m_file.good());
}

void FileSource::compute(const std::uint64_t N) {
    switch(m_format){
        case FileFormat::PlainInteger:
        {
            std::vector<std::int64_t> outValues;
            readPlainValues(outValues, N);
            m_outputChannels[0].send(outValues);

            break;
        }

        case FileFormat::BinaryInteger:
        {
            std::vector<std::int64_t> outValues;
            readBinaryValues(outValues, N);
            m_outputChannels[0].send(outValues);

            break;
        }

        case FileFormat::PlainDouble:
        {
            std::vector<double> outValues;
            readPlainValues(outValues, N);
            m_outputChannels[0].send(outValues);

            break;
        }

        case FileFormat::BinaryDouble:
        {
            std::vector<double> outValues;
            readBinaryValues(outValues, N);
            m_outputChannels[0].send(outValues);

            break;
        }

        case FileFormat::PlainComplex:
        {
            std::vector< std::complex<double> > outValues;
            readPlainValues(outValues, N);
            m_outputChannels[0].send(outValues);

            break;
        }

        case FileFormat::BinaryComplex:
        {
            std::vector< std::complex<double> > outValues;
            readBinaryValues(outValues, N);
            m_outputChannels[0].send(outValues);

            break;
        }
    }
}

bool FileSource::isReady(const std::uint64_t N) const {
    USELESS_PARAMETER(N);

    return true;
}

bool FileSource::hasFinished(const std::uint64_t N) const {
    switch(m_format){
        case FileFormat::PlainInteger:
        case FileFormat::BinaryInteger:
            return m_outputChannels[0].size(sizeof(std::int64_t)) >= N;

        case FileFormat::PlainDouble:
        case FileFormat::BinaryDouble:
            return m_outputChannels[0].size(sizeof(double)) >= N;

        case FileFormat::PlainComplex:
        case FileFormat::BinaryComplex:
            return m_outputChannels[0].size(sizeof(std::complex<double>)) >= N;
    }

    return false;
}

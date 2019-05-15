#ifndef UTILS_H
#define UTILS_H

#include <complex>
#include <fstream>
#include <iomanip>
#include <limits>
#include <list>
#include <mutex>
#include <type_traits>
#include <vector>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <dsps/Channel.h>

class Task;

#define USELESS_PARAMETER(x) (void)(x)

namespace DSP {
    void processing(std::list<Task*> sourceTask, std::list<Task*> outputChannel, const std::uint64_t N);

    std::list<Task*> dagLinearisation(std::list<Task*> sourceTask);
}

class Writer {
public:
    enum WriteMode {
        OVERRIDE,       //< Delete the old file data
        APPEND,         //< Append the new data to the end of file
    };

    /// Write the data into a ascii file
    ///
    /// \param data Channel of data which will be write into file
    /// \param path Path for the file (if it already exits, it will be override otherwise it will be created)
    /// \param WINDOW_SIZE If this paramater was differtent of 0, the file will contains only the WINDOW_SIZE latest data
    /// \param mode Specify the mode of open file
    template <typename T>
    static void writeAsciiFile(Channel &data, const std::string &path, const std::uint64_t WINDOW_SIZE = 0, WriteMode mode = WriteMode::OVERRIDE) {
        std::ofstream file;
        if (mode == WriteMode::APPEND) {
            file.open(path, std::ios_base::app);
        }
        else {
            file.open(path);
        }

        // Set the max precision write
        if (!std::is_integral<T>::value) {
            file << std::scientific << std::setprecision(std::numeric_limits<double>::digits10 + 1);
        }

        // If we have to skip the first data
        if (WINDOW_SIZE != 0) {
            std::size_t numberSkip = data.size(sizeof(T)) / WINDOW_SIZE - 1;
            for (std::size_t j = 0; j < numberSkip; ++j) {
                std::vector<T> dataSkiped;
                data.receive(dataSkiped, WINDOW_SIZE);
                data.send(dataSkiped);
            }
        }

        // Read the all data and resend
        const std::size_t LIMIT = (WINDOW_SIZE == 0) ? data.size(sizeof(T)) : WINDOW_SIZE;
        std::vector<T> dataToWrite;
        data.receive(dataToWrite, LIMIT);

        for(std::size_t i = 0; i < LIMIT; ++i) {
            if (std::is_same<T, std::complex<double>>::value) {
                std::complex<double> c = static_cast<std::complex<double>>(dataToWrite[i]);
                file << c.real() << "\t" << c.imag() << std::endl;
            }
            else {
                file << dataToWrite[i] << std::endl;
            }
        }

        data.send(dataToWrite);
        file.close();
    }

    /// Write the data into a binary file
    ///
    /// \param data Channel of data which will be write into file
    /// \param path Path for the file (if it already exits, it will be override otherwise it will be created)
    /// \param WINDOW_SIZE If this paramater was differtent of 0, the file will contains only the WINDOW_SIZE latest data
    /// \param mode Specify the mode of open file
    template <typename T>
    static void writeBinaryFile(Channel &data, const std::string &path, const std::uint64_t WINDOW_SIZE = 0, WriteMode mode = WriteMode::OVERRIDE) {
        std::ofstream file;
        if (mode == WriteMode::APPEND) {
            file.open(path, std::ios_base::app|std::ios::binary);
        }
        else {
            file.open(path, std::ios::binary);
        }

        // If we have to skip the first data
        if (WINDOW_SIZE != 0) {
            std::size_t numberSkip = data.size(sizeof(T)) / WINDOW_SIZE - 1;
            for (std::size_t j = 0; j < numberSkip; ++j) {
                std::vector<T> dataSkiped;
                data.receive(dataSkiped, WINDOW_SIZE);
                data.send(dataSkiped);
            }
        }

        // Read the all data and resend
        const std::size_t LIMIT = (WINDOW_SIZE == 0) ? data.size(sizeof(T)) : WINDOW_SIZE;
        std::vector<T> dataToWrite;
        data.receive(dataToWrite, LIMIT);

        for(std::size_t i = 0; i < LIMIT; ++i) {
            file.write(reinterpret_cast<char*>(&(dataToWrite[i])), sizeof(T));
        }

        data.send(dataToWrite);
        file.close();
    }

    /// Write the data into a binary file
    ///
    /// \param data Channel of data which will be write into file
    /// \param path Path for the file (if it already exits, it will be override otherwise it will be created)
    template <typename T>
    static void writeBinaryFile(std::vector<T> &data, const std::string &path) {
        std::ofstream file;
        file.open(path, std::ios::binary);

        for (T value: data) {
            file.write(reinterpret_cast<char*>(&value), sizeof(T));
        }
        file.close();
    }

    /// Write the data into a gzip file
    ///
    /// \param data Channel of data which will be write into file
    /// \param path Path for the file (if it already exits, it will be override otherwise it will be created)
    /// \param WINDOW_SIZE If this paramater was differtent of 0, the file will contains only the WINDOW_SIZE latest data
    template <typename T>
    static void writeGzipFile(Channel &data, const std::string &path, const std::uint64_t WINDOW_SIZE = 0) {
        std::ofstream file;
        file.open(path, std::ios::binary);

        // Create the gzip wrapper
        boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
        out.push(boost::iostreams::gzip_compressor());

        // If we have to skip the first data
        if (WINDOW_SIZE != 0) {
            std::size_t numberSkip = data.size(sizeof(T)) / WINDOW_SIZE - 1;
            for (std::size_t j = 0; j < numberSkip; ++j) {
                std::vector<T> dataSkiped;
                data.receive(dataSkiped, WINDOW_SIZE);
                data.send(dataSkiped);
            }
        }

        // Read the all data and resend
        const std::size_t LIMIT = (WINDOW_SIZE == 0) ? data.size(sizeof(T)) : WINDOW_SIZE;
        std::vector<T> dataToWrite;
        data.receive(dataToWrite, LIMIT);
        data.send(dataToWrite);

        // Create the gzip file
        out.push(boost::iostreams::array_source(reinterpret_cast<char*>(dataToWrite.data()), dataToWrite.size() * sizeof(T)));
        boost::iostreams::copy(out, file);
        file.close();
    }
};

template <typename T>
ChannelType getChannelType() {
    if (std::is_same<T, double>::value) {
        return ChannelType::Double;
    }

    if (std::is_same<T, std::complex<double>>::value) {
        return ChannelType::ComplexDouble;
    }

    if (std::is_same<T, float>::value) {
        return ChannelType::Float;
    }

    if (std::is_same<T, std::complex<float>>::value) {
        return ChannelType::ComplexFloat;
    }

    if (std::is_same<T, std::int64_t>::value) {
        return ChannelType::Int64;
    }

    return ChannelType::None;
}

#endif // UTILS_H

#ifndef SIGNAL_FROM_FILE_H
#define SIGNAL_FROM_FILE_H

#include <deque>
#include <fstream>

#include "Task.h"

class SignalFromFile : public Task {
public:
    /// Enum which represent the method of reading file
    enum ReaderType {
        RAM,                    /// Load all file in ram
        STREAM,                 /// Read file in stream
        OVERLAPS,               /// Overlap this file
    };
public:
    /// Constructor
    /// The file will be stored in RAM
    ///
    /// \param path File which the signal is stocked
    SignalFromFile(const std::string &path);

    /// Constructor
    /// The fille will be read as a stream
    ///
    /// \param path File which the signal is stocked
    /// \param repeat Indicate if at the end of file we repeat the data
    SignalFromFile(const std::string &path, bool repeat);

    /// Constructor
    /// The fille will be overlaped
    ///
    /// \param path File which the signal is stocked
    /// \param repeat Indicate if at the end of file we repeat the data
    /// \param overlap Number of recycled data
    SignalFromFile(const std::string &path, bool repeat, std::uint64_t overlap);

    /// \brief Read a file and send a signal
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
    void readBuffer(const std::uint64_t N);
    void sendBuffer(const std::uint64_t N);
    void checkFile();
    void loadToRam();

private:
    std::string m_path;
    std::ifstream m_file;
    ReaderType m_reader;
    bool m_repeat;
    std::deque<double> m_buffer;
    std::size_t m_currentIndex;
    std::uint64_t m_overlap;
};

#endif // SIGNAL_FROM_FILE_H

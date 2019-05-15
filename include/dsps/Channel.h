#ifndef CHANNEL_H
#define CHANNEL_H

#include <cassert>
#include <cstdint>
#include <vector>

#include "Queue.h"

class Task;

enum class ChannelType {
    Double,
    ComplexDouble,
    Float,
    ComplexFloat,
    Int64,
    None,
};

class Channel {
public:
    /// Constructor
    Channel();

    Channel(const Channel&) = delete;
    Channel& operator=(const Channel&) = delete;

    Channel(Channel&& rhs) = delete;
    Channel& operator=(Channel&&) = delete;

    /// \brief Send a data to the output task
    ///
    /// \param values The new data to write
    template<typename T>
    void send(const std::vector<T> &values) {
        // Write the new data
        m_data.push(values.data(), values.size() * sizeof(T));
    }

    /// \brief Receive a data from the input task
    ///
    /// \param values This vector contains the values
    /// \param length Number of elements
    template<typename T>
    void receive(std::vector<T> &values, const std::size_t length) {
        assert((m_data.size() >= length * sizeof(T)) && "Error the channel is empty!");

        // Get the values
        values.resize(length);
        m_data.pop(values.data(), length * sizeof(T));
    }

    /// \brief Get the number of pending data in the channel
    /// \param dataSize Size of data needed
    ///
    /// \return The number of pending data
    std::size_t size(const std::size_t dataSize) const;

    /// \brief Get the input task
    /// If no input task was set an assertion was throw.
    ///
    /// \return The input task
    Task *getIn() const;

    /// \brief Get the output task
    ///
    /// \return The output task
    Task *getOut() const;

    /// \brief Set the input task
    /// If a task was already set an assertion was throw
    ///
    /// \param task The new input task
    void setIn(Task *task);

    /// \brief Set the output task
    /// If a task was already set an assertion was throw
    ///
    /// \param task The new output task
    void setOut(Task *task);

private:
    Task *m_inputTask;
    Task *m_outputTask;

    Queue m_data;
};

#endif // CHANNEL_H

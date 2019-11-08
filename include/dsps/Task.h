/* DSPS - library to build a digital signal processing simulation
 * Copyright (C) 2019  Arthur HUGEAT
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 */

#ifndef TASK_H
#define TASK_H

#include <cstdint>

#include "Channel.h"
#include "Utils.h"

class Task {
public:
    Task() = delete;

    /// Virtual destructor
    virtual ~Task() {

    }

    /// \brief The specific compute of the task
    ///
    /// \param N The window size
    virtual void compute(const std::uint64_t N) = 0;

    /// \brief Indicate if the task was ready for the compute
    ///
    /// \param N The window size
    /// \return True if the task was ready else false
    virtual bool isReady(const std::uint64_t N) const = 0;

    /// \brief Indicate if the task was finished the compute
    ///
    /// \param N The window size
    /// \return True if the task was finished else false
    virtual bool hasFinished(const std::uint64_t N) const = 0;

    /// \brief Get the number of next Task
    ///
    /// \return The number of next tasks
    std::size_t countNextTask() const;

    /// \brief Get the one next task
    ///
    /// \param i The number of task
    /// \return The task next task at the number i
    Task* getNextTask(const std::size_t i) const;

    /// \brief Set manualy the input channel (to debug mainly)
    ///
    /// \param channel The new input channel
    /// \param index Index of input channel
    void setInput(Channel &channel, const std::size_t index);

    /// \brief Get manualy the output channel (to debug mainly)
    ///
    /// \param index Index of output channel
    /// \return The output channel
    Channel& getOutput(const std::size_t index);

    /// \brief Connect an output of input task to an input of output task
    ///
    /// \param inputTask The reference of input task
    /// \param inputChannelIndex The index of output channel of input task
    /// \param outputTask The reference of output task
    /// \param outputChannelIndex The index of intput channel of output task
    static void connect(Task &inputTask, std::size_t inputChannelIndex, Task &outputTask, std::size_t outputChannelIndex);

    /// \brief Connect the first output of input task to the first input of output task
    ///
    /// \param inputTask The reference of input task
    /// \param inputChannelIndex The index of output channel of input task
    /// \param outputTask The reference of output task
    /// \param outputChannelIndex The index of intput channel of output task
    static void connect(Task &inputTask, Task &outputTask);

protected:
    /// \brief Create a task with the correct number of input/output
    ///
    /// \param inputType Type of input data
    /// \param numInput Number of input parameter
    /// \param outputType Type of output data
    /// \param numOutput Number of output parameter
    Task(ChannelType inputType, const std::size_t numInput, ChannelType outputType, const std::size_t numOutput);

protected:
    ChannelType m_inputChannelType;
    ChannelType m_outputChannelType;
    std::vector<Channel*> m_inputChannels;
    std::vector<Channel> m_outputChannels;
};

#endif // TASK_H

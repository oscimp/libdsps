#include <dsps/Task.h>

#include <cassert>

void Task::setInput(Channel &channel, const std::size_t index) {
    m_inputChannels[index] = &channel;
    channel.setOut(this);
}

Channel& Task::getOutput(const std::size_t index) {
    return m_outputChannels[index];
}

std::size_t Task::countNextTask() const {
    return m_outputChannels.size();
}

Task* Task::getNextTask(const std::size_t i) const {
    if (i >= m_outputChannels.size()) {
        return nullptr;
    }

    return m_outputChannels[i].getOut();
}

void Task::connect(Task &inputTask, std::size_t channelInputTaskIndex, Task &outputTask, std::size_t channelOutputTaskIndex) {
    // Check parameters
    assert(channelInputTaskIndex < inputTask.m_outputChannels.size() && "The index channel of input task is too big");
    assert(channelOutputTaskIndex < outputTask.m_inputChannels.size() && "The index channel of output task is too big");
    assert(inputTask.m_outputChannelType == outputTask.m_inputChannelType && "The type of channels don't match");

    // Connect the input task
    Channel &inputChannel = inputTask.m_outputChannels[channelInputTaskIndex];
    inputChannel.setOut(&outputTask);
    outputTask.m_inputChannels[channelOutputTaskIndex] = &inputChannel;
}

void Task::connect(Task &inputTask, Task &outputTask) {
    connect(inputTask, 0, outputTask, 0);
}

Task::Task(ChannelType inputType, const std::size_t numInput, ChannelType outputType, const std::size_t numOutput)
: m_inputChannelType(inputType)
, m_outputChannelType(outputType)
, m_inputChannels(numInput)
, m_outputChannels(numOutput) {
    // Connect all input task
    for (auto &channel: m_outputChannels) {
        channel.setIn(this);
    }
}

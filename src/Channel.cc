#include <dsps/Channel.h>

Channel::Channel()
: m_inputTask(nullptr)
, m_outputTask(nullptr) {

}

// Channel::Channel(Channel&& rhs)
// : m_inputTask(rhs.m_inputTask)
// , m_outputTask(rhs.m_outputTask)
// , m_data(rhs.m_data) {

// }

std::size_t Channel::size(const std::size_t dataSize) const {
    return m_data.size() / dataSize;
}

Task* Channel::getIn() const {
    assert(m_inputTask != nullptr && "Error the input Task isn't set!");

    return m_inputTask;
}

Task* Channel::getOut() const {
    return m_outputTask;
}

void Channel::setIn(Task *task) {
    assert(m_inputTask == nullptr && "Error the input Task is already set!");

    m_inputTask = task;
}

void Channel::setOut(Task *task) {
    assert(m_outputTask == nullptr && "Error the output Task is already set!");

    m_outputTask = task;
}
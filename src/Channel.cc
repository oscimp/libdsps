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

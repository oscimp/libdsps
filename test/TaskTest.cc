#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Task.h>

#include "local/Utils.h"

namespace {
    class MockTask: public Task {
    public:
        MockTask(ChannelType inputType, const std::size_t numInput, ChannelType outputType, const std::size_t numOutput)
        : Task(inputType, numInput, outputType, numOutput) {

        }

        MockTask(const std::size_t numInput = 1, const std::size_t numOutput = 1)
        : Task(ChannelType::Double, numInput, ChannelType::Double, numOutput) {

        }

        MOCK_METHOD1(compute, void(const std::uint64_t));
        MOCK_CONST_METHOD1(isReady, bool(const std::uint64_t));
        MOCK_CONST_METHOD1(hasFinished, bool(const std::uint64_t));

        static Channel& observerInputChannels(const MockTask &task, std::size_t numChannel) {
            return *(task.m_inputChannels[numChannel]);
        }

        static const Channel& observerOutputChannels(const MockTask &task, std::size_t numChannel) {
            return task.m_outputChannels[numChannel];
        }
    };

    TEST(TaskTest, testCountNextTask) {
        MockTask mock1(0, 5);
        EXPECT_EQ(static_cast<std::size_t>(5), mock1.countNextTask());

        MockTask mock2(0, 2);
        EXPECT_EQ(static_cast<std::size_t>(2), mock2.countNextTask());

        MockTask mock3(0, 0);
        EXPECT_EQ(static_cast<std::size_t>(0), mock3.countNextTask());

        MockTask mock4(0, 168);
        EXPECT_EQ(static_cast<std::size_t>(168), mock4.countNextTask());
    }

    TEST(TaskTest, testGetNextTaskAndConnectTasksNormalExit) {
        // Create an empty task
        MockTask task0(0, 5);

        for (std::size_t i = 0; i < 8; ++i) {
            EXPECT_EQ(nullptr, task0.getNextTask(i));
            if (i < 5) {
                EXPECT_EQ(nullptr, MockTask::observerOutputChannels(task0, i).getOut());
                EXPECT_EQ(&task0, MockTask::observerOutputChannels(task0, i).getIn());
            }

        }

        // Connect the first task
        MockTask task1(1, 0);
        Task::connect(task0, task1);
        EXPECT_EQ(&task1, task0.getNextTask(0));

        // Check the channel
        {
            const Channel &channel = MockTask::observerOutputChannels(task0, 0);
            EXPECT_EQ(&task0, channel.getIn());
            EXPECT_EQ(&task1, channel.getOut());
            for (std::size_t i = 1; i < 5; ++i) {
                EXPECT_EQ(nullptr, MockTask::observerOutputChannels(task0, i).getOut());
            }
            EXPECT_EQ(&channel, &MockTask::observerInputChannels(task1, 0));
        }

        // Connect the second task
        MockTask task2(3, 0);
        Task::connect(task0, 1, task2, 0);
        EXPECT_EQ(&task2, task0.getNextTask(1));

        // Check the channel
        {
            const Channel &channel = MockTask::observerOutputChannels(task0, 1);
            EXPECT_EQ(&task0, channel.getIn());
            EXPECT_EQ(&task2, channel.getOut());
            for (std::size_t i = 2; i < 5; ++i) {
                EXPECT_EQ(nullptr, MockTask::observerOutputChannels(task0, i).getOut());
            }
            EXPECT_EQ(&channel, &MockTask::observerInputChannels(task2, 0));
        }

        Task::connect(task0, 2, task2, 2);
        EXPECT_EQ(&task2, task0.getNextTask(2));

        // Check the channel
        {
            const Channel &channel = MockTask::observerOutputChannels(task0, 2);
            EXPECT_EQ(&task0, channel.getIn());
            EXPECT_EQ(&task2, channel.getOut());
            for (std::size_t i = 3; i < 5; ++i) {
                EXPECT_EQ(nullptr, MockTask::observerOutputChannels(task0, i).getOut());
            }
            EXPECT_EQ(&channel, &MockTask::observerInputChannels(task2, 2));
        }

        Task::connect(task0, 4, task2, 1);
        EXPECT_EQ(&task2, task0.getNextTask(4));

        // Check the channel
        {
            const Channel &channel = MockTask::observerOutputChannels(task0, 4);
            EXPECT_EQ(&task0, channel.getIn());
            EXPECT_EQ(&task2, channel.getOut());
            EXPECT_EQ(nullptr, MockTask::observerOutputChannels(task0, 3).getOut());
            EXPECT_EQ(&channel, &MockTask::observerInputChannels(task2, 1));
        }

        // Connect the last task
        MockTask task3(1, 0);
        Task::connect(task0, 3, task3, 0);
        EXPECT_EQ(&task3, task0.getNextTask(3));

        // Check the channel
        {
            const Channel &channel = MockTask::observerOutputChannels(task0, 3);
            EXPECT_EQ(&task0, channel.getIn());
            EXPECT_EQ(&task3, channel.getOut());
            for (std::size_t i = 0; i < 5; ++i) {
                EXPECT_NE(nullptr, MockTask::observerOutputChannels(task0, i).getOut());
            }
            EXPECT_EQ(&channel, &MockTask::observerInputChannels(task3, 0));
        }
    }

    TEST(TaskTest, testGetNextTaskAndConnectTasksFailure) {
        {
            // Create an empty task
            MockTask task0(0, 1);
            MockTask task1(0, 0);

            EXPECT_DEATH({ Task::connect(task0, task1); }, "The index channel of output task is too big");
        }

        {
            // Create an empty task
            MockTask task0(0, 0);
            MockTask task1(1, 0);

            EXPECT_DEATH({ Task::connect(task0, task1); }, "The index channel of input task is too big");
        }

        {
            // Create an empty task
            MockTask task0(0, 1);
            MockTask task1(1, 0);
            MockTask task2(1, 0);

            Task::connect(task0, task1);

            EXPECT_DEATH({ Task::connect(task0, task2); }, "Error the output Task is already set!");
        }
    }

    TEST(TaskTest, testCheckType) {
        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::Double, 1);
            MockTask task1(ChannelType::Double, 1, ChannelType::None, 0);

            Task::connect(task0, task1);
        }

        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::Float, 1);
            MockTask task1(ChannelType::Float, 1, ChannelType::None, 0);

            Task::connect(task0, task1);
        }

        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::ComplexDouble, 1);
            MockTask task1(ChannelType::ComplexDouble, 1, ChannelType::None, 0);

            Task::connect(task0, task1);
        }

        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::ComplexFloat, 1);
            MockTask task1(ChannelType::ComplexFloat, 1, ChannelType::None, 0);

            Task::connect(task0, task1);
        }

        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::Int64, 1);
            MockTask task1(ChannelType::Int64, 1, ChannelType::None, 0);

            Task::connect(task0, task1);
        }
    }

    TEST(TaskTest, testCheckTypeFailure) {
        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::Int64, 1);
            MockTask task1(ChannelType::Double, 1, ChannelType::None, 0);

            EXPECT_DEATH({ Task::connect(task0, task1); }, "The type of channels don't match");
        }

        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::Double, 1);
            MockTask task1(ChannelType::Float, 1, ChannelType::None, 0);

            EXPECT_DEATH({ Task::connect(task0, task1); }, "The type of channels don't match");
        }

        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::Float, 1);
            MockTask task1(ChannelType::ComplexDouble, 1, ChannelType::None, 0);

            EXPECT_DEATH({ Task::connect(task0, task1); }, "The type of channels don't match");
        }

        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::ComplexDouble, 1);
            MockTask task1(ChannelType::ComplexFloat, 1, ChannelType::None, 0);

            EXPECT_DEATH({ Task::connect(task0, task1); }, "The type of channels don't match");
        }

        {
            // Create an empty task
            MockTask task0(ChannelType::None, 0, ChannelType::ComplexFloat, 1);
            MockTask task1(ChannelType::Int64, 1, ChannelType::None, 0);

            EXPECT_DEATH({ Task::connect(task0, task1); }, "The type of channels don't match");
        }
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

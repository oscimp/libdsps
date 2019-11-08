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

#include <complex>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Task.h>

namespace {
    class MockTask: public Task {
    public:
        MockTask()
        : Task(ChannelType::Double, 1, ChannelType::Double, 1) {

        }

        MOCK_METHOD1(compute, void(const std::uint64_t));
        MOCK_CONST_METHOD1(isReady, bool(const std::uint64_t));
        MOCK_CONST_METHOD1(hasFinished, bool(const std::uint64_t));
    };

    TEST(ChannelTest, testSendReceiveSizeNormalExit) {
        // Test the double channel
        Channel channelDouble;
        std::vector<double> values;

        EXPECT_EQ(static_cast<std::size_t>(0), channelDouble.size(sizeof(double)));

        for (unsigned i = 0; i < 100; ++i) {
            values.push_back(i * 10.0);
        }
        channelDouble.send(values);

        EXPECT_EQ(static_cast<std::size_t>(100), channelDouble.size(sizeof(double)));

        channelDouble.receive(values, 100);
        for (unsigned i = 0; i < 100; ++i) {
            EXPECT_EQ(i * 10.0, values[i]);
        }

        EXPECT_EQ(static_cast<std::size_t>(0), channelDouble.size(sizeof(double)));

        // Test the complexe channel
        Channel channelComplex;
        std::vector< std::complex<double> > valuesComplex;

        EXPECT_EQ(static_cast<std::size_t>(0), channelComplex.size(sizeof(std::complex<double>)));

        for (unsigned i = 0; i < 100; ++i) {
            valuesComplex.push_back(std::complex<double>(i * 10.0, i / 10.0));
        }
        channelComplex.send(valuesComplex);

        EXPECT_EQ(static_cast<std::size_t>(100), channelComplex.size(sizeof(std::complex<double>)));

        channelComplex.receive(valuesComplex, 100);
        for (unsigned i = 0; i < 100; ++i) {
            EXPECT_EQ(std::complex<double>(i * 10.0, i / 10.0), valuesComplex[i]);
        }

        EXPECT_EQ(static_cast<std::size_t>(0), channelComplex.size(sizeof(std::complex<double>)));
    }

    TEST(ChannelTest, testReceiveFailExit) {
        // Test the double channel
        Channel channelDouble;
        std::vector<double> values;

        EXPECT_EQ(static_cast<std::size_t>(0), channelDouble.size(sizeof(double)));
        EXPECT_DEATH({ channelDouble.receive(values, 100); }, "Error the channel is empty!");

        // Test the complexe channel
        Channel channelComplex;
        std::vector< std::complex<double> > valuesComplex;

        EXPECT_EQ(static_cast<std::size_t>(0), channelComplex.size(sizeof(std::complex<double>)));
        EXPECT_DEATH({ channelComplex.receive(valuesComplex, 100); }, "Error the channel is empty!");
    }

    TEST(ChannelTest, testGetSetTasksNormalExit) {
        MockTask in;
        MockTask out;

        // Test the double channel
        Channel channelDouble;

        channelDouble.setIn(&in);
        channelDouble.setOut(&out);

        EXPECT_EQ(&in, channelDouble.getIn());
        EXPECT_EQ(&out, channelDouble.getOut());

        // Test the complex channel
        Channel channelComplex;

        channelComplex.setIn(&in);
        channelComplex.setOut(&out);

        EXPECT_EQ(&in, channelComplex.getIn());
        EXPECT_EQ(&out, channelComplex.getOut());
    }

    TEST(ChannelTest, testGetSetTasksFailExit) {
        MockTask in;
        MockTask out;

        // Test the double channel
        Channel channelDouble;

        EXPECT_DEATH({ channelDouble.getIn(); }, "Error the input Task isn't set!");

        channelDouble.setIn(&in);
        EXPECT_DEATH({ channelDouble.setIn(&out); }, "Error the input Task is already set!");

        channelDouble.setOut(&out);
        EXPECT_DEATH({ channelDouble.setOut(&in); }, "Error the output Task is already set!");

        EXPECT_EQ(&in, channelDouble.getIn());
        EXPECT_EQ(&out, channelDouble.getOut());

        // Test the complex channel
        Channel channelComplex;

        EXPECT_DEATH({ channelComplex.getIn(); }, "Error the input Task isn't set!");

        channelComplex.setIn(&in);
        EXPECT_DEATH({ channelComplex.setIn(&out); }, "Error the input Task is already set!");

        channelComplex.setOut(&out);
        EXPECT_DEATH({ channelComplex.setOut(&in); }, "Error the output Task is already set!");

        EXPECT_EQ(&in, channelDouble.getIn());
        EXPECT_EQ(&out, channelDouble.getOut());
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

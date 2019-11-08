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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/Utils.h>

#include "local/Utils.h"

namespace {
    TEST(UtilsTest, testWriteAsciiReal) {
        Channel channel;
        std::string path = "/tmp/test-utils.dat";
        double value;

        // Load the channel
        std::vector<double> data;
        for (std::size_t i = 0; i < 1000; ++i) {
            data.push_back(i * 3.14 / 1.98);
        }
        channel.send(data);

        // Write all data
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeAsciiFile<double>(channel, path);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));

        // Read the file
        std::ifstream file(path);
        for (std::size_t i = 0; i < 1000; ++i) {
            EXPECT_FALSE(file.eof());

            file >> value;

            EXPECT_EQ(data[i], value);
        }

        // Check if the end reached
        file >> value;
        EXPECT_TRUE(file.eof());
        file.close();

        // Write only N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeAsciiFile<double>(channel, path, 100);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));

        // Read the file
        file.open(path);
        for (std::size_t i = 900; i < 1000; ++i) {
            EXPECT_FALSE(file.eof());

            file >> value;

            EXPECT_EQ(data[i], value);
        }

        // Check if the end reached
        file >> value;
        EXPECT_TRUE(file.eof());
        file.close();

        // Append N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeAsciiFile<double>(channel, path, 100, Writer::WriteMode::APPEND);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));

        // Read the file
        file.open(path);
        for (std::size_t j = 0; j < 2; ++j) {
            for (std::size_t i = 900; i < 1000; ++i) {
                EXPECT_FALSE(file.eof());

                file >> value;

                EXPECT_EQ(data[i], value);
            }
        }

        // Check if the end reached
        file >> value;
        EXPECT_TRUE(file.eof());
        file.close();
    }

    TEST(UtilsTest, testWriteAsciiComplex) {
        Channel channel;
        std::string path = "/tmp/test-utils.dat";
        double valueReal;
        double valueImag;

        // Load the channel
        std::vector< std::complex<double> > data;
        for (std::size_t i = 0; i < 1000; ++i) {
            data.push_back(std::complex<double>(i * 3.14 / 1.98, i * 6544.34 - 723.687));
        }
        channel.send(data);

        // Write all data
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeAsciiFile< std::complex<double> >(channel, path);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));

        // Read the file
        std::ifstream file(path);
        for (std::size_t i = 0; i < 1000; ++i) {
            EXPECT_FALSE(file.eof());

            file >> valueReal;
            file >> valueImag;

            EXPECT_EQ(data[i].real(), valueReal);
            EXPECT_EQ(data[i].imag(), valueImag);
        }

        // Check if the end reached
        file >> valueReal;
        EXPECT_TRUE(file.eof());
        file.close();

        // Write only N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeAsciiFile< std::complex<double> >(channel, path, 100);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));

        // Read the file
        file.open(path);
        for (std::size_t i = 900; i < 1000; ++i) {
            EXPECT_FALSE(file.eof());

            file >> valueReal;
            file >> valueImag;

            EXPECT_EQ(data[i].real(), valueReal);
            EXPECT_EQ(data[i].imag(), valueImag);
        }

        // Check if the end reached
        file >> valueReal;
        EXPECT_TRUE(file.eof());
        file.close();

        // Append N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeAsciiFile< std::complex<double> >(channel, path, 100, Writer::WriteMode::APPEND);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));

        // Read the file
        file.open(path);
        for (std::size_t j = 0; j < 2; ++j) {
            for (std::size_t i = 900; i < 1000; ++i) {
                EXPECT_FALSE(file.eof());

                file >> valueReal;
                file >> valueImag;

                EXPECT_EQ(data[i].real(), valueReal);
                EXPECT_EQ(data[i].imag(), valueImag);
            }
        }

        // Check if the end reached
        file >> valueReal;
        EXPECT_TRUE(file.eof());
        file.close();
    }

    TEST(UtilsTest, testWriteBinaryReal) {
        Channel channel;
        std::string pathCha = "/tmp/test-utils.bin";
        std::string pathVec = "/tmp/test-utils-vec.bin";
        double value;

        // Load the channel
        std::vector<double> data;
        for (std::size_t i = 0; i < 1000; ++i) {
            data.push_back(i * 3.14 / 1.98);
        }
        channel.send(data);

        // Write all data
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeBinaryFile<double>(channel, pathCha);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeBinaryFile<double>(data, pathVec);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));

        // Read the file
        std::ifstream fileCha(pathCha);
        std::ifstream fileVec(pathVec);
        for (std::size_t i = 0; i < 1000; ++i) {
            // Check the channel file
            EXPECT_FALSE(fileCha.eof());

            fileCha.read(reinterpret_cast<char*>(&value), sizeof(double));
            EXPECT_EQ(data[i], value);

            // Check the channel file
            EXPECT_FALSE(fileVec.eof());

            fileVec.read(reinterpret_cast<char*>(&value), sizeof(double));
            EXPECT_EQ(data[i], value);
        }

        // Check if the end reached
        fileCha >> value;
        EXPECT_TRUE(fileCha.eof());
        fileCha.close();
        fileVec >> value;
        EXPECT_TRUE(fileVec.eof());
        fileVec.close();

        // Write only N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeBinaryFile<double>(channel, pathCha, 100);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));

        // Read the file
        fileCha.open(pathCha);
        for (std::size_t i = 900; i < 1000; ++i) {
            EXPECT_FALSE(fileCha.eof());

            fileCha.read(reinterpret_cast<char*>(&value), sizeof(double));
            EXPECT_EQ(data[i], value);
        }

        // Check if the end reached
        fileCha >> value;
        EXPECT_TRUE(fileCha.eof());
        fileCha.close();

        // Append N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeBinaryFile<double>(channel, pathCha, 100, Writer::WriteMode::APPEND);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));

        // Read the file
        fileCha.open(pathCha);
        for (std::size_t j = 0; j < 2; ++j) {
            for (std::size_t i = 900; i < 1000; ++i) {
                EXPECT_FALSE(fileCha.eof());

                fileCha.read(reinterpret_cast<char*>(&value), sizeof(double));
                EXPECT_EQ(data[i], value);
            }
        }

        // Check if the end reached
        fileCha >> value;
        EXPECT_TRUE(fileCha.eof());
        fileCha.close();
    }

    TEST(UtilsTest, testWriteBinaryComplex) {
        Channel channel;
        std::string pathCha = "/tmp/test-utils.bin";
        std::string pathVec = "/tmp/test-utils-vec.bin";
        double valueReal;
        double valueImag;

        // Load the channel
        std::vector< std::complex<double> > data;
        for (std::size_t i = 0; i < 1000; ++i) {
            data.push_back(std::complex<double>(i * 3.14 / 1.98, i * 6544.34 - 723.687));
        }
        channel.send(data);

        // Write all data
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeBinaryFile< std::complex<double> >(channel, pathCha);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeBinaryFile< std::complex<double> >(data, pathVec);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));

        // Read the file
        std::ifstream fileCha(pathCha);
        std::ifstream fileVec(pathVec);
        for (std::size_t i = 0; i < 1000; ++i) {
            // Check the channel file
            EXPECT_FALSE(fileCha.eof());

            fileCha.read(reinterpret_cast<char*>(&valueReal), sizeof(double));
            fileCha.read(reinterpret_cast<char*>(&valueImag), sizeof(double));
            EXPECT_EQ(data[i].real(), valueReal);
            EXPECT_EQ(data[i].imag(), valueImag);

            // Check the vector file
            EXPECT_FALSE(fileVec.eof());

            fileVec.read(reinterpret_cast<char*>(&valueReal), sizeof(double));
            fileVec.read(reinterpret_cast<char*>(&valueImag), sizeof(double));
            EXPECT_EQ(data[i].real(), valueReal);
            EXPECT_EQ(data[i].imag(), valueImag);
        }

        // Check if the end reached
        fileCha >> valueReal;
        EXPECT_TRUE(fileCha.eof());
        fileCha.close();
        fileVec >> valueReal;
        EXPECT_TRUE(fileVec.eof());
        fileVec.close();

        // Write only N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeBinaryFile< std::complex<double> >(channel, pathCha, 100);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));

        // Read the file
        fileCha.open(pathCha);
        for (std::size_t i = 900; i < 1000; ++i) {
            EXPECT_FALSE(fileCha.eof());

            fileCha.read(reinterpret_cast<char*>(&valueReal), sizeof(double));
            fileCha.read(reinterpret_cast<char*>(&valueImag), sizeof(double));

            EXPECT_EQ(data[i].real(), valueReal);
            EXPECT_EQ(data[i].imag(), valueImag);
        }

        // Check if the end reached
        fileCha >> valueReal;
        EXPECT_TRUE(fileCha.eof());
        fileCha.close();

        // Append N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeBinaryFile< std::complex<double> >(channel, pathCha, 100, Writer::WriteMode::APPEND);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));

        // Read the file
        fileCha.open(pathCha);
        for (std::size_t j = 0; j < 2; ++j) {
            for (std::size_t i = 900; i < 1000; ++i) {
                EXPECT_FALSE(fileCha.eof());

                fileCha.read(reinterpret_cast<char*>(&valueReal), sizeof(double));
                fileCha.read(reinterpret_cast<char*>(&valueImag), sizeof(double));
                EXPECT_EQ(data[i].real(), valueReal);
                EXPECT_EQ(data[i].imag(), valueImag);
            }
        }

        // Check if the end reached
        fileCha >> valueReal;
        EXPECT_TRUE(fileCha.eof());
        fileCha.close();
    }

    TEST(UtilsTest, testWriteGzipReal) {
        Channel channel;
        std::string pathGzip = "/tmp/test-utils.bin.gz";
        std::string pathBin = "/tmp/test-utils.bin";
        std::string command = "gunzip -f " + pathGzip;
        double value;

        // Load the channel
        std::vector<double> data;
        for (std::size_t i = 0; i < 1000; ++i) {
            data.push_back(i * 3.14 / 1.98);
        }
        channel.send(data);

        // Write all data
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeGzipFile<double>(channel, pathGzip);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));

        // Unzip the file
        system(command.c_str());

        // Read the file
        std::ifstream file(pathBin);
        for (std::size_t i = 0; i < 1000; ++i) {
            // Check the channel file
            EXPECT_FALSE(file.eof());

            file.read(reinterpret_cast<char*>(&value), sizeof(double));
            EXPECT_EQ(data[i], value);
        }

        // Check if the end reached
        file >> value;
        EXPECT_TRUE(file.eof());
        file.close();

        // Write only N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));
        Writer::writeGzipFile<double>(channel, pathGzip, 100);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(double)));

        // Unzip the file
        system(command.c_str());

        // Read the file
        file.open(pathBin);
        for (std::size_t i = 900; i < 1000; ++i) {
            EXPECT_FALSE(file.eof());

            file.read(reinterpret_cast<char*>(&value), sizeof(double));
            EXPECT_EQ(data[i], value);
        }

        // Check if the end reached
        file >> value;
        EXPECT_TRUE(file.eof());
        file.close();
    }

    TEST(UtilsTest, testWriteGzipComplex) {
        Channel channel;
        std::string pathGzip = "/tmp/test-utils.bin.gz";
        std::string pathBin = "/tmp/test-utils.bin";
        std::string command = "gunzip -f " + pathGzip;
        double valueReal;
        double valueImag;

        // Load the channel
        std::vector< std::complex<double> > data;
        for (std::size_t i = 0; i < 1000; ++i) {
            data.push_back(std::complex<double>(i * 3.14 / 1.98, i * 6544.34 - 723.687));
        }
        channel.send(data);

        // Write all data
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeGzipFile< std::complex<double> >(channel, pathGzip);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));

        // Unzip the file
        system(command.c_str());

        // Read the file
        std::ifstream file(pathBin);
        for (std::size_t i = 0; i < 1000; ++i) {
            // Check the channel file
            EXPECT_FALSE(file.eof());

            file.read(reinterpret_cast<char*>(&valueReal), sizeof(double));
            file.read(reinterpret_cast<char*>(&valueImag), sizeof(double));
            EXPECT_EQ(data[i].real(), valueReal);
            EXPECT_EQ(data[i].imag(), valueImag);
        }

        // Check if the end reached
        file >> valueReal;
        EXPECT_TRUE(file.eof());
        file.close();

        // Write only N last values
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));
        Writer::writeGzipFile< std::complex<double> >(channel, pathGzip, 100);
        EXPECT_EQ(static_cast<std::size_t>(1000), channel.size(sizeof(std::complex<double>)));

        // Unzip the file
        system(command.c_str());

        // Read the file
        file.open(pathBin);
        for (std::size_t i = 900; i < 1000; ++i) {
            EXPECT_FALSE(file.eof());

            file.read(reinterpret_cast<char*>(&valueReal), sizeof(double));
            file.read(reinterpret_cast<char*>(&valueImag), sizeof(double));
            EXPECT_EQ(data[i].real(), valueReal);
            EXPECT_EQ(data[i].imag(), valueImag);
        }

        // Check if the end reached
        file >> valueReal;
        EXPECT_TRUE(file.eof());
        file.close();
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

#include <cmath>
#include <complex>

#include <fftw3.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <dsps/Channel.h>
#include <dsps/NoiseGenerator.h>

#include "local/Utils.h"

namespace {
    void computeDBC(std::vector<double> &values) {
        const size_t N = values.size();

        // Compute the fft
        // Allocate the fftw parameter
        fftw_complex *data = nullptr;
        fftw_plan fftForward;

        data = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * N));
        fftForward = fftw_plan_dft_1d(N, data, data, FFTW_FORWARD, FFTW_MEASURE);

        // Initialise values
        for (std::size_t i = 0; i < N; ++i) {
            data[i][0] = values[i];
            data[i][1] = 0.0;
        }

        fftw_execute(fftForward);

        // Normalize
        for (std::size_t i = 0; i < N / 2; ++i) {
            std::complex<double> cpl(data[i][0], data[i][1]);
            double tmp = 4.0 * std::abs(cpl * std::conj(cpl)) / std::pow(N, 2);
            values[i] = 10 * std::log10(tmp) - 10 * std::log10(250e6 / N);
        }

        // Erase the useless values
        values.erase(values.begin(), values.begin() + 49);
        values.erase(values.begin() + (N / 2 - 99), values.end());

        // Free the memory
        fftw_destroy_plan(fftForward);
        fftw_free(data);
    }

    TEST(NoiseGeneratorTest, testComputeReady) {
        static constexpr unsigned N = 2048;
        Random random;

        // Alloc task
        NoiseGenerator<double> task(random, 10e6, 250e6, 1e-16, NoiseGenerator<double>::OutputType::PHI);
        Channel &out = task.getOutput(0);

        // Check if the channel was correct
        EXPECT_EQ(&task, out.getIn());

        // The task is always ready
        EXPECT_TRUE(task.isReady(0));
        EXPECT_TRUE(task.isReady(N));
        EXPECT_TRUE(task.isReady(N / 2));

        // Open the oracle file
        std::ifstream file(std::string(ORACLE_DATA_DIR) + "/oracle_noise_output.bin", std::ios_base::in|std::ios_base::binary);
        ASSERT_TRUE(file.good());
        auto oracleVec = extractVectorFromOracleFile<double>(file, 2);

        for (std::size_t i = 0; i < 1000; ++i) {
            EXPECT_TRUE(task.isReady(N));
            EXPECT_FALSE(task.hasFinished(N));
            task.compute(N);
            EXPECT_TRUE(task.isReady(N));
            EXPECT_TRUE(task.hasFinished(N));

            std::vector<double> actualVec;
            out.receive(actualVec, out.size(sizeof(double)));

            // Compute the dBC to compare
            computeDBC(actualVec);

            double sum = std::accumulate(actualVec.begin(), actualVec.end(), 0.0);
            double mean = sum / actualVec.size();

            double sq_sum = std::inner_product(actualVec.begin(), actualVec.end(), actualVec.begin(), 0.0);
            double stdev = std::sqrt(sq_sum / actualVec.size() - mean * mean);

            expect_eq_double(oracleVec[oracleVec.size() - 1], mean, 1);
            expect_eq_double(oracleVec[oracleVec.size() - 2], stdev, 1);
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

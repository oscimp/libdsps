#include <dsps/Unwrap.h>

#include <cmath>
#include <iostream>

#include <dsac/t_pnm.h>

#include <dsps/Channel.h>

Unwrap::Unwrap()
: Task(ChannelType::Double, 1, ChannelType::Double, 1) {
}

void Unwrap::compute(const std::uint64_t N) {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Unwrap: No input task is connected");

    // Unwrap algo from here
    // https://www.medphysics.wisc.edu/~ethan/phaseunwrap/unwrap.c
    std::vector<double> p(N);
    double *dp = new double[N];
    double *dps = new double[N];
    double *dp_corr = new double[N];
    double *cumsum = new double[N];
    double cutoff = M_PI;               /* default value in matlab */
    std::size_t j;

    // Check if alloc is OK
    if (dp == nullptr || dps == nullptr || dp_corr == nullptr || cumsum == nullptr) {
        std::cerr << "Unwrap::compute(): Bad allocation" << std::endl;
        std::exit(-1);
    }

    m_inputChannels[0]->receive(p, N);

   // incremental phase variation
   // MATLAB: dp = diff(p, 1, 1);
    for (j = 0; j < N-1; j++) {
        dp[j] = p[j+1] - p[j];
    }

   // equivalent phase variation in [-pi, pi]
   // MATLAB: dps = mod(dp+dp,2*pi) - pi;
    for (j = 0; j < N-1; j++) {
        dps[j] = (dp[j]+M_PI) - floor((dp[j]+M_PI) / (2*M_PI))*(2*M_PI) - M_PI;
    }

   // preserve variation sign for +pi vs. -pi
   // MATLAB: dps(dps==pi & dp>0,:) = pi;
    for (j = 0; j < N-1; j++) {
        if ((dps[j] == -M_PI) && (dp[j] > 0)) {
            dps[j] = M_PI;
        }
    }

   // incremental phase correction
   // MATLAB: dp_corr = dps - dp;
    for (j = 0; j < N-1; j++){
        dp_corr[j] = dps[j] - dp[j];
    }

   // Ignore correction when incremental variation is smaller than cutoff
   // MATLAB: dp_corr(abs(dp)<cutoff,:) = 0;
    for (j = 0; j < N-1; j++) {
        if (fabs(dp[j]) < cutoff) {
            dp_corr[j] = 0;
        }
    }

   // Find cumulative sum of deltas
   // MATLAB: cumsum = cumsum(dp_corr, 1);
    cumsum[0] = dp_corr[0];
    for (j = 1; j < N-1; j++){
        cumsum[j] = cumsum[j-1] + dp_corr[j];
    }

   // Integrate corrections and add to P to produce smoothed phase values
   // MATLAB: p(2:m,:) = p(2:m,:) + cumsum(dp_corr,1);
    for (j = 1; j < N; j++){
        p[j] += cumsum[j-1];
    }

    // Send result
    m_outputChannels[0].send(p);

    // Free the memory
    delete[] dp;
    delete[] dps;
    delete[] dp_corr;
    delete[] cumsum;
}

bool Unwrap::isReady(const std::uint64_t N) const {
    // Check if the input task is connected
    assert(m_inputChannels[0] != nullptr && "Unwrap: No input task is connected");

    return m_inputChannels[0]->size(sizeof(double)) >= N;
}

bool Unwrap::hasFinished(const std::uint64_t N) const {
    return m_outputChannels[0].size(sizeof(double)) >= N;
}

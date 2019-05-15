#include <dsps/Random.h>

#include <algorithm>
#include <functional>

// see http://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
// and http://www.pcg-random.org/posts/cpp-seeding-surprises.html
static std::mt19937 getCorrectlyInitializedEngine() {
    std::mt19937::result_type data[std::mt19937::state_size];
    std::random_device source;
    std::generate(std::begin(data), std::end(data), std::ref(source));
    std::seed_seq seeds(std::begin(data), std::end(data));
    return std::mt19937(seeds);
}

Random::Random()
: m_engine(getCorrectlyInitializedEngine()) {
}

Random::Random(std::uint64_t seed)
: m_engine(seed) {
}

void Random::computeNormalPlage(const double mean, const double stddev, std::vector< std::complex<double> > &data) {
    std::normal_distribution<double> dist(mean, stddev);
    for (auto &d: data) {
        d.real(dist(m_engine));
        d.imag(0.0);
    }
}

#include <dsps/Avar.h>

#include <fstream>

void Avar::getResult() const {
    return m_result;
}

void Avar::compute(const std::string &dataFile) {

}

void Avar::compute(const std::vector<double> &data) {
    m_result.clear();
}

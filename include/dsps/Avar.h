#ifndef AVAR_H
#define AVAR_H

#include <vector>

class Avar {
public:
    void getResult() const;

    void compute(const std::string &dataFile);
    void compute(const std::vector<double> &data);

private:
    std::vector<double> m_result;
};

#endif // AVAR_H

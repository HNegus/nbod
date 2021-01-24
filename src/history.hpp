#pragma once

#include "utils.hpp"

class History {

private:
    std::vector<real> m_positions;
    unsigned char[4] m_color;
    unsigned int m_resolution = 1;

public:

    History(unsigned char[4] color) : m_color(color) {};
    ~History() {};

    void Add(vec3 position);
    void Clear();

};

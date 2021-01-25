#pragma once

#include "utils.hpp"
#include "world.hpp"
#include "body.hpp"

class Logger {

private:
    bool currently_logging = false;
    World *world;
    std::ofstream ofs;

public:

    Logger() : world() {};
    ~Logger() {};

    void Start(std::string name, World *world);
    void Log();
    void Close();
};

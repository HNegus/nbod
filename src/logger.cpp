#include "logger.hpp"

void Logger::Start(std::string name, World *world) {

    this->world = world;

    ofs.open(LOG_DIR + name + "-" + std::to_string(std::time(0)) + ".log");

    ofs << world->GetBodyCount() << std::endl;
    ofs <<  world->GetBodyNames() << std::endl;
    currently_logging = true;
    Log();
}

void Logger::Log() {

    if (!currently_logging) return;
    ofs << world->LogString() << std::endl;
}

void Logger::Close() {

    ofs.close();
    currently_logging = false;
}

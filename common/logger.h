#pragma once

#include <iostream>

namespace log
{
    char endl = '\n';
}

class Logger
{
    int debugLevel_;
    std::ostream* s_;
public:
    template<class T>
    Logger& operator<<(const T& t);
    template<class T>
    Logger& operator<<(T* t);
    Logger(std::ostream* str_v) : s_(str_v) {}
    void SetDebugLevel(const int i) {debugLevel_ = i;};
};

template<class T>
Logger& Logger::operator<<(const T& t)
{
    if (Logger::debugLevel_ > 0)
    {
        *s_ << t;
    }
    return *this;
}

template<class T>
Logger& Logger::operator<<(T* t)
{
    if (Logger::debugLevel_ > 0)
    {
        *s_ << t;
    }
    return *this;
}

Logger logger(&std::cout);
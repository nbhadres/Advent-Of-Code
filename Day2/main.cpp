#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <exception>

#include "logger.h"

#define PUZZLE_PART_2

constexpr int MAX_PW_LENGTH = 31;

class Condition
{
private:
    int min_occurances_;
    int max_occurances_;
    char c_;
    
public:
    bool Apply(const std::string& line);
    Condition(int min, int max, char c);
};

Condition::Condition(int min, int max, char c) : min_occurances_(min), max_occurances_(max), c_(c)
{
    if (max_occurances_ > MAX_PW_LENGTH)
    {
        std::cerr << "Max occurances ["<<  max_occurances_ << "] exceeds length allowed [" << MAX_PW_LENGTH << "]. Please increase limit and try again." << std::endl;
        throw std::unexpected;
    }
}

bool Condition::Apply(const std::string& line)
{
    #ifndef PUZZLE_PART_2
    uint count = 0;
    for (auto c: line)
    {
        if (c == c_)
        {
            if (++count > max_occurances_)
            {
                return false;
            }
        }
    }
    return count >= min_occurances_;
    #else
    return ((line[min_occurances_ - 1] == c_) != (line[max_occurances_ - 1] == c_));
    #endif
}

uint Solve(const char* inputFile)
{
    std::ifstream input(inputFile);
    std::string line;
    uint passedConditions = 0;
    if (input.is_open())
    {
        while (getline (input,line))
        {
            int min = 0; 
            int max = 0;
            char c = '\0';
            char password[MAX_PW_LENGTH] = "";
            if (sscanf(line.c_str(), "%u-%u %c: %s", &min, &max, &c, password) != 4)
            {
                logger << "min: "<< min << ". max: "  << max << ". char: " << c << ". password: " << password << log::endl;
                throw std::invalid_argument(line);
            }
            Condition cond(min, max, c); 
            logger << "line: "  << line << log::endl;
            logger << "min: "<< min << ". max: "  << max << ". char: " << c << ". password: " << password  << ". Applied: " << cond.Apply(password) << log::endl ;
            passedConditions += cond.Apply(password) ? 1 : 0;
        }
    }
    std::cout << "Number of passed conditions is: " << passedConditions << log::endl;
    return passedConditions;
}

int main()
{
    logger.SetDebugLevel(0);
    Solve("input.txt");
    return 0;
}
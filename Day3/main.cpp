#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "logger.h"

struct MatchingCriteria
{
    MatchingCriteria(char s, int r, int d) :
        symbol(s),
        right(r),
        down(d)
    {}     

    char symbol;
    int right;
    int down;
};

class Solver
{
    private:
    unsigned int linesRead_;
    std::ifstream input_;
    void ResetInput();
    bool DoesLineMatch(const std::string& line, const MatchingCriteria& match);
    
    public:
    void SetMatchCriteria(char s, int r, int d);
    Solver(const std::string& file);
    int Solve(const MatchingCriteria& match);
};

Solver::Solver(const std::string& file) :
    input_(file), 
    linesRead_(0)
{
    if (!input_.is_open())
    {
        throw;
    }
}

void Solver::ResetInput()
{
    if (linesRead_ == 0)
    {
        return;
    }

    input_.clear();
    input_.seekg(0);
    linesRead_ = 0;
}

bool Solver::DoesLineMatch(const std::string& line, const MatchingCriteria& match)
{
    const auto pos = match.right * linesRead_;
    if (linesRead_++ % match.down > 0)
    {
        return false;
    }
    if (line[pos % line.length()] == match.symbol)
    {
        return true;
    }
    return false;

}

int Solver::Solve(const MatchingCriteria& match)
{
    ResetInput();
    
    std::string line;
    int occurances = 0;
    while (getline (input_, line))
    {
        if (DoesLineMatch(line, match))
        {
            ++occurances;
        }
    }
    logger << "For matching conditions: [" << match.symbol << ", " << match.right << ", " << match.down <<"], the number of occurances is: " << occurances << log::endl;
    return occurances;
}

int main()
{
    logger.SetDebugLevel(1);
    MatchingCriteria match();
    Solver solver("input.txt");
    std::cout << "Mulitplication of all conditions is: " << log::endl <<
        (unsigned long long) (solver.Solve({'#', 1, 1})) * solver.Solve({'#', 3, 1}) * solver.Solve({'#', 5, 1}) * solver.Solve({'#', 7, 1}) * solver.Solve({'#', 1, 2}) << std::endl;

    return 0;
}
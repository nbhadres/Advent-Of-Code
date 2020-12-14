#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

class Solver
{
public: 
    typedef  unsigned long returntype;
    returntype Solve(const char* inputFile);

private:
    std::vector<int> ReadFromFile(const char* inputFile);
    returntype Solve(const std::vector<int>& input);
};

std::vector<int> Solver::ReadFromFile(const char* inputFile)
{
    std::ifstream input(inputFile);
    std::string line;
    std::vector<int> inputVec;
    if (input.is_open())
    {
        while (getline (input,line))
        {
            inputVec.emplace_back(std::stoi(line));
        }
    }
    std::cout << "Successful reading of file. Number of lines read: " << inputVec.size() << std::endl;
    return inputVec;
}

Solver::returntype Solver::Solve(const char* inputFile)
{
    const auto solution = Solve(ReadFromFile(inputFile));
    if (solution == 0)
    {
        std::cerr << "Solver could not find a solution to the input file [" << inputFile << "]" << std::endl;
    }
    std::cout << "Solver found the solution as: " << solution << std::endl;
    return solution;
}

Solver::returntype Solver::Solve(const std::vector<int>& input)
{
    std::array<bool, 2020> pos = {}; 
    for (auto i : input)
    {
        if (i > 0 && i < 2020)
        {
            if (pos[i])
            {
                std::cout << "Two numbers which add up to 2020 are: " << i << ", and " << 2020 - i << std::endl;
                return i * (2020 - i);
            }
            else
            {
                pos[2020 - i] = true;
            }
        }
    }   
    return 0;
}

int main()
{
    Solver solver;
    const auto solution = solver.Solve("input.txt");
    return solution > 0;
}
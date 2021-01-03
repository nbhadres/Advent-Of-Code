#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <optional>
#include <map>
#include <algorithm>

#include "logger.h"

namespace
{
    namespace delimiters
    {
        constexpr char keyValue = ':';
        constexpr char keyEntry = ' ';
        constexpr char passportEntry = '\n';
    } // namespace delimiters

    const std::vector<std::string> expectedFields = {
        "byr",
        "iyr",
        "eyr",
        "hgt",
        "hcl",
        "ecl",
        "pid",
        "cid"
    };

    const std::vector<std::string> requiredFields = {
        "byr",
        "iyr",
        "eyr",
        "hgt",
        "hcl",
        "ecl",
        "pid"
    };
}

class Passport
{
    std::map<std::string, std::string> m_fields;
    void AddFieldsFromKeyValue(const std::string& keyValue);

public:
    void AddFieldsFromLine(std::string line);
    bool Validate();
};

bool Passport::Validate()
{
    if (m_fields.size() < requiredFields.size())
    {
        logger << "returning false because fields size: " << m_fields.size() << " is less than than the required amount: " << requiredFields.size() << log::endl;
        return false;
    }
    for (auto iter = requiredFields.begin(); iter != requiredFields.end(); ++iter)
    {
        if (std::find_if(m_fields.begin(), m_fields.end(), [iter](auto& p) {return *iter == p.first;}) == m_fields.end())
        {
            logger << "Returning false because of missing field: " << *iter << log::endl;
            return false;
        }
    }
    return true;
}

void Passport::AddFieldsFromLine(std::string line)
{
    size_t keyEntryPos = 0;
    do
    {
        keyEntryPos = line.find(delimiters::keyEntry);
        const std::string keyValuePair = line.substr(0, keyEntryPos);
        logger << "Key value pair is: " << keyValuePair << log::endl;
        AddFieldsFromKeyValue(keyValuePair);

        line.erase(0, keyEntryPos + 1);
    } while (keyEntryPos != std::string::npos);

}

void Passport::AddFieldsFromKeyValue(const std::string& keyValue)
{
    const size_t keyValuePos = keyValue.find(delimiters::keyValue);
    if (keyValuePos != std::string::npos)
    {
        const std::string key = keyValue.substr(0, keyValuePos);
        const std::string value = keyValue.substr(keyValuePos+1);
        m_fields.emplace(key, value);
        logger << "Key [" << key << "], value [" << value << "]" << log::endl;
    }
}


class Solver
{
private:
    std::vector<Passport> m_passports;
    std::fstream m_input;

public:
    Solver(const std::string& inputFile);  
    int Solve();
};

Solver::Solver(const std::string& inputFile) : m_input(inputFile)
{
    std::fstream input(inputFile);
    if (!input.is_open())
    {
        throw;
    }
    m_passports.emplace_back();
}

int Solver::Solve()
{
    std::string line;
    while (getline (m_input, line))
    {
        if (line.empty())
        {
            logger << "Moving to next passport, this passport was: " << m_passports.back().Validate()  << log::endl;

            m_passports.emplace_back();
            continue;
        }
        m_passports.back().AddFieldsFromLine(line);
    }
    unsigned int i = 0;
    for (auto iter = m_passports.begin(); iter != m_passports.end(); ++iter)
    {
        i += iter->Validate() ? 1 : 0;
    }
    return i;
}

void ReadLine(std::string& line)
{
    logger << "Reading line: " << line << log::endl;
    if (line.empty())
    {
        return;
    }
}

int main()
{
    logger.SetDebugLevel(1);
    Solver solver("input.txt");
    const int solution = solver.Solve();
    std::cout << "Solution is: " << solution << log::endl;
    return 0;
}
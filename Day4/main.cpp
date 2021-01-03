#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <functional>

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

    bool StringBoundsCheck(const std::string& value, const int lowerBound, const int upperBound)
    {
        const int val = std::stoi(value);
        if (val >= lowerBound && val <= upperBound)
        {
            return true;
        }
        return false;
    }

    bool byrValidation(const std::string& value)
    {
        return StringBoundsCheck(value, 1920, 2002);
    }

    bool iyrValidation(const std::string& value)
    {
        return StringBoundsCheck(value, 2010, 2020);
    }

    bool eyrValidation(const std::string& value)
    {
        return StringBoundsCheck(value, 2020, 2030);
    }

    bool hgtValidation(const std::string& value)
    {
        if (value.length() < 3)
        {
            return false;
        }
        const std::string unit = value.substr(value.length() - 2);
        if (unit == "cm")
        {
            return StringBoundsCheck(value.substr(0, value.length() - 2), 150, 193);
        }
        if (unit == "in")
        {
            return StringBoundsCheck(value.substr(0, value.length() - 2), 59, 76);
        }
        return false;
    }

    bool hclValidation(const std::string& value)
    {
        if (value.size() != 7)
        {
            return false;
        }
        if (value[0] != '#')
        {
            return false;
        }
        for (int i = 1; i < value.size(); ++i)
        {
            if (!(::isdigit(value[i])) && (value[i] < 'a' && value[i] > 'f'))
            {
                return false;
            }
        }
        return true;
    }

    bool eclValidation(const std::string& value)
    {
        static const std::vector<std::string> eyeColours = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        if (std::find(eyeColours.begin(), eyeColours.end(), value) != eyeColours.end())
        {
            return true;
        }
        return false;
    }

    bool pidValidation(const std::string& value)
    {
        if (value.size() != 9)
        {
            return false;
        }
        for (int i = 0; i < value.size(); ++i)
        {
            if (!::isdigit(value[i]))
            {
                return false;
            }
        }
        return true;
    }

    const std::map<std::string, std::function<bool(const std::string&)>> requiredFields = {
        {"byr", byrValidation},
        {"iyr", iyrValidation},
        {"eyr", eyrValidation},
        {"hgt", hgtValidation},
        {"hcl", hclValidation},
        {"ecl", eclValidation},
        {"pid", pidValidation}
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
        if (std::find_if(m_fields.begin(), m_fields.end(), [iter](auto& p) {return (iter->first == p.first && iter->second(p.second));}) == m_fields.end())
        {
            logger << "Returning false because of missing or invalid field: " << iter->first << log::endl;
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
    unsigned int solution = 0;
    for (auto iter = m_passports.begin(); iter != m_passports.end(); ++iter)
    {
        solution += iter->Validate() ? 1 : 0;
    }
    return solution;
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
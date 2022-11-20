#pragma once

#include <string>


using std::string;

class Category
{
private:

    string name;
    string code;

public:

    Category(string code, string name)
    {
        this->code = code;
        this->name = name;
    }

    string get_name()
    {
        return this->name;
    }

    string get_code()
    {
        return this->code;
    }
};
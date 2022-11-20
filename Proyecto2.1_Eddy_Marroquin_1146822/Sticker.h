#pragma once

#include <string>
#include "Category.h"

using std::string;

class Sticker
{
private:

    int number;
    string name;
    Category* category;
    int appearances;

public:

    Sticker(Category* category, int number, string name)
    {
        this->category = category;
        this->number = number;
        this->name = name;

        this->appearances = 0;
    }

    int get_number()
    {
        return this->number;
    }

    string get_code()
    {
        return category->get_code() + std::to_string(this->number);
    }

    string get_name()
    {
        return this->name;
    }

    Category* get_category() {
        return this->category;
    }

    void mark() {
        this->appearances++;
    }

    void unmark() {
        this->appearances--;
    }

    bool is_marked() {
        return this->appearances > 0;
    }

    bool is_repeated() {
        return this->appearances > 1;
    }

    int get_appeareances() {
        return this->appearances;
    }
};
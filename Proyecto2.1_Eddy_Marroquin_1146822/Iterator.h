#pragma once



template<typename T>
class Iterator {
public:

    virtual Iterator<T>* next() = 0;

    virtual T* value() = 0;

    virtual bool has_next() = 0;
};

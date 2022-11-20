#pragma once
#include "Iterator.h"
#include "Comparator.h"
#include <functional>



template <typename T>
class List {
public:

    virtual int get_size() = 0;

    virtual bool is_empty() = 0;

    virtual bool is_not_empty() {
        return !is_empty();
    }

    virtual void add(T* value) = 0;

    virtual bool insert(int index, T* value) = 0;

    virtual T* get(int index) = 0;

    virtual T* remove(int index) = 0;

    virtual Iterator<T>* iterator() = 0;

    virtual void for_each(void (*action)(T*)) = 0;

    virtual List<T>* bubble_sort(int (*comparator)(T*, T*)) = 0;

    virtual List<T>* merge_sort(Comparator<T>* comparator) = 0;

    virtual List<T>* filter(std::function<bool(T*)> predicate) = 0;

    virtual T* binary_search(std::function<int(T*)> compare) = 0;
};
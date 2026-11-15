#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <stdexcept>

using namespace std;

template <typename T>
class Collection
{
    private:
    vector<T> items;
    int linearSearch(const T& item);

    public:
    Collection() = default;
    Collection(vector<T> item) : items(item) {}

    bool isEmpty() const;
    int size() const;
    vector<T> getItems() const;
    bool makeEmpty();
    void sort();
    bool insert(const T& item);
    bool remove(const T& item);
    int search(const T& item);
    bool contains(const T& item);
    T at(int index) const;
    void print() const;
    string convertBoolToString(bool boolean);

    void save(const string& filename) const;
    void load(const string& filename);
};

template <typename T>
bool Collection<T>::isEmpty() const
{
    return items.size() == 0;
}

template <typename T>
int Collection<T>::size() const
{
    return items.size();
}

template <typename T>
vector<T> Collection<T>::getItems() const
{
    return items;
}

template <typename T>
bool Collection<T>::makeEmpty()
{
    int originalSize = items.size();
    items.clear();
    return items.size() < originalSize;
}

template <typename T>
bool Collection<T>::insert(const T& item)
{
    // check for duplicates. if so, do not add it

    if (!contains(item))
    {
        items.push_back(item);
        return true;
    }

    return false;
}

template <typename T>
bool Collection<T>::remove(const T& item)
{
    // checks if the value already exists. if so, remove it

    if (contains(item))
    {
        items.erase(items.begin() + search(item));
        return true;
    }

    return false;
}

template <typename T>
int Collection<T>::search(const T& item)
{
    // binary search

    int low = 0, high = items.size() - 1, middle = 0;

    while (low <= high)
    {
        middle = (low + high) / 2;

        if (items[middle] == item)
        {
            return middle;
        }
        else if (items[middle] < item)
        {
            low = middle + 1;
        }
        else if (items[middle] > item)
        {
            high = middle - 1;
        }
    }

    return -1;
}

template <typename T>
int Collection<T>::linearSearch(const T& item)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i] == item)
        {
            return i;
        }
    }

    return -1;
}

template <typename T>
bool Collection<T>::contains(const T& item)
{
    int searchResult = linearSearch(item);

    return searchResult != -1;
}

template <typename T>
T Collection<T>::at(int index) const
{
    if (index < 0 || index >= items.size())
    {
        throw out_of_range("Index is out of range");
    }

    return items[index];
}

template <typename T>
void Collection<T>::sort()
{
    // insertion sort

    for (int i = 1; i < items.size(); i++)
    {
        T temp = items[i];
        int j = i - 1;
        while (j >= 0 && items[j] > temp)
        {
            items[j + 1] = items[j];
            j--;
        }
        items[j + 1] = temp;
    }
}

template <typename T>
void Collection<T>::print() const
{
    for (const auto& item : items)
    {
        cout << item << " ";
    }
}

template <typename T>
string Collection<T>::convertBoolToString(bool boolean)
{
    return boolean ? "yes" : "no";
}

template <typename T>
void Collection<T>::save(const string& filename) const
{
    ofstream file(filename);

    for (const auto& item : items)
    {
        file << item << endl;
    }

    file.close();
}

template <typename T>
void Collection<T>::load(const string& filename)
{
    ifstream file(filename);

    T item;

    items.clear();

    while (file >> item)
    {
        items.push_back(item);
    }

    file.close();
}
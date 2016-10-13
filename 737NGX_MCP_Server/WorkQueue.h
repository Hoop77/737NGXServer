#pragma once


#include <queue>
#include <mutex>


template <typename T>
class WorkQueue
{
public:
    typedef typename std::queue<T>::size_type size_type;

    bool empty() const;
    size_type size() const;
    void add( const T & item );
    T & remove();

private:
    std::queue<T> theQueue;
    std::mutex queueMutex;
    std::condition_variable notEmptyCondition;
};


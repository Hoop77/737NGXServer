#include "WorkQueue.h"


template <typename T>
bool
WorkQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock( queueMutex );
    return theQueue.empty();
}


template <typename T>
typename WorkQueue<T>::size_type
WorkQueue<T>::size() const
{
    std::lock_guard<std::mutex> lock( queueMutex );
    return theQueue.size();
}


template <typename T>
void
WorkQueue<T>::add( const T & item )
{
    std::lock_guard<std::mutex> lock( queueMutex );
    theQueue.push( value );
    notEmptyCondition.notify_one();
}


template <typename T>
T &
WorkQueue<T>::remove()
{
    std::lock_guard<std::mutex> lock( queueMutex );

    while( queue.empty() )
    {
        notEmptyCondition.wait( lock );
    }

    T & item = theQueue.front();
    theQueue.pop();
    return item;
}
#pragma once


#include "Exceptions.h"


#include <queue>
#include <mutex>
#include <chrono>


namespace Utils
{
    // A thread safe queue implementation using mutexes.
    template <typename T>
    class WorkQueue
    {
    public:
        typedef typename std::queue<T>::size_type size_type;

        bool empty() const
        {
            std::lock_guard<std::mutex> lock( queueMutex );
            return theQueue.empty();
        }

        size_type size() const
        {
            std::lock_guard<std::mutex> lock( queueMutex );
            return theQueue.size();
        }

        void enqueue( const T & item )
        {
            std::lock_guard<std::mutex> lock( queueMutex );
            theQueue.push( item );
            notEmptyCondition.notify_one();
        }

        void enqueue( T && item )
        {
            std::lock_guard<std::mutex> lock( queueMutex );
            theQueue.push( std::move( item ) );
            notEmptyCondition.notify_one();
        }

        T & dequeue( const std::chrono::duration & maxWaitTime )
        {
            std::unique_lock<std::mutex> lock( queueMutex );

            while( theQueue.empty() )
            {
                bool finishedWaiting = notEmptyCondition.wait_for( lock, maxWaiTime );
                if( !finishedWaiting )
                    throw TimeoutException;
            }

            T & item = theQueue.front();
            theQueue.pop();
            return item;
        }

    private:
        std::queue<T> theQueue;
        std::mutex queueMutex;
        std::condition_variable notEmptyCondition;
    };
}

#pragma once


#include "Exceptions.h"


#include <queue>
#include <mutex>
#include <chrono>
#include <type_traits>


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

		T dequeue( int timeoutMilliseconds )  // throws Utils::TimoutException
		{
			std::chrono::milliseconds timeout( timeoutMilliseconds );
			std::unique_lock<std::mutex> lock( queueMutex );

			while( theQueue.empty() )
			{
				// Check for timout.
				std::cv_status status = notEmptyCondition.wait_for( lock, timeout );
				if( status == std::cv_status::timeout )
					throw TimeoutException();
			}

			// We std::move so we are compatible with move-only objects like std::unique_ptr.
			T item = std::move( theQueue.front() );

			theQueue.pop();
			return item;
		}

	private:
		std::queue<T> theQueue;
		std::mutex queueMutex;
		std::condition_variable notEmptyCondition;
	};
}

#pragma once


#include "TCP.h"
#include "TCPStream.h"


#include <string>
#include <memory>


namespace TCP
{
	class Acceptor
	{
	public:
		explicit Acceptor( const std::string & address, uint16_t port );
		virtual ~Acceptor();

		// Copy protection since destructor closes the listening socket.
		Acceptor( const Acceptor & other ) = delete;
		Acceptor & operator=( const Acceptor & other ) = delete;

		void start();
		void close();
		std::unique_ptr<Stream> accept();

	private:
		SOCKET listenSocket;
		std::string address;
		uint16_t port;
		bool listening;
	};
}
#pragma once


#include "TCP.h"


#include <string>


namespace TCP
{
	class Stream
	{
		friend class Acceptor;
		friend class Connector;

	public:
		virtual ~Stream();

		void write( const char *buffer, size_t len );
		size_t read( char *buffer, size_t len );
		void close();

		void setTCPNoDelay( bool opt );

		std::string getPeerIP() const;
		uint16_t getPeerPort() const;

	private:
		explicit Stream( SOCKET socket, const std::string & peerIP, uint16_t peerPort );

		SOCKET socket;
		std::string peerIP;
		uint16_t peerPort;
	};
}


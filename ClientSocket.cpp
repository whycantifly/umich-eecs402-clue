//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    ClientSocket member function definitions

// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"


ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
  {
    throw SocketException ( "Could not create client socket." );
  }

  if ( ! Socket::connect ( host, port ) )
  {
    throw SocketException ( "Could not bind to port." );
  }
}


const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  if (!Socket::send(s, true) )
  {
    throw SocketException ( "Could not write to socket." );
  }

  return *this;
}


const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
  {
    throw SocketException ( "Could not read from socket." );
  }

  return *this;
}

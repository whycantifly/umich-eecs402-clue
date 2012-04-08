// Implementation of the ServerSocket class
#include <iostream>
#include <errno.h>
using namespace std;

#include "ServerSocket.h"
#include "SocketException.h"

ServerSocket::ServerSocket(
     const int port
     )
{
  this->create(SOCK_STREAM, port);
}

ServerSocket::ServerSocket(
     const int &type,
     const int port
     )
{
  this->create(type, port);
}

ServerSocket::~ServerSocket(
     )
{
}

void ServerSocket::create(
     const int &type,
     const int &port     
     )
{
  if (!Socket::create(type))
  {
    throw SocketException("Could not create server socket.");
  }

  if (!Socket::bind(port))
  {
    throw SocketException("Could not bind to port.");
  }

  if (type == 1)
  {
    if (!Socket::listen())
    {
      throw SocketException("Could not listen to socket.");
    }
  }
}

const ServerSocket& ServerSocket::operator<<(
     const std::string& s
     ) const
{
  if (!Socket::send(s, false))
  {
    throw SocketException("Could not write int to socket.");
  }

  return *this;
}

const ServerSocket& ServerSocket::operator>>(
     std::string& s
     ) const
{
  if (!Socket::recv(s))
  {
    throw SocketException("Could not read from socket.");
  }

  return *this;
}

void ServerSocket::accept(
     ServerSocket& sock
     )
{
  int retVal;

  do 
  {
    retVal = Socket::accept(sock);
      
    if (!retVal && errno != EINTR)
    {
      throw SocketException("Could not accept socket.");
    }

  } while (!retVal);
}


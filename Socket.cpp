//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    Socket member function definitions

// Implementation of the Socket class.

#include "socketConstants.h"
#include "Socket.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <errno.h>

#include <iostream>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <cstdlib>
using namespace std;

//bool Socket::swapBytesForNetworkOrder = (12345 != htonl(12345));

Socket::Socket(
     ):m_sock(-1)
{
  memset(&m_addr, 0, sizeof(m_addr));
}

Socket::~Socket(
     )
{
  if (is_valid())
  {
    ::close(m_sock);
  }
}

void Socket::close(
     )
{
  if (is_valid())
  {
    ::close(m_sock);
    m_sock = -1;
  }
}

bool Socket::create(
     const int &inType
     )
{
  int on = 1;
  type = inType;

  m_sock = socket(AF_INET, type, 0);

  if (!is_valid())
  {
    return false;
  }

  // TIME_WAIT - argh
  if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR,
                 (const char*) &on, sizeof(on)) == -1)
  {
    return false;
  }

  return true;
}

bool Socket::bind(
     const int port
     )
{
  if (!is_valid())
  {
    return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons(port);

  int bind_return = ::bind(m_sock,
			   (struct sockaddr*) &m_addr,
			   sizeof(m_addr));

  if (bind_return == -1)
  {
    return false;
  }

  return true;
}


bool Socket::listen(
     ) const
{
  if (!is_valid())
  {
    return false;
  }

  int listen_return = ::listen(m_sock, MAXCONNECTIONS);

  if (listen_return == -1)
  {
    return false;
  }

  return true;
}

bool Socket::accept(
     Socket& new_socket
     ) const
{
  int addr_length = sizeof(m_addr);
  new_socket.m_sock = ::accept(m_sock, (sockaddr*)&m_addr,
                                 (socklen_t*)&addr_length);

  if (new_socket.m_sock <= 0)
  {
    cout << "ERROR: Failed to accept socket: " << new_socket.m_sock << endl;

    return false;
  }
  else
  {
    return true;
  }
}

bool Socket::send(
     const std::string s,
     const bool sendNull
     ) const
{
  int status;

  if (sendNull)
  {
    status = ::send(m_sock, s.c_str(), s.size() + 1, 0);
  }
  else
  {
    status = ::send(m_sock, s.c_str(), s.size(), 0);
  }

  if (status == -1)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool Socket::send(
     char *data,
     int numBytes
     ) const
{
  int status;
  int numBytesLeft;
  int numBytesSent;

  numBytesLeft = numBytes;
  numBytesSent = 0;
  
  while (numBytesLeft > 0 && status != -1)
  {
    if (numBytesLeft > SUBMSG_SIZE)
    {
      status = ::send(m_sock, data + numBytesSent, SUBMSG_SIZE, 0);
      numBytesLeft -= SUBMSG_SIZE;
      numBytesSent += SUBMSG_SIZE;
    }
    else
    {
      status = ::send(m_sock, data + numBytesSent, numBytesLeft, 0);
      numBytesLeft = 0;
      numBytesSent = numBytes;
    }
  }

  if (status == -1)
  {
    return false;
  }
  else
  {
    return true;
  }
}

int Socket::recv(
     std::string& s
     ) const
{
  char buf[MAXRECV + 1];

  s = "";

  memset(buf, 0, MAXRECV + 1);

  int status = ::recv(m_sock, buf, MAXRECV, 0);

  if (status == -1)
  {
    std::cout << "status == -1 in Socket::recv\n";
    return 0;
  }
  else if (status == 0)
  {
    return 0;
  }
  else
  {
    s = buf;
    return status;
  }
}

int Socket::recv(
     char ** data,
     int numBytes
     ) const
{
  int status;
  char *buf;
  int numBytesLeft;
  int numBytesRecvd;

  buf = (char*)malloc(numBytes);
 
  memset(buf, 0, numBytes);

  numBytesLeft = numBytes;
  numBytesRecvd = 0;
  while (numBytesLeft > 0 && status != -1)
  {
    //cout << "NUM RECVD: " << numBytesRecvd <<
    //        " NUM LEFT: " << numBytesLeft << endl;

    if (numBytesLeft > SUBMSG_SIZE)
    {
      status = ::recv(m_sock, buf + numBytesRecvd, SUBMSG_SIZE, 0);
      //if (status != SUBMSG_SIZE)
      //{
      //  cout << "UHOH - DID NOT RECECIVE EXPECTED AMT OF DATA" << endl;
      //}
      numBytesLeft -= status;
      numBytesRecvd += status;
    }
    else
    {
      status = ::recv(m_sock, buf + numBytesRecvd, numBytesLeft, 0);
      //if (status != numBytesLeft)
      //{
      //  cout << "UHOH - DID NOT RECECIVE EXPECTED AMT OF DATA" << endl;
      //}
      numBytesLeft -= status;
      numBytesRecvd += status;
    }
    //cout << "NUM RECVD: " << numBytesRecvd <<
    //        " NUM LEFT: " << numBytesLeft << endl;
  }

  if (status == -1)
  {
    std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
    return 0;
  }
  else if (status == 0)
  {
    return 0;
  }
  else
  {
    *data = buf;
    return status;
  }
}

bool Socket::connect(
     const std::string host,
     const int port
     )
{
  if (!is_valid())
  {
    return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons(port);

  int status = inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);

  if (errno == EAFNOSUPPORT)
  {
    return false;
  }

  status = ::connect(m_sock, (sockaddr*)&m_addr, sizeof(m_addr));

  if (status == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}



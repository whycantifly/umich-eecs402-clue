//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    Socket class definition

// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>


#include <iostream>
using namespace std;

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 1000;


class Socket
{
  public:
    Socket();
    virtual ~Socket();

    static bool swapBytesForNetworkOrder;

    // Server initialization
    bool create()
    {
      return this->create(1);
    }

    bool create(
         const int &type
         );

    bool bind(const int port);
    bool listen() const;
    bool accept(Socket&) const;

    // Client initialization
    bool connect(const std::string host, const int port);

    // Data Transimission
    bool send(const std::string, bool) const;
    int recv(std::string&) const;

    //New transmission
    template< class T >
    bool send( T val ) const
    {
      //cout << "Sending VAL: " << val << ", (bytes=" << sizeof(val) << ")"
      //     << endl;
      
      if (swapBytesForNetworkOrder)
      {
        this->swapBytes((char*) &val, sizeof(val));
      }

      return this->send( (char*) &val, sizeof(val));
    }
    
    void swapBytes(
         char* data,
         unsigned long size
         ) const
    {
      unsigned char tmpByte;
      unsigned long i;
      unsigned long j;
      unsigned long sizeOver2 = size / 2;

      for( i = 0; i < sizeOver2; i++ )
      {
        j = (size - 1) - i;

        tmpByte = data[i];
        data[i] = data[j];
        data[j] = tmpByte;
      }
    }

    bool send(char *, int) const;

    int recv(char **, int) const;

    bool is_valid() const { return m_sock != -1; }

    void close(
         );

  private:

    int m_sock;
    sockaddr_in m_addr;
    int type;
};

#endif

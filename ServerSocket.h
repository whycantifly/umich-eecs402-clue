//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    ServerSocket class definition

// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include <string>
using namespace std;

#include "Socket.h"


class ServerSocket : public Socket
{
  public:

    ServerSocket ( const int port );
    ServerSocket ( const int &type, const int port );
    ServerSocket (){};
    virtual ~ServerSocket();

    void create(
         const int &type,
         const int &port
         );

    void accept(ServerSocket&);

    const ServerSocket& operator<<(const std::string&) const;
    const ServerSocket& operator>>(std::string&) const;

};


#endif

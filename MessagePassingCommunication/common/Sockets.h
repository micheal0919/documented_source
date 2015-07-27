#ifndef SOCKETS_H
#define SOCKETS_H
/////////////////////////////////////////////////////////////////////
// Sockets.h   -  Provides basic network communication services    //
// ver 1.4                                                         //
// Language:      Visual C++, 2005                                 //
// Platform:      Dell Dimension 9150, Windows XP Pro, SP 2.0      //
// Application:   Utility for CSE687 and CSE775 projects           //
// Author:        Jim Fawcett, Syracuse University, CST 2-187      //
//                (315) 443-3948, jfawcett@twcny.rr.com            //
/////////////////////////////////////////////////////////////////////
/*
   Module Operations:
   ==================
   This module provides network communication services, using 
   WinSock2, a nearly Berkley Sockets compliant implementation
   with extensions.  Three classes are provided:

   SocketSystem:
   -------------
   provides WinSock loading, unloading and a few program wide services.
   A recent change has ensured that the WinSock library is only loaded
   once, no matter how many times you construct SocketSystem objects.
   So now, the Socket class has a SocketSystem instance so you don't
   have to do an explicit creation of a SocketSystem object before
   creating a Socket.

   Socket:
   -------
   Provides connect request, string read, and string write service.
   
   SocketListener:
   ---------------
   Provides connection handling.
   
   Public Interface:
   =================
   SocketSystem sys;
   SocketListener listener(2048);
   Socket recvr = listener.waitForConnect();
   Socket sendr;
   sender.connect("\\localhost",2048);
   sender.send("this is a message\n");
   sender.send("another message\n");
   sender.send("quit\n");
   Socket recvr;
   while(true)
   {
     std::string msg = Socket::stripTerminator(recvr.recv());
     std::cout << "\n  received \"" << msg;
     if(msg == "quit")
       break;
   }
   recvr.disconnect();
   sender.disconnect();
*/
//
/*
   Build Process:
   ==============
   Required Files:
     Sockets.h, Sockets.cpp

   Compile Command:
   ================
   cl /EHa Sockets.cpp WSock32.lib

   Maintenance History:
   ====================
   ver 1.4 : 26 Apr 06
   - added more tracing and more exception handling
   - included facilities from diagnostics module and moved tracing
     to diagnostics
   ver 1.3 : 17 Apr 06
   - added Socket::disconnect()
   - made some cosmetic changes
   ver 1.2 : 11 Apr 06
   - removed the static qualifiers for the SocketSystem class
     as they cannot be used successfully without creating a
     SocketSystem object, which loads the wsock library.
   - that required adding static bool loaded and static int count.
   ver 1.1 : 09 Apr 06
   - added to SocketSystem:
     getIpFromName(...), getNameFromIp(...), getHostName()
   - added to Sockets:
     getRemoteIP(), getRemotePort(), getLocalIP(), getLocalPort()
   - added test drivers for SocketClient and SocketServer
   ver 1.0 : 27 Mar 06
   - first release

   To Do:
   ======
   Add SocketSystem::Terminators("\n", "quit");
   Add SocketServer class, in its own module, since it will require threads.
   
*/
//
#include <string>
#include <winsock2.h>
#include <sstream>

/////////////////////////////////////////////////////////////////////
// SocketSystem class loads and unloads WinSock library
// and provides a few system services

class SocketSystem
{
public:
  SocketSystem();
  ~SocketSystem();
  std::string getHostName();
  std::string getNameFromIp(const std::string& ip);
  std::string getIpFromName(const std::string& name);
  void throwMsg(const char* msg, const char *file, int line);
  std::string GetLastMsg(bool WantSocketMsg=true);
private:
  static bool loaded;
  static int count;
};

/////////////////////////////////////////////////////////////////////
// Socket class provides basic connect, read, and write operations

class Socket
{
public:
  Socket();
  Socket(const std::string& url, int port);
  Socket(SOCKET s);
  ~Socket();
  operator SOCKET ();
  bool connect(std::string url, int port);
  void disconnect();
  bool error() { return (s_ == SOCKET_ERROR); }
  bool send(const std::string& msg);
  bool send(const char* block, size_t len);
  std::string recv();
  void recv(char* block, size_t len);
  std::string getRemoteIP();
  int getRemotePort();
  std::string getLocalIP();
  int getLocalPort();
  void setTerminator(const std::string& terminal);
  std::string getTerminator();
  std::string stripTerminator(const std::string& msg);
private:
  SOCKET s_;
  SocketSystem ss_;
  std::string terminator_;
};
//
/////////////////////////////////////////////////////////////////////
// SocketListener class waits for connections, then delivers
// connected socket

class SocketListener
{
public:
  SocketListener(int port);
  ~SocketListener();
  Socket waitForConnect();
  void stop();
private:
  SOCKADDR_IN tcpAddr;
  Socket s_;
};

template <typename T>
std::string ToString(const T& t)
{
  std::ostringstream out;
  out << t;
  return out.str();
}

#endif

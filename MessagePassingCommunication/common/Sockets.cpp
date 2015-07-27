/////////////////////////////////////////////////////////////////////
// Sockets.cpp - Provides basic network communication services     //
// ver 1.3                                                         //
// Language:      Visual C++, 2005                                 //
// Platform:      Dell Dimension 9150, Windows XP Pro, SP 2.0      //
// Application:   Utility for CSE687 and CSE775 projects           //
// Author:        Jim Fawcett, Syracuse University, CST 2-187      //
//                (315) 443-3948, jfawcett@twcny.rr.com            //
/////////////////////////////////////////////////////////////////////

#include <sstream>
#include "Sockets.h"
#include "diagnostics.h"
#include <iostream>

bool SocketSystem::loaded = false;
int SocketSystem::count = 0;

//----< get socket error message string >----------------------------

std::string SocketSystem::GetLastMsg(bool WantSocketMsg) {

// ask system what type of error occurred

  DWORD errorCode;
  if(WantSocketMsg)
    errorCode = WSAGetLastError();
  else
    errorCode = GetLastError();
  if(errorCode == 0)
    return "no error";

// map errorCode into a system defined error string
    
  DWORD dwFlags = 
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER;
  LPCVOID lpSource = NULL;
  DWORD dwMessageID = errorCode;
  DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
  LPSTR lpBuffer;
  DWORD nSize = 0;
  va_list *Arguments = NULL;

  FormatMessage(
    dwFlags,lpSource,dwMessageID,dwLanguageId, 
    (LPTSTR)&lpBuffer,nSize,Arguments
  );

  std::string _msg(lpBuffer);
  LocalFree(lpBuffer);
  return _msg;
}
//----< throw exception string >-------------------------------------

void SocketSystem::throwMsg(const char *msg, const char *file, int line) {

  std::ostringstream collect;
  collect << msg << " " << GetLastMsg();
  if(file)
    collect << "\nfile: " << file;
  if(line)
    collect << "\nline: " << line;
  collect << '\n' << std::ends;
  throw std::exception(collect.str().c_str());
}
//
//----< load WinSock Library >---------------------------------------

SocketSystem::SocketSystem()
{
  ++count;
  if(loaded == false)
  {
    Trace("loading wsock32 library");
    WORD wVersionRequested = MAKEWORD(1,1); // requesting version 1.1
    WSAData wData;                          // startup data filled by WSAStartup
    int err = WSAStartup(wVersionRequested, &wData);
    if(err == SOCKET_ERROR)
      SocketSystem::throwMsg("initialization error: ",__FILE__,__LINE__);
    loaded = true;
  }
}
//----< destructor unloads socket library >--------------------------

SocketSystem::~SocketSystem()
{ 
  if(--count == 0)
  {
    Trace("unloading wsock32 library");
    WSACleanup();
    loaded = false;
  }
}
//
//----< get host name >----------------------------------------------

std::string SocketSystem::getHostName()
{
  char buffer[256];
  gethostname(buffer,256);
  return buffer;
}
//----< get ip address of network machine >--------------------------

std::string SocketSystem::getIpFromName(const std::string& name)
{
  hostent* remoteHost = 0;
  SOCKADDR_IN tcpAddr;
  tcpAddr.sin_family = AF_INET;
  tcpAddr.sin_addr.s_addr = inet_addr(name.c_str());
  if(tcpAddr.sin_addr.s_addr == INADDR_NONE)
  {
    // name is not an ip address, so try to map name to address
    // via DNS

    hostent* remoteHost = gethostbyname(name.c_str());
    if(remoteHost == NULL)
      SocketSystem::throwMsg("invalid name",__FILE__,__LINE__);
    CopyMemory(
      &tcpAddr.sin_addr, 
      remoteHost->h_addr_list[0], 
      remoteHost->h_length
    );
  }
  return inet_ntoa(tcpAddr.sin_addr);
}
//----< get network name of machine from ip address >----------------

std::string SocketSystem::getNameFromIp(const std::string& ip)
{
  struct hostent* host = (hostent*)malloc(sizeof(struct hostent));
  struct in_addr *ipaddr = (in_addr*)malloc(sizeof(struct in_addr));
  ipaddr->s_addr = inet_addr(ip.c_str());
  host = gethostbyaddr((char*)ipaddr, sizeof(ipaddr), AF_INET);
  if(!host)
    SocketSystem::throwMsg("name resolution error: ",__FILE__,__LINE__);    
  return host->h_name;
}
//
//----< constructor creates TCP Stream socket >----------------------

Socket::Socket() : terminator_("</msg>")
{
  s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
}
//----< constructor creates connected TCP Stream socket >------------

Socket::Socket(const std::string& url, int port) : terminator_("\n")
{
  s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  this->connect(url, port);
}
//----< promotes WinSock SOCKET handle to Socket object >------------

Socket::Socket(SOCKET s) : s_(s), terminator_("</msg>") {}

//----< destructor closes socket handle >----------------------------

Socket::~Socket()
{ 
  disconnect();
}
//----< connects to IP address or network host >---------------------

bool Socket::connect(std::string url, int port)
{
  int err;
  try
  {
    if(isalpha(url[0]))
      url = SocketSystem().getIpFromName(url);
    SOCKADDR_IN tcpAddr;
    tcpAddr.sin_family = AF_INET;
    tcpAddr.sin_addr.s_addr = inet_addr(url.c_str());
    tcpAddr.sin_port = htons(port);
    err = ::connect(s_, (sockaddr*)&tcpAddr, sizeof(tcpAddr));
  }
  catch(std::exception& ex)
  {
    std::string msg 
      = std::string(ex.what()) + std::string(", Error in Socket::connect(...)");
    diagnostics().ExceptionMsg(msg.c_str(),"Socket Error");
  }
  if(err == SOCKET_ERROR)
    return false;
  return true;
}
//----< disconnect socket >------------------------------------------

void Socket::disconnect()
{
  shutdown(s_, SD_BOTH); 
  closesocket(s_); 
}
//
//----< casts Socket to WinSock SOCKET handle >----------------------

Socket::operator SOCKET ()
{ 
  return s_; 
}
//----< send blocks until all characters are sent >------------------

bool Socket::send(const std::string& msg)
{
  return send(msg.c_str(),msg.size());
}
//----< send blocks until all characters are sent >------------------

bool Socket::send(const char* block, size_t len)
{
  try
  {
    const int sendRetries = 100;
    size_t blockLen = strlen(block);
    size_t bytesSent;
    size_t bytesLeft = blockLen;
    size_t blockIndx = 0;
    size_t count = 0;
    while(bytesLeft > 0) {
      bytesSent = ::send(s_,&block[blockIndx],static_cast<int>(bytesLeft),0);
      if(bytesSent == SOCKET_ERROR) {
        Sleep(50);
        ++count;
      }
      if(count==sendRetries)
        throw std::exception("send failed after 100 retries");

      bytesLeft -= bytesSent;
      blockIndx += bytesSent;
    }
  }
  catch(std::exception& ex)
  {
    std::string msg = std::string(ex.what()) + std::string(", Error in Socket::send(...)");
    diagnostics().ExceptionMsg(msg.c_str(),"Socket Error");
    return false;
  }
  return true;
}
//----< set socket message terminator >------------------------------

void Socket::setTerminator(const std::string& terminal) 
{ 
  terminator_ = terminal; 
}
//----< get socket message terminator >------------------------------

std::string Socket::getTerminator()
{ 
  return terminator_;
}
//----< strip terminator from end of message >-----------------------

std::string Socket::stripTerminator(const std::string& msg)
{
  return msg.substr(0,msg.size() - getTerminator().size());
}
//
//----< recv blocks until a terminator is received >-----------------

std::string Socket::recv()
{
  std::string temp;
  size_t len = terminator_.size();
  char ch = 0;
  while(true)
  {
    recv(&ch, 1);
    temp += ch;
    size_t i=temp.size(), j=terminator_.size();
    while(--i>=0 && --j>=0)
    {
      if(temp[i] != terminator_[j])
        break;
      if(j==0)
        return temp;
    }
  }
  return "message framing error";
}
//----< blocks until len characters have been sent >-----------------

void Socket::recv(char* block, size_t len)
{
  try
  {
    const size_t recvRetries = 100;
    size_t bytesRecvd, bytesLeft = len;
    size_t blockIndx = 0, count = 0;
    while(bytesLeft > 0) {
      bytesRecvd = ::recv(s_,&block[blockIndx],static_cast<int>(bytesLeft),0);
      if(bytesRecvd == SOCKET_ERROR) {
        Sleep(50);
        ++count;
      }
      if(count==recvRetries)
        SocketSystem().throwMsg("recv failed after 100 retries",__FILE__,__LINE__);

      bytesLeft -= bytesRecvd;
      blockIndx += bytesRecvd;
    }
  }
  catch(std::exception& ex)
  {
    std::string msg = std::string(ex.what()) 
                    + std::string(", Error in Socket::recv(...)");
    diagnostics().ExceptionMsg(msg.c_str(),"Socket Error");
  }
}
//
//----< get local ip address >---------------------------------------

std::string Socket::getLocalIP()
{
  struct sockaddr name;
  int len = sizeof(name);
  hostent* local = gethostbyname("");
  return inet_ntoa(*(struct in_addr*)*local->h_addr_list);
  //getsockname(s_, &name, &len);
  //std::string str(name.sa_data);
  //return str;
}
//----< get local port >---------------------------------------------

int Socket::getLocalPort()
{
  struct sockaddr name;
  int len = sizeof(name);
  int status = getsockname(s_,&name,&len);
  if(status == 0)
  {
    struct sockaddr_in* pLocal = reinterpret_cast<sockaddr_in*>(&name);
    return htons(pLocal->sin_port);
  }
  return -1;
}
//----< get remote ip address >--------------------------------------

std::string Socket::getRemoteIP()
{
  struct sockaddr name;
  int len = sizeof(name);
  int status = getpeername(s_,&name,&len);
  if(status == 0)
  {
    struct sockaddr_in* pRemote = reinterpret_cast<sockaddr_in*>(&name);
    return inet_ntoa(pRemote->sin_addr);
  }
  return "";
}
//----< get remote port >--------------------------------------------

int Socket::getRemotePort()
{
  struct sockaddr name;
  int len = sizeof(name);
  int status = getpeername(s_,&name,&len);
  if(status == 0)
  {
    struct sockaddr_in* pRemote = reinterpret_cast<sockaddr_in*>(&name);
    return htons(pRemote->sin_port);
  }
  return -1;
}
//
//----< starts listener socket listening for connections >-----------

SocketListener::SocketListener(int port)
{
  try
  {
  tcpAddr.sin_family = AF_INET;   // TCP/IP
  tcpAddr.sin_port = htons(port); // listening port
  tcpAddr.sin_addr.s_addr = INADDR_ANY;
                                  // listen over every network interface
  int err = bind(s_, (SOCKADDR*)&tcpAddr, sizeof(tcpAddr));

  if(err == SOCKET_ERROR)
  {
    throw std::exception("binding error type:");
  }

  /////////////////////////////////////////////////////////////////
  // listen for incoming connection requests

  int backLog = 10;
  err = listen(s_, backLog);

  if(err == SOCKET_ERROR)
    throw std::exception("listen error type:");
  }
  catch(std::exception& ex)
  {
    std::string msg = std::string(ex.what()) 
                    + ", error in SocketListener construction";
    diagnostics().ExceptionMsg(msg,"Socket Error");
  }
}
//----< destructor closes socket >-----------------------------------

SocketListener::~SocketListener()
{
  std::cout << "\n  destroying SocketListener";
  shutdown(s_, SD_BOTH);
  closesocket(s_);
}
//----< blocks until a connection request has been received >--------

Socket SocketListener::waitForConnect()
{
  int size = sizeof(tcpAddr);
  SOCKET toClient = accept(s_, (SOCKADDR*)&tcpAddr, &size);      

  if(toClient == INVALID_SOCKET)
    return Socket(INVALID_SOCKET);
  return Socket(toClient);
}
//----< shuts down listerner >---------------------------------------

void SocketListener::stop()
{
  Trace("shutting down listener in SocketListerer");
  shutdown(s_,SD_BOTH);
  closesocket(s_);
}
//
//----< test stub >--------------------------------------------------

#ifdef TEST_SOCKETS
#include <iostream>

void main()
{
  std::cout << "\n  Testing Socket, SocketListener, and SocketSystem classes";
  std::cout << "\n ==========================================================\n";

  try
  {
    SocketSystem su;
    std::string host = su.getHostName();
    std::cout << "\n  host machine name:           " << host.c_str();
    std::string ip = su.getIpFromName(host);
    std::cout << "\n  IP Address of machine:       " << ip.c_str();
    std::string name = su.getNameFromIp(ip);
    std::cout << "\n  DNS name of machine from ip: " << name.c_str() << '\n';
  
    std::cout << "\n  connecting to listener";
    std::cout << "\n ------------------------";

    SocketListener listener(2048);
    Socket sendr;
    if(!sendr.connect(name,2048))  // can use ip addr, e.g., 127.0.0.1
      su.throwMsg("",__FILE__,__LINE__);
    
    Socket recvr = listener.waitForConnect();
    std::cout << "\n  remote ip is: " << recvr.getRemoteIP();
    std::cout << ", remote port is: " << recvr.getRemotePort();
    std::cout << "\n  local ip is:  " << recvr.getLocalIP();
    std::cout << ", local port is: " << recvr.getLocalPort() << '\n';
    recvr.setTerminator("</msg>");

    std::cout << "\n  sending from connector to listener";
    std::cout << "\n ------------------------------------";

    sendr.send("<msg>this is a message</msg><msg>and another message</msg>");
    sendr.send("<msg>and a final message</msg>");
    sendr.send("<msg>quit</msg>");
    while(true)
    {
      std::string msg = recvr.recv();
      std::cout << "\n  received \"" << msg;
      if(msg == "<msg>quit</msg>")
        break;
    }
    std::cout << "\n";
//
    // demonstrating full duplex operation

    std::cout << "\n  sending from listener back to connector";
    std::cout << "\n -----------------------------------------";

    sendr.setTerminator("\n");
    recvr.send("sending message back\nquit\n");
    while(true)
    {
      std::string msg = sendr.stripTerminator(sendr.recv());
      std::cout << "\n  received \"" << msg;
      if(msg == "quit")
        break;
    }
    sendr.disconnect();
    recvr.disconnect();
    std::cout << "\n\n";
  }
  catch(std::exception& e)
  {
    std::cout << "\n  " << e.what() << "\n\n";
  }
}

#endif

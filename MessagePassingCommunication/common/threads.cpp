/////////////////////////////////////////////////////////////////////
//  threads.cpp   - class supporting creation of threads           //
//  ver 5.3                                                        //
//  Language:       Visual C++, ver 7.1, SP 2                      //
//  Platform:       Dell Dimension 8300, Win XP, SP2               //
//  Application:    CSE687 - Object Oriented Design                //
//  Author:         Jim Fawcett, CST 2-187, Syracuse Univ          //
//                  (315) 443-3948, jfawcett@twcny.rr.com          //
//                                                                 //
/////////////////////////////////////////////////////////////////////

#include "threads.h"
#include "locks.h"
#include <crtdbg.h>

//----< Win32 thread is created here >-------------------------------

void thread::start() {

  if(th)
    CloseHandle(th);    // in case we restart the thread

  _lck.lock();
  _alive = true;
  _lck.unlock();

  th = (HANDLE)_beginthreadex(
    NULL,             // default security attributes
    0,                // default stack size
    threadProc,       // thread processing function
    (void*)this,      // pointer to this thread object
    0,                // create thread in running state
    &_threadID         // thread identifier assigned by OS
  );
}

//----< destroy thread >---------------------------------------------

thread::~thread() { 
  
  _lck.lock();
  _alive = false;  // parent may call isAlive 
  _lck.unlock();

  CloseHandle(th); 
  th = 0;
#ifdef TEST_THREADS
  sout << "\n  thread object destroyed";
#endif
}

//----< is this thread still alive? >--------------------------------
//
//  thread becomes alive when start() is called and becomes
//  not alive when its run() function completes
//
bool thread::alive() { 
  _lck.lock();    // wait for modification to complete
  _lck.unlock();
  return _alive; 
}

//
//----< thread proc wrapper for derived class's run function >-------

unsigned int __stdcall thread::threadProc(void *pSelf) {

  (((thread*)pSelf)->_pProc)->run();
  
  ((thread*)pSelf)->_lck.lock();
  ((thread*)pSelf)->_alive = false;
  ((thread*)pSelf)->_lck.unlock();
  
  if(((thread*)pSelf)->_suicidal)
  {
    delete ((thread*)pSelf)->_pProc;
    delete (thread*)pSelf;
  }
  return 0;
}

//----< wait until thread terminates >-------------------------------

void thread::wait()
{ 
  if(_alive)
    WaitForSingleObject(th,INFINITE); 
}

//----< set thread priority >----------------------------------------

void thread::setPriority(thread::priority pr) {

  switch(pr) {
    case thread::low : 
      ::SetThreadPriority(th,THREAD_PRIORITY_LOWEST); 
      break;
    case thread::normal : 
      ::SetThreadPriority(th,THREAD_PRIORITY_NORMAL); 
      break;
    case thread::high : 
      ::SetThreadPriority(th,THREAD_PRIORITY_HIGHEST); 
      break;
    default : 
      ::SetThreadPriority(th,THREAD_PRIORITY_NORMAL);
  }
}
//----< increment suspend count >------------------------------------
//
//  puts thread to sleep if not already asleep
//
bool thread::suspend() { 
  
  if(_canSuspend) {
    ::SuspendThread(th);
  }
  return _canSuspend;
}
//
//----< add a thread to the pool >-----------------------------------

void threadPool::add(thread* pThread)
{ 
  // remove dead threads and their handles

  std::map<HANDLE,thread*>::iterator iter;
  for(iter=handles.begin(); iter!=handles.end(); ++iter)
    if(!(*iter).second->alive())
      handles.erase(iter);

  // now add the new thread and handle

  handles[pThread->handle()] = pThread;
}
//----< wait for all the threads in pool to terminate >--------------

void threadPool::waitAll()
{
  std::map<HANDLE,thread*>::iterator hiter;
  for(hiter=handles.begin(); hiter!=handles.end(); ++hiter)
    WaitForSingleObject((*hiter).first,INFINITE);
}
//

//----< test stub >--------------------------------------------------

#ifdef TEST_THREADS

#include "locks.h"
#include "blockingQueue.h"
#include <iostream>
#include <sstream>
#include <queue>
using namespace std;

///////////////////////////////////////////////////////////////
//  Note: 
//    The two structs, below, provide a static string and a
//    static int, respectively.  Types with static members have
//    to be defined at global level, but their instances can be
//    members of local classes, and that is how we have used
//    them for Demo #6, below, in main.
//
//    Their instances are memebers of sharedString1 and 
//    sharedString2, defined in that demo.  They are used as data
//    that will be locked, and shared between two threads.
//
///////////////////////////////////////////////////////////////

//----< struct with static string >----------------------------------

struct StaticString
{
  static std::string sharedStr;
};
std::string StaticString::sharedStr = "";

//----< struct with static int >-------------------------------------

struct StaticInt { static int count; };
int StaticInt::count = 0;

//
//----< helper string conversion function for Demo #6 >--------------

template <typename T>
std::string ToString(const T& t)
{
  std::ostringstream converter;
  converter << t;
  return converter.str();
}

/////////////////////////////////////////////////////////////////////
// cosmetic stuff

class linesAtEnd  // emit a couple of newlines when program quits
{
public:
  ~linesAtEnd()
  { 
    std::cout << "\n\n";
    std::cout.flush(); 
  }
} global;

/////////////////////////////////////////////////////////////////////
//
//  Note: 
//    You must create derived classes to define all your application's
//    processing that you want allocated to a thread.  Examples of how
//    to do that are given here and on the next pages.  Each class,
//    derived from the abstract base processing class, provides a run
//    function to supply application specific processing.
//
/////////////////////////////////////////////////////////////////////

//
//----< entry point >------------------------------------------------

void main() {

  cout << "\n  Testing Thread, BlockingQueue, and Lock classes"
       << "\n =================================================\n";
  cout.flush();

/////////////////
// Demo #1

  ///////////////////////////////////////////////////////////////
  //  non-queued processing for first demonstration

  class appProc1 : public processing {
  
  public:
    appProc1(int n=10) : numIter(n) {}
    ~appProc1() { sout << "\n  appProc1 destroyed"; }
    void run() 
    {
      for(int i=0; i<numIter; i++)
      {
        sout << locker << "\n  thread #" << thread::currentThread() 
             << ": " << _str.c_str() << unlocker;
        ::Sleep(20);   // relinquish control
      }
    }
    void setString(const string &s) { _str = s; }
  private:
    string _str;
    int numIter;
  };
  ///////////////////////////////////////////////////////////////
  //  non-queued processing for first demonstration

  class appProc2 : public processing
  {
  public:
    appProc2(int n=10) : numIter(n) {}
    ~appProc2() { sout << "\n  appProc2 destroyed"; }
    void run() 
    {
      for(int i=0; i<numIter; i++) {
        sout << locker << "\n  thread #" << thread::currentThread() 
             << ": " << _str.c_str() << unlocker;
        ::Sleep(1);   // relinquish control
      }
    }
    void setString(const string &s) { _str = s; }
  private:
    string _str;
    int numIter;
  };
//
  cout << "\n  Demo #1 - two concurrent child threads "
       << "\n ----------------------------------------\n";
  cout.flush();

  sout << locker
       << "\n   main thread id = " << thread::currentThread() << unlocker;
  
  appProc1 proc1;
  proc1.setString("a short message");
  thread t1(&proc1);
  sout << locker << "\n  child thread id = " << t1.id() << unlocker;

  appProc2 proc2;
  proc2.setString("a somewhat longer, and rather boring, message");
  thread t2(&proc2);
  sout << locker << "\n  child thread id = " << t2.id() << unlocker;

  t1.start();
  t2.start();

  if(t1.alive())
    sout << locker << "\n  thread t1 is alive" << '\n' << unlocker;
  else
    sout << locker << "\n  thread t1 has completed" << '\n' << unlocker;

  t1.wait();
  t2.wait();

  sout << "\n";
  if(t1.alive())
    sout << "\n  thread t1 is alive";
  else
    sout << "\n  thread t1 has completed";

  sout << "\n\n";

// end Demo #1

//
/////////////////
// Demo #2

  ///////////////////////////////////////////////////////////////
  //  queued processing for second demonstration

  class Qproc : public processing
  {
  public:
    Qproc(BQueue<std::string>& Q) : _Q(Q) {}
    void run() 
    {
      string temp;
      do {
        temp = _Q.deQ();
        sout << locker << "\n  child thread deQ'd: " << temp.c_str() << unlocker;
      } while(temp != "quit");
    }
  private:
    BQueue<std::string>& _Q;
  };

//
  cout << "\n  Demo #2 - thread using blocking Queue passed to proc "
        << "\n -----------------------------------------------------\n";

  // Create processing object, defined above,
  // then create and start thread.

  BQueue<std::string> Q;
  Qproc proc3(Q);
  thread t3(&proc3);
  t3.start();

  for(int i=0; i<10; i++) {

    // make a message string

    string temp("message #");
    temp += ToString(i+1);
    
    // Now enQ a message to be handled by the child thread.
    // Note that no synchronization is necessary.  That's 
    // handled by the interface enQ(...) and deQ() functions.

    Q.enQ(temp);
    sout << locker << "\n   main thread enQ'd: " << temp << unlocker;

    // Sleep used to slow down main thread so enQs and deQs interleave.
    ::Sleep(15);
  }
  string temp("quit");
  Q.enQ(temp);
  sout << locker << "\n   main thread enQ'd: " << temp << unlocker;
  t3.wait();
  sout << "\n\n";
  sout << "\f\n";

  sout << locker
       << "\n  restarting the thread with queue "
       << "\n ----------------------------------\n"
       << unlocker;

  t3.start();
  Q.enQ("started again");
  Q.enQ("now quitting");
  Q.enQ("quit");
  t3.wait();

  sout << "\n\n";

// end Demo #2
//
/////////////////
// Demo #3

  ///////////////////////////////////////////////////////////////
  //  spawn new thread in threadproc

  class InProc : public processing
  {
  public:
    ~InProc() { sout << "\n  InProc object destroyed"; }
    void run()
    {
      sout << "\n  inner thread processing in InProc::run()";
    }
  };

  class OutProc : public processing
  {
  public:
    ~OutProc() { sout << "\n  OutProc object destroyed"; }
    void run()
    {
      sout << "\n  outer thread processing in OutProc::run()";
      sout << "\n  creating child thread";
      InProc inproc;
      thread t(&inproc);
      t.start();
      t.wait();
    }
  };

  sout << "\n  Demo #3 - grandchild thread inside child thread proc "
       << "\n ------------------------------------------------------\n";

  OutProc outer;
  thread t4(&outer);
  t4.start();
  t4.wait();

  sout << "\n\n";

// end Demo #3
//
/////////////////
// Demo #4

  /////////////////////////////////////////////////////////////////////
  //  spawn new thread on heap in threadproc

  class OutProcOnHeap : public processing
  {
  public:
    ~OutProcOnHeap() { sout << "\n  OutProcOnHeap object destroyed"; }
    void run()
    {
      sout << "\n  creating inner thread and its processing object on C++ heap";

  ////////////////////////////////////////////////////////////////
  //    thread* pThread = new thread(new InProc);
  //    pThread->willSelfDestruct();
  //      are equivalent to:
  //    thread* pThread = CreateThreadOnHeap<InProc>();
  //      which is equivalent to:
    
      thread* pThread = CreateThreadOnHeap(new InProc);

      sout << "\n  outer thread processing in OutProcOnHeap::run()";
      pThread->start();
      pThread->wait();
    }
  };

  sout << locker
    << "\n  Demo #4 - running threads from the C++ heap"
       << "\n ------------------------------------------\n"
       << unlocker;

  sout << "\n  creating outer thread and its processing object on C++ heap";
  thread* pThread = CreateThreadOnHeap(new OutProcOnHeap);
  pThread->start();
  pThread->wait();

  sout << "\n\n";
/*
===========================================================================
NOTE:
  The processing below will fail WITH AN ASSERT that cannot be caugth as
  a std::exception or Win32 structured exception, unfortunately.

  OutProc op;
  pThread = CreateThreadOnHeap(&op);
  pThread->start();
  pThread->wait();

  when the processing in OutProc::run() terminates, the thread class instance
  will delete (thread*)pSelf, which deletes the thread - that's correct,
  then will delete ((thread*pSelf)->_pProc, which will ASSERT since _pProc
  points to op, created on the stack.  The solution is to create op on
  the C++ heap.
===========================================================================
*/
// end Demo #4
//
/////////////////
// Demo #5

  sout << locker
    << "\n  Demo #5 - loop creating threads on the C++ heap "
       << "\n ------------------------------------------------\n"
       << unlocker;

  threadPool thrdp;
  for(int i=0; i<10; ++i)
  {
    appProc1* pProc = new appProc1(1);
    pProc->setString("Hi from heap");
    pThread = new thread(pProc);
    pThread->willSelfDestruct();
    pThread->start();
    thrdp.add(pThread);
  }
  thrdp.waitAll();

  sout << "\n\f\n";

// end Demo #5
//
/////////////////
// Demo #6

  /////////////////////////////////////////////////////////////////////
  // Threads sharing a common static string, sharedString1::shared
  /////////////////////////////////////////////////////////////////////
  // - classes with statics can't be locally defined, so you will
  //   find them just above main

  cout << "\n  Demo #6 - Demonstrating string sharing between threads "
        << "\n -------------------------------------------------------\n";

  /////////////////////////////////////////////////////////////////////
  // Thread processing that shares a static string locked with GLock<1>

  class sharedString1 : public processing
  {
  public:
    sharedString1() : myCount(++numberOfObjects.count) {}
    void run()
    {
      const string blanks(41,' ');
      for(int i=0; i<10; ++i)
      {
        gl.lock();
        if(shared.sharedStr.length() > 10)
        {
          cout << "\n  " << blanks;
          shared.sharedStr = "";
        }
        shared.sharedStr += ToString(myCount);
        cout << "\n  " << shared.sharedStr;
        gl.unlock();
      }
    }
  private:
    StaticInt numberOfObjects;
    int myCount;
    StaticString shared;
    GLock<1> gl;
  };
//
/////////////////////////////////////////////////////////////////////
// Thread processing that shares a static string using lockingPtr

class sharedString2 : public processing
{
public:
  sharedString2() : myCount(++numberOfObjects.count) {}
  void addString(const std::string& str)
  {
    shared.sharedStr += str;
  }
  void clearString()
  {
    shared.sharedStr = "";
  }
  void run()
  {
    const string blanks(41,' ');
    for(int i=0; i<10; ++i)
    {
      if(shared.sharedStr.length() > 10)
      {
        sout << locker << "\n  " << blanks << unlocker;
        lockingPtr< sharedString2, GLock<1> >(*this,gl)->clearString();
      }
      lockingPtr< sharedString2, GLock<1> >(*this,gl)
        ->addString(ToString(myCount));
      sout << locker << "\n  " << shared.sharedStr << unlocker;
    }
  }
private:
  StaticInt numberOfObjects;
  int myCount;
  StaticString shared;
  GLock<1> gl;
};
//
  // Create processing object, defined above,
  // then create and start thread.

  std::cout << "\n  using Glock<1> for synchronization";
  StaticString::sharedStr = "";
  sharedString1 ss1, ss2;
  thread t5(&ss1);
  thread t6(&ss2);
  t5.start();
  t6.start();
  t5.wait();
  t6.wait();
  std::cout << "\n\n";

  std::cout << "\n  using lockingPtr for synchronization";
  sharedString2 ss3, ss4;
  StaticString::sharedStr = "";
  thread t7(&ss3);
  thread t8(&ss4);
  t7.start();
  t8.start();
  t7.wait();
  t8.wait();
  std::cout << "\n\n";

// end Demo #6
//
/////////////////
// Demo #7

  /////////////////////////////////////////////////////////////////////
  // provide parameters to processing object on heap before starting

  cout << "\n  Demo #7 - Set paramter in processing object on C++ heap"
        << "\n ---------------------------------------------------------\n";

  class giveParam : public processing
  {
  public:
    giveParam(const std::string& s="") { str = s; }
    ~giveParam() { sout << "\n  destroying giveParam"; }
    void setString(const std::string& s) { str = s; }
    void run()
    {
      sout << locker << "\n  giveParam speaking from the C++ heap: " 
           << str << unlocker;
    }
  private:
    std::string str;
  };

  pThread = CreateThreadOnHeap<giveParam>();
  giveParam* pGp = dynamic_cast<giveParam*>(pThread->proc());
  if(pGp)
    pGp->setString("hi from heap");
  else
    sout << "\n  parameter setting failed";
  pThread->start();
  pThread->wait();
  std::cout << "\n";

  // Here is a simpler way to provide the parameter:

  pThread = CreateThreadOnHeap(new giveParam("hi again from C++ heap"));
  pThread->start();
  pThread->wait();
  std::cout << "\n";

// end Demo #7
//
  ///////////////////////////////////////////////////////////////////
  // cosmetic stuff

  class lines  // emit a couple of newlines when program quits
  {
  public:
    ~lines()
    { 
      std::cout << "\n  Cleaning Up";
      std::cout << "\n -------------";
      std::cout.flush();
    }
  } local;
}

#endif

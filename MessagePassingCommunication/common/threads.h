#ifndef THREADS_H
#define THREADS_H
/////////////////////////////////////////////////////////////////////
//  threads.h     - class supporting creation of threads           //
//  ver 5.3                                                        //
//  Language:       Visual C++, ver 7.1, SP 2                      //
//  Platform:       Dell Dimension 8300, Win XP, SP2               //
//  Application:    CSE687 - Object Oriented Design                //
//  Author:         Jim Fawcett, CST 2-187, Syracuse Univ          //
//                  (315) 443-3948, jfawcett@twcny.rr.com          //
/////////////////////////////////////////////////////////////////////
/*
    Module Operations
    =================
    The class thread encapsulates creation of Win32 threads.  To
    construct a thread object, you will first need to create a class
    derived from the processing class, declared in this header file,
    and pass it to the thread constructor.

    Each derived processing class must provide a public run() function
    that defines the operations that the thread is expected to carry
    out.  The test stub in the accompanying implementation file shows
    how to define classes derived from processing.

    A client can access the proc object using thread::proc() which
    returns the threads pointer to its processing object.  You might
    what to do this, before calling thread::start() to set some
    parmaters in the processing object.  Remember that you must
    synchronize access to all static and member data in this derived
    processing class if clients use thread::proc().

    This Threads module provides a global function:
    
        thread* pThread = CreateThreadOnHeap(new procObject);
 
    intended to allow threads constructed on the heap to delete themselves
    and their procObject as they complete their assigned tasks.  Do not call
    this function with a pointer to a local procObject unless you like to
    see ASSERTS thrown.

    Note #1
    -------
    If you call t.proc() before t.start(), perhaps to set some parameters for
    t to use:
    
      MyProcessing* pMp = dynamic_cast<MyProcessing>(t.proc());
      if(pMp)
        pMp->setParameter(someParam);

    then, you don't need to synchronize the processing code.  However, if
    you call t.proc() after t.start(), then you must synchronize all member
    data, statics, and globals of MyProcessing you use in setParameter(...).
//
    Public Interface
    ================
    class myProc : public processing { ... }
    myProc proc;                // create a processing object
    thread t(proc);             // create a thread object
    processing* pP = t.proc();  // return pointer to thread's copy of proc
    t.start();                  // create running Win32 thread
    t.wait();                   // wait for thread to complete
    t.sleep(50);                // number of milliseconds to sleep
    t.canSuspend(false);        // set false just before thread acquires lock
    t.canSuspend(true);         // set true just after thread releases lock
    t.suspend();                // suspend thread operation
    t.resume();                 // resume thread operation
    if(t.alive()) ...           // has thread terminated?
    t.setPriority(high);        // sets priority to low, normal, or high
    HANDLE th = t.handle();     // return Win32 thread handle
    unsigned int id = t.id();   // returns t's thread ID
    unsigned int id = thread::currentThread();
                                // return caller's thread ID
    thread* pThread = CreateThreadOnHeap(new myProc);
                                // runs thread and proc from C++ heap
                                // and deletes both when processing ends
    t.willSelfDestruct();       // t calls "delete this;" and delete on its
                                // processing* pointer, when terminating
    thread* pThread = CreateThreadOnHeap(new myThreadProc);
                                // when done thread kills myThreadProc and self
    threadPool tp;              // creates a "threadPool"
    tp.add(pThread);            // gives threadPool a thread to hold onto

    Note #2
    -------
    Sometimes you need to create threads in a loop, and each time around the
    loop you create a new thread.  To avoid overwritting a running thread, 
    with disasterous consequences, you need to create the threads and their
    processing objects on the c++ heap.  You do that like this:

      class MyProc : public processing { ... };
        // some code here

      threadPool thrdp;
      while(true)
      {
        // some code that will need a thread, but eventually break

        thread* pThread = CreateThreadOnHeap(new MyProc);
        thrdp.add(pThread);
        pThread->start();

        // don't call pThread->wait() here.
      }
      thrdp.waitAll();

    Now, each thread will merrily run along until finished, then will
    delete its MyProc object and itself.
*/
//
///////////////////////////////////////////////////////////////////////
//  Build Process                                                    //
///////////////////////////////////////////////////////////////////////
//  Files required to build test stub:                               //
//    threads.h, threads.cpp, locks.h, locks.cpp, blockingQueue.h    //
//                                                                   //
//  Compiler Command:                                                //
//    cl /GX /MTd /DTEST_THREADS threads.cpp locks.cpp               //
///////////////////////////////////////////////////////////////////////
/*  
    Note: You need to compile in IDE with project property setting:
    C/C++\Code Generation\Runtime Library = Multithreaded Debug (/MTd).

    Maintenance History
    ===================
    ver 5.3 : 24 Apr 2006
    - cosmetic changes to function prologues
    ver 5.2 : 17 Apr 2006
    - changed the data structure in threadPool class from std::vector
      to std::map.  This was done to support easily removing dead threads.
    - dead threads are now removed from threadPool on each call to add(...).
    ver 5.1 : 04 Apr 2006
    - eliminated default parameter in CreateThreadOnHeap and provided
      an overload, so the processing class does not have to provide a
      default constructor if it is not needed.
    ver 5.0 : 21 Feb 2006
    - removed clone() function from processing objects
    - added CreateThreadOnHeap() global function
    - changed the argument of the thread constructor from a processing&
      to a processing*.  This is a BREAKING change.
    ver 4.2 : 14 Apr 2005
    - added locks for all modifications of _alive.
    ver 4.1 : 12 Apr 2005
    - added threadd::proc() to return a pointer to thread's proc object
      allowing mutations while the thread is running.  Be careful to
      synchronize any static or member data of the derived processing
      class.
    ver 4.0 : 12 Apr 2005
    - added thread::id(), added missing information on manual page,
      moved two #includes to test stub.
    ver 3.9 : 09 Apr 2005
    - separated threads into three modules, this module, locks,
      and blockingQueues.  Version history notes with fixes for 
      locks or blockingQueues were moved to those modules.
    ver 3.8 : 31 Mar 2005
    - added note about project settings to manual page.
    ver 3.5 : 27 Apr 2004
    - added a static function currentThread() to return thread id
    ver 3.3 : 10 Apr 2004
    - a pure virtual clone() function has been added to the processing
      interface so that each thread can make its own copy of the proc.
      That avoids a problem when the derived proc object is declared
      inside a loop, causing each prior proc to get blown away.  That
      could be avoided in ver 3.2 by declaring the proc instance above
      the loop.  Now that is not necessary.
    ver 3.2 : 08 Apr 2004
    - a few cosmetic changes, but no functional changes
    ver 3.0 : 08 Apr 2004
    - added willSelfDestruct() function which you call if, and only if,
      you created a thread on the heap.  The thread then deletes itself
      when it returns from its processing::run() function.  You will
      find a demonstration in the test stub for this module.
    - changed the way handles are closed in the start() function to
      avoid an irritating exception thrown only during debugging.
//
    ver 2.1 : 18 Nov 2001
    - added setPriority() and alive() member functions
    ver 2.0 : 10 Nov 2001
    - added procWithQ class, providing queuing services for its thread
      (removed in ver 3.9)
    ver 1,2 : 05 Nov 2001
    - made processing class abstract
    - provided fixes to the manual page
    ver 1.1 : 28 Oct 2001
    - added missing file names to Build Process comments
    ver 1.0 : 26 Oct 2001
    - first release
*/
//
#include <process.h>
#include <Windows.h>
#include <map>
#include "locks.h"

/////////////////////////////////////////////////////////////////////
//  thread processing interface declaration

class processing
{
public:
  virtual ~processing() {}
  virtual void run()=0;
};

/////////////////////////////////////////////////////////////////////
//  thread class declaration

class thread
{
public:
  enum priority { low, normal, high };
  thread(processing* pProc); 
  virtual ~thread();
  void start();
  void wait();
  void waitAll();
  unsigned int id();                  // valid after calling start()
  static unsigned int currentThread();// returns caller's thread id
  HANDLE handle();
  void canSuspend(bool b=true);       // set false if thread holds a lock
  bool suspend();                     // otherwise thread may deadlock
  void resume();
  void sleep(int MilliSec);
  void setPriority(priority pr);
  bool alive();
  void willSelfDestruct(bool b=true); // call this only if thread is on heap
  processing* proc();

protected:
  processing* _pProc;
  LLock _lck;
  HANDLE th;
  unsigned int _threadID;
  static unsigned int __stdcall threadProc(void *pSelf);
  bool _canSuspend;
  bool _alive;
  bool _suicidal;
};
//
//----< constructor >------------------------------------------------

inline thread::thread(processing* pProc)
       : _pProc(pProc), _suicidal(false), _canSuspend(true), th(0) {}

//----< enable suicide - call this only if created on heap >---------

inline void thread::willSelfDestruct(bool b) { _suicidal = b; }

//----< return thread handle >---------------------------------------

inline HANDLE thread::handle() { return th; }

//----< get current thread id >--------------------------------------

inline unsigned int thread::currentThread() 
{
  return ::GetCurrentThreadId(); 
}

//----< decrement suspend count >------------------------------------
//
//  if count is zero, make thread schedualable
//
inline void thread::resume() { ::ResumeThread(th); }

//----< put thread to sleep for t milliseconds >---------------------

inline void thread::sleep(int t) { ::Sleep(t); }

//----< is thread locked? >------------------------------------------

inline void thread::canSuspend(bool b) { _canSuspend = b; }

//----< return thread ID >-------------------------------------------

inline unsigned int thread::id() 
{ 
  return _threadID; 
}

//----< return thread ID >-------------------------------------------
//
//  You must synchronize all static and member data of the class
//  that _pProc points to if clients use the pointer returned by
//  this function.

inline processing* thread::proc() { return _pProc; }

//
//----< create thread and proc on heap >-----------------------------
//
// If a pointer to a class derived from processing is supplied, it will
// be used, assuming the pointer refers to an object on the heap, and
// deleted at the end of processing.  Otherwise, a new instance is created.
//
template <typename T>
thread* CreateThreadOnHeap(T* pProcessingType)
{
  thread* pThread;
  pThread = new thread(pProcessingType);
  pThread->willSelfDestruct();
  return pThread;
}

//----< create thread and proc on heap >-----------------------------
//
// If a pointer to a class derived from processing is supplied, it will
// be used, assuming the pointer refers to an object on the heap, and
// deleted at the end of processing.  Otherwise, a new instance is created.
//
template <typename T>
thread* CreateThreadOnHeap()
{
  thread* pThread;
  pThread = new thread(new T);
  pThread->willSelfDestruct();
  return pThread;
}

/////////////////////////////////////////////////////////////////////
// Not a real thread pool yet - just holds onto handles for
// waitAll()

class threadPool
{
public:
  void add(thread* pThread);
  size_t size() { return handles.size(); }
  void waitAll();
private:
  std::map<HANDLE,thread*> handles;
};

#endif

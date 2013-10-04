//>Maker.cpp

#ifndef PROFILERH
#define PROFILERH

#ifdef NOPROFILER
#undef USEPROFILER
#endif


#define PRFLTRACEENABLE(a)

#define pprnt if (false PRFLTRACEENABLE(||true)) prnt

#ifdef USEPROFILER

#define _PRFLFN(name) Prfl ___prfl(name,false PRFLTRACEENABLE(||true))
#define _PRFLNOTE(notenm,name) Prfl notenm(name,false PRFLTRACEENABLE(||true))


#include "Hash.h"

#ifdef WIN32
#define PRFL_MUTEX HANDLE
#define PRFL_INITMUTEX(mutex) mutex=CreateMutex(NULL,false,NULL)
#define PRFL_CLOSEMUTEX(mutex) CloseHandle(mutex)
#define PRFL_AQUIREMUTEX(mutex) WaitForSingleObject(mutex,INFINITE)
#define PRFL_RELEASEMUTEX(mutex) ReleaseMutex(mutex)
#define SLEEPMS(ms) Sleep(ms)
#else
#define PRFL_MUTEX pthread_mutex_t
#define PRFL_INITMUTEX(mutex) pthread_mutex_init(&(mutex),NULL)
#define PRFL_CLOSEMUTEX(mutex) pthread_mutex_destroy(&(mutex))
#define PRFL_AQUIREMUTEX(mutex) pthread_mutex_lock(&(mutex))
#define PRFL_RELEASEMUTEX(mutex) pthread_mutex_unlock(&(mutex))
#define SLEEPMS(ms) usleep(ms*1000)
#endif

#define MAXPROFILERDEEP 1000
#define PRFLIN(name) \
  do { \
    PRFL_AQUIREMUTEX(Prfl::mutex);   \
    Prfl::fnName[Prfl::deep++]=name; \
    PRFL_RELEASEMUTEX(Prfl::mutex); \
  } while(0);

#define PRFLOUT Prfl::deep--;

class Prfl {// not multithread safe, i.e., only profile one thread at a time
public:
  static const char *fnName[MAXPROFILERDEEP],*copiedFnName[MAXPROFILERDEEP];
  static long deep,copiedDeep;
  static PRFL_MUTEX mutex;
  static void init() {
    deep=0;
    PRFL_INITMUTEX(mutex);
  }
  bool isTraced;
  Prfl(const char *name) {
    PRFLIN(name);
    isTraced=false;
  }
  Prfl(const char *name,bool traced) {
    PRFLIN(name);
    if (isTraced=traced) printTraceIn(name);
  }
  static void printTraceIn(const char *name) {
    prnt("\n ",name);
    for (long i=0;i<deep;i++) prnt(">");
    prnt("%s  ",name);
  }
  static void printTraceOut(const char *name) {
    prnt("\n ",name);
    for (long i=0;i<deep;i++) prnt(" ");
    prnt("<%s  ",name);
  }
  ~Prfl() {
    PRFLOUT;
    if (isTraced) printTraceOut(Prfl::fnName[Prfl::deep]);
  }
  static void copy() {
    PRFL_AQUIREMUTEX(mutex); \
    memcpy(copiedFnName,fnName,sizeof(char*)*(copiedDeep=deep));
    PRFL_RELEASEMUTEX(mutex); \
  }
};

#ifdef MAINCPPFILE
const char *Prfl::fnName[MAXPROFILERDEEP];
const char *Prfl::copiedFnName[MAXPROFILERDEEP];
long Prfl::deep;
long Prfl::copiedDeep;
PRFL_MUTEX Prfl::mutex;
#endif



class PrflFn : public ManagedSetItem, public StringHashable {
  PubDefCC("Pr");
protected:
  PrflFn(const char *afunc) : StringHashable(afunc), func(afunc) {timesSeen=1;timesNotSeen=0;hashTable+=*this;mark=staticMark-1;};
public:
  static ManagedStringHashSet<PrflFn> hashTable;
  String func;
  long timesSeen,timesNotSeen,mark;
  static long staticMark,staticNumAdded;

  static PrflFn *construct(ContextIndex &ci,PrflFn *nul,WRD typcc) {return(NULL);}
  virtual void destroy() {_DELETE(PrflFn,this);}
  virtual long orderRelationTo(Ordered &to,long relation=0) {
    PrflFn &tofn=*(PrflFn *)&to;
    if (relation==0) return(func<tofn.func?-1:(func>tofn.func?1:0));
    else return(timesSeen>tofn.timesSeen?-1:(timesSeen<tofn.timesSeen?1:0));
  }

  static void init() {Prfl::init();staticMark=staticNumAdded=0;hashTable.clear();}

  static void addCurrentStats() {
    Prfl::copy();
    staticNumAdded++;
    staticMark++;
    PrflFn *func;
    for (long i=0;i<Prfl::copiedDeep;i++) {
      if (!(func=hashTable[Prfl::copiedFnName[i]])) func=NEWCONSb(PrflFn,(Prfl::copiedFnName[i]));
      else if (func->mark!=staticMark) func->timesSeen++;
      func->mark=staticMark;
    }
    ForEachInSet(func,hashTable) if (func->mark!=staticMark) func->timesNotSeen++;
  }


#ifdef WIN32

  static void spawnProfiler() {
    init();
    HANDLE thrd=CreateThread(NULL,0,profilerProc,NULL,0,NULL);
  }

  static DWORD WINAPI profilerProc(LPVOID lpParameter) {
#else
  static void spawnProfiler() {
    pthread_t thrd;
    init();
    int rc = pthread_create(&thrd, NULL, &profilerProc, NULL); 
    if(rc) ERROR("Could not create profiler thread");

    //    pthread_join( threads[count], NULL);
  }

  static void* profilerProc( void *argPointer ) {
#endif
    while (true) {
      long num;
      for (num=0;num<500;num++) {SLEEPMS(10);addCurrentStats();}
      PrflFn *func;
      ArraySet<PrflFn> funcs(false,false,0,"funcs");
      funcs=hashTable;
      funcs.sort(1);
      prnt("\nProfiler:%d\n",num);
      if (staticNumAdded) {
        ForEachInSet(func,funcs) if (func->timesSeen) {
          prnt("%5.3f %5.3f %s\n",double(func->timesSeen)/staticNumAdded,double(func->timesNotSeen)/staticNumAdded,func->func.a);
          func->timesSeen=func->timesNotSeen=0;
        }
        staticNumAdded=0;
      }
      for (long i=0;i<Prfl::copiedDeep;i++) prnt("[%d]:%s\n",i,Prfl::copiedFnName[i]);
      prnt("\n");
    }
    return(0);
  }
};

#ifdef MAINCPPFILE
ManagedStringHashSet<PrflFn> PrflFn::hashTable;
long PrflFn::staticMark;
long PrflFn::staticNumAdded;
#endif


#else

#define _PRFLFN(a)
#define _PRFLNOTE(a,b)

#endif

#define PRFLFN(a) _PRFLFN(a)
#define PRFLNOTE(a,b) _PRFLNOTE(a,b)

#endif

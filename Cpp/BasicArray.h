//>Maker.cpp

#ifndef BASICARRAYH
#define BASICARRAYH

#include "Basic.h"


#define MINARRSIZE 8
#define ARRQUANTUM 8

#undef NOTEENABLE
#define NOTEENABLE(a)


#define ARRmemmove(to,from,cnt) memmove(to,from,cnt);
#define ARRmemcpy(to,from,cnt) memcpy(to,from,cnt);
#define ARRmemset(to,val,cnt) memset(to,val,cnt);
//#define ARRmemmove(to,from,cnt) do {_ensurePtr(to,cnt);memmove(to,from,cnt);} while (0);
//#define ARRmemcpy(to,from,cnt) do {_ensurePtr(to,cnt);memcpy(to,from,cnt);} while (0);
//#define ARRmemset(to,val,cnt) do {_ensurePtr(to,cnt);memset(to,val,cnt);} while (0);

template <class I> class BasicAllocableArray {// not suitable for a class as I, only structs, ptrs, basic datatypes
  PubDefCC("aA");
  long __size;       // actual size of allocated _a block, (number of I's)
  I *__a;            // allocated block
  char *__name;      // allows this array to be identified (can be NULL)
  long __nameSize;   // length of name block

 protected:
  long _aSize;
  I *_a;// protected var for allocated block, must equal __a at all times

 public:
  I *a;             // quick public access to block
                    //   do not change, unless to set to NULL (i.e. no public access to block)

  // virtual methods for derived classes

 protected:
  // handle a change in size, return the size 
  virtual long notifySetSize(long size) {
    // size is in blocks of ARRQUANTUM, and cannot be less than MINARRSIZE
    if (size<=MINARRSIZE) size=MINARRSIZE;
    else if (size%ARRQUANTUM) size+=ARRQUANTUM-(size%ARRQUANTUM);
    return(size);
  }

  // copy entire contents to a new block, called from setSize
  virtual void moveWholeArray(I *newa,long newSize) {
    FNNOTE3("Move whole array %d->%d",__size,newSize);
    if ((__size<0)||(newSize<0)) ERROR("size<0");
    if (newSize>__size) {
      memcpy(newa,__a,__size*sizeof(I));
      memset(newa+__size,0,(newSize-__size)*sizeof(I));
    }
    else memcpy(newa,__a,newSize*sizeof(I));
  }
 public:
  // methods of this class:
  //  BasicAllocableArray(long initSize=0, const char *aname=NULL) {
  //  virtual ~BasicAllocableArray() {
  //  I *A() {
  //  long aSize() {
  //  const char *name() {
  //  void setName(const char *aname) {
  //  void setSize(long newSize,bool ifBigger=true,bool powerOfTwo=true) {

   WRD typcc;

  BasicAllocableArray(WRD typcc,long initSize, const char *aname) {
    __name=NULL;
    this->typcc=typcc;
    FNNOTE2("BasicAllocableArray %d",initSize);
    if (aname) setName(aname);
    __a=a=NULL;__size=0;
    setSize(initSize);
    a=__a;
  }
  BasicAllocableArray(const BasicAllocableArray<I> &copy) {
    SERROR("Array copy constructor not supported");
  }
  virtual ~BasicAllocableArray() {
    FNNOTE("~BasicAllocableArray");
    //      prnt("~DELETE %p %d\n",__a,__size);
    DELETEA(typcc,__a,__size);
    if (__name) setName(NULL);
  }

  I *getA() {
    if (a&&(a!=__a)) ERROR("a has been changed!\n");
    if (_a!=__a) ERROR("_a has been changed!\n");
    if (__size!=_aSize) ERROR("aSize has been changed!\n");
    return(__a);
  }

  const char *name() {if (__name) return(__name); else return("<unnamed>");}
  void setName(const char *aname) {
    FNNOTE2("set name %s",aname?aname:"NULL");
    if (__name) DELETEA(typcc,__name,__nameSize);
    if (!aname) __name=NULL;
    else {
      __nameSize=long(strlen(aname))+1;
      WRAPNEWA_varExists(typcc,char,__name,__nameSize);
      memcpy(__name,aname,__nameSize);
    }
  }

  void setSize(long newSize,bool ifBigger=true,bool powerOfTwo=true) {
    FNNOTE2("setSize %d",newSize);

    if (newSize>0xffffffff) ERROR("The new size is very large (over a long)");
    if (newSize<0) ERROR2("attempt to set to negative size (%d)!\n",newSize);

    I *newa=NULL;
    if (powerOfTwo) {long newSizeWas=newSize;for (newSize=1;newSize<newSizeWas;newSize<<=1);}
    newSize=notifySetSize(newSize);

    // don't alloc if not neccesary (i.e. same size or big enough already)
    if ((newSize!=__size)&&!(ifBigger&&(__size>=newSize))) {
      WRAPNEWA_varExists(typcc,I,newa,newSize);

      if (!newa) ERROR("Out of memory");

      // copy _filled I's from old array
      if (__a) {
        moveWholeArray(newa,newSize);
        //      prnt("DELETE %p %d\n",__a,__size);
        DELETEA(typcc,__a,__size);
      }

      _a=__a=newa;
      _aSize=__size=newSize;
      if (a) a=__a;
    }
  }

  bool containsPtr(void *ptr) {if (a==__a) return((ptr>=__a)&&(ptr<__a+__size));else return(false);}
 protected:
  bool _containsPtr(void *ptr) {return((ptr>=_a)&&(ptr<_a+_aSize));}
  public:
  void _ensurePtr(void *ptr,long cnt=1) {
    if (cnt&&!(_containsPtr(ptr)&&_containsPtr(((char*)ptr)+(cnt-1)))) {
      ERROR("Access to ptr outside array");
    }
  }
};









template <class I> class BasicArray : public BasicAllocableArray<I> {
  PubDefCC("bA");
 protected:

  long lowestIndex,highestIndex;
  bool autoExtendUp,autoExtendDown;

 public:

  BasicArray(WRD typcc,long alowestIndex,bool aautoExtendDown,long ahighestIndex,bool aautoExtendUp,const char *aname=NULL) :
       lowestIndex(alowestIndex),highestIndex(alowestIndex-1),BasicAllocableArray<I>(typcc,0,aname) {
    FNNOTE4("BasicArray %d %d %s",alowestIndex,ahighestIndex,aname?aname:"NULL");
    autoExtendDown=aautoExtendDown;
    autoExtendUp=aautoExtendUp;
    setLowerBound(alowestIndex);
    setUpperBound(ahighestIndex);
    //this->a=NULL; // decomment to disallow public access to array
  }

  BasicArray(WRD typcc,long ahighestIndex=-1,bool aautoExtendUp=true,const char *aname=NULL) : 
        lowestIndex(0),highestIndex(-1),BasicAllocableArray<I>(typcc,0,aname) {
    FNNOTE3("BasicArray %d %s",ahighestIndex,aname?aname:"NULL");
    autoExtendDown=false;
    autoExtendUp=aautoExtendUp;
    setUpperBound(ahighestIndex);
    //this->a=NULL; // decomment to disallow public access to array
  }

  BasicArray<I> &operator=(BasicArray<I> &copy) {return(copyBasicArray(copy));}
  virtual BasicArray<I> &copyBasicArray(BasicArray<I> &copy) {
    FNNOTE("BasicArray=");
    BasicAllocableArray<I>::typcc=copy.typcc;
    if (this==&copy) return(*this);
    remove(lowestIndex,highestIndex+1-lowestIndex);
    lowestIndex=copy.lowestIndex;
    highestIndex=lowestIndex-1;
    autoExtendDown=autoExtendUp=true;
    insert(lowestIndex,*copy._a,copy.highestIndex+1-copy.lowestIndex);
    autoExtendDown=copy.autoExtendDown;
    autoExtendUp=copy.autoExtendUp;
    return(*this);
  }

  bool operator==(BasicArray<I> &b) {
    return((highestIndex==b.highestIndex)&&(lowestIndex==b.lowestIndex)&&!memcmp(this->_a,b._a,sizeof(I)*num()));
  }
  bool operator!=(BasicArray<I> &b) {return(!(*this==b));}

  virtual void setAutoExtend(bool down,bool up) {autoExtendDown=down;autoExtendUp=up;}
  virtual void lockBounds() {autoExtendDown=autoExtendUp=false;}
  virtual void unlockBounds() {autoExtendDown=autoExtendUp=true;}
  virtual void lockLowerBound() {autoExtendDown=false;}
  virtual void lockUpperBound() {autoExtendUp=false;}
  virtual void unlockLowerBound() {autoExtendDown=true;}
  virtual void unlockUpperBound() {autoExtendUp=true;}

  virtual void setLowerBound(long bound,long lock=false,bool ifLess=false,bool exactSize=true) {
    FNNOTE2("setLowerBound %d",bound);
    if (bound>highestIndex+1) bound=highestIndex+1;
    if (bound>lowestIndex) {
      if (!ifLess) {
        clearItems(lowestIndex,bound-lowestIndex);
        move(lowestIndex,bound,highestIndex+1-bound);
        lowestIndex=bound;
      }
    }
    else if (bound<lowestIndex) {
      this->setSize(1+highestIndex-bound,!exactSize,!exactSize);
      long lowWas=lowestIndex;
      lowestIndex=bound;
      move(lowWas,lowestIndex,highestIndex+1-lowWas);
      zero(lowestIndex,lowWas-lowestIndex);
    }
    if (lock) lockLowerBound();
  }

  virtual void setUpperBound(long bound,long lock=false,bool ifGreater=false,bool exactSize=true) {
    FNNOTE2("setUpperBound %d",bound);
    if (bound<lowestIndex-1) bound=lowestIndex-1;
    if (bound<highestIndex) {
      if (!ifGreater) {
        clearItems(bound+1,highestIndex-bound);
        highestIndex=bound;
      }
    }
    else if (bound>highestIndex) {
      this->setSize(1+bound-lowestIndex,!exactSize,!exactSize);
      long highWas=highestIndex;
      highestIndex=bound;
      zero(highWas+1,highestIndex-highWas);
    }
    if (lock) lockUpperBound();
  }

  bool extendToRegion(long start,long end,bool boundError=true,bool exactSize=false) {
    FNNOTE6("extend %d->%d %d->%d     (%d)",lowestIndex,start,highestIndex,end,this->_aSize);
    this->getA();
    if (start>end+1) ERROR3("invalid bounds : [%d,%d] )",start,end);
    if (start<lowestIndex) {
      if (!autoExtendDown) {
        if (boundError) ERROR4("start of region is out of bounds (indexed %d in [%d,%d] )",start,lowestIndex,highestIndex);
        else return(false);
      }
      setLowerBound(start,0,false,exactSize);
    }
    if (end>highestIndex) {
      if (!autoExtendUp) {
        if (boundError) ERROR4("end of region is out of bounds (indexed %d in [%d,%d] )",end,lowestIndex,highestIndex);
        else return(false);
      }
      setUpperBound(end,0,false,exactSize);
    }
    return(true);
  }

  I &itemAt(long index) {
    FNNOTE2("itemAt %d",index);
    extendToRegion(index,index);
    return(this->_a[index-lowestIndex]);
  }

  
  void move(long to,long start,long num,bool copy=false) {
    FNNOTE5("BasicArray::move %d <- %d+%d      (%d)",to,start,num,this->_aSize);
    if (num<0) ERROR2("move: num is negative (%d)",num);
    if ((start<lowestIndex)||(start+num>highestIndex+1)) ERROR5("move: out of bounds (indexed [%d,%d] in [%d,%d] )",start,start+num-1,lowestIndex,highestIndex);
    if ((to<lowestIndex)||(to+num>highestIndex+1)) ERROR5("move: to is out of bounds (indexed [%d,%d] in [%d,%d] )",to,to+num-1,lowestIndex,highestIndex);
    if (to>start) {
      if (to>=start+num) clearItems(to,num);
      else clearItems(start+num,to-start);
      ARRmemmove(this->_a+(to-lowestIndex),this->_a+(start-lowestIndex),sizeof(I)*num);
      if (copy) copied(start,(to>=start+num)?num:to-start);
      else zero(start,(to>=start+num)?num:to-start);
    }
    else if (to<start) {
      clearItems(to,(to+num<=start)?num:start-to);
      ARRmemmove(this->_a+(to-lowestIndex),this->_a+(start-lowestIndex),sizeof(I)*num);
      if (copy) {
        if (to+num<=start) copied(start,num);
        else copied(to+num,start-to);
      }
      else {
        if (to+num<=start) zero(start,num);
        else zero(to+num,start-to);
      }
    }
  }

  void shift(long shiftBy) {
    lowestIndex+=shiftBy;
    highestIndex+=shiftBy;
  }

  void zero() {zero(lowestIndex);}
  void zero(long start,long num=-1) {
    FNNOTE6("BasicArray::zero %d+%d     [%d:%d](%d)",start,num,lowestIndex,highestIndex,this->_aSize);
    if (num<0) num=highestIndex+1-start;
    if (num<0) ERROR2("zero: num is negative (%d)",num);
    if ((start<lowestIndex)||(start+num>highestIndex+1)) ERROR5("zero: out of bounds (indexed [%d,%d] in [%d,%d] )",start,start+num-1,lowestIndex,highestIndex);
    ARRmemset(this->_a+(start-lowestIndex),0,sizeof(I)*num);
  }

  void insertZeros(const long start,const long num) {
    FNNOTE4("BasicArray::insertZeros %d+%d     (%d)",start,num,this->_aSize);
    if (num<0) ERROR2("insertZeros: num is negative (%d)",num);
    if ((start<lowestIndex)||(start>highestIndex+1)) ERROR4("insertZeros: start is out of bounds (indexed %d in [%d,%d] )",start,lowestIndex,highestIndex);
    extendToRegion(start,highestIndex+num);
    ARRmemmove(this->_a+(start+num-lowestIndex),this->_a+(start-lowestIndex),sizeof(I)*(highestIndex+1-(start+num)));
    zero(start,num);
  }

  void remove(const long start,const long num) {
    FNNOTE4("BasicArray::remove %d+%d     (%d)",start,num,this->_aSize);
    if (num<0) ERROR2("remove: num is negative (%d)",num);
    if ((start<lowestIndex)||(start+num>highestIndex+1)) ERROR5("remove: out of bounds (indexed [%d,%d] in [%d,%d] )",start,start+num-1,lowestIndex,highestIndex);
    clearItems(start,num);
    ARRmemmove(this->_a+(start-lowestIndex),this->_a+(start+num-lowestIndex),sizeof(I)*(highestIndex+1-(start+num)));
    highestIndex-=num;
  }

  void copy(const long to,I &block,const long num) {// assumes no overlap
    FNNOTE4("BasicArray::copy %d <- block+%d     (%d)",to,num,this->_aSize);
    if (num<0) ERROR2("copy: num is negative (%d)",num);
    if ((to<lowestIndex)||(to+num>highestIndex+1)) ERROR5("copy: to is out of bounds (indexed [%d,%d] in [%d,%d] )",to,to+num-1,lowestIndex,highestIndex);
    clearItems(to,num);
    ARRmemcpy(this->_a+(to-lowestIndex),&block,sizeof(I)*num);
    copied(to,num);
  }

  void insert(const long to,I &block,const long num) {// assumes no overlap
    FNNOTE4("BasicArray::insert %d <- block+%d   (%d)",to,num,this->_aSize);
    if (num<0) ERROR2("copy: num is negative (%d)",num);
    if ((to<lowestIndex)||(to>highestIndex+1)) ERROR4("insert: to is out of bounds (indexed %d in [%d,%d] )",to,lowestIndex,highestIndex);
    insertZeros(to,num);
    ARRmemcpy(this->_a+(to-lowestIndex),&block,sizeof(I)*num);
    copied(to,num);
  }

  long upperBound() {return(highestIndex);}

  long lowerBound() {return(lowestIndex);}

  void clear(bool centre=true,bool unlock=true) {
    FNNOTE("BasicArray::clear");
    if (unlock) autoExtendDown=autoExtendUp=true;
    clearItems(lowestIndex,highestIndex+1-lowestIndex);
    this->setSize(0,false,false);
    if (centre) lowestIndex=0;
    highestIndex=lowestIndex-1;
  }

  long num() {return(highestIndex+1-lowestIndex);}



  // some virtual functions:

  virtual void clearItems(long start,long num) {// called whenever items are nullified
    zero(start,num);
  }

 protected:

  virtual void copied(long start,long num) {}// called whenever items are duplicated

 // virtual void destroy() {_DELETE(BasicArray<I>,this);}
};








#endif

//>Maker.cpp

#pragma once








template <class I> class ArrayStore {
  ManagedArray<StorableArray<I> > arrs;
  Array<Array<I>*> stack;
  long stackSize,alloced;
public:
  long num() {return(alloced);}
  long numTotal() {return(arrs.num());}
  ArrayStore(WRD typcc) : arrs(MakeCC("st"),typcc) {stackSize=alloced=0;}
  virtual ~ArrayStore() {fullClear();}
  void fullClear() {arrs.clear();stack.clear();stackSize=alloced=0;}
  void clear() {
    //if (stackSize!=arrs.num()) ERROR3("Stack size should equal number of arrays allocated, they are %d, %d respectively",stackSize,arrs.num());
    stackSize=0;arrs.clear();alloced=0;
  }
  operator Array<I>&() {
    alloced++;
    Array<I>&ret=(stackSize?*stack[--stackSize]:arrs.get(arrs.num()));
    ret.clear();
    return(ret);
  }
  void returnArr(Array<I> *&arr) {if (arr) {returnArr(*arr);arr=NULL;}}
  void returnArr(Array<I> &arr) {
    if (((RefCnt*)&arr)->getRefCnt()<0) ((RefCnt*)&arr)->addRef(1);
    arr.clear();
#ifdef GENDEBUG
    for (long i=0;i<stackSize;i++) if (stack[i]==&arr) ERROR("Duplicate array returned");//debug
#endif
    alloced--;stack.get(stackSize++)=&arr;
  }
};

template <class I> class ClassStore {
  ManagedArray<I> arrs;
  Array<I*> stack;
  long stackSize,alloced;
public:
  long num() {return(alloced);}
  long numTotal() {return(arrs.num());}
  ClassStore(WRD typcc) : arrs(MakeCC("st"),typcc) {stackSize=alloced=0;}
  virtual ~ClassStore() {fullClear();}
  void fullClear() {arrs.clear();stack.clear();stackSize=alloced=0;}
  void clear() {
    //if (stackSize!=arrs.num()) ERROR3("Stack size should equal number of objects allocated, they are %d, %d respectively",stackSize,arrs.num());
    stackSize=0;arrs.clear();alloced=0;
  }
  operator I&() {
    alloced++;
    return(stackSize?*stack[--stackSize]:arrs.get(arrs.num()));
  }
  void returnObj(I *&arr) {if (arr) {returnObj(*arr);arr=NULL;}}
  void returnObj(I &arr) {
    if (((RefCnt*)&arr)->getRefCnt()<0) ((RefCnt*)&arr)->addRef(1);
#ifdef GENDEBUG
    for (long i=0;i<stackSize;i++) if (stack[i]==&arr) ERROR("Duplicate class returned");//debug
#endif
    alloced--;stack.get(stackSize++)=&arr;
  }
};


template <class I> class StructStore {
  Array<I> arrs;
  Array<I*> stack;
  long stackSize,alloced;
public:
  long num() {return(alloced);}
  StructStore(long maxAlloc) {
    stackSize=alloced=0;
    arrs.setUpperBound(maxAlloc-1);
    stack.setUpperBound(maxAlloc-1);
    for (stackSize=0;stackSize<maxAlloc;stackSize++) stack[stackSize]=&arrs[stackSize];
  }
  virtual ~StructStore() {fullClear();}
  void fullClear() {arrs.clear();stack.clear();stackSize=alloced=0;}
  void clear() {
    //if (stackSize!=arrs.num()) ERROR3("Stack size should equal number of objects allocated, they are %d, %d respectively",stackSize,arrs.num());
    stackSize=0;arrs.clear();alloced=0;
  }
  operator I&() {
    alloced++;
    if (!stackSize) SERROR("Run out of room");
    return(*stack[--stackSize]);
  }
  void returnObj(I *&arr) {if (arr) {returnObj(*arr);arr=NULL;}}
  void returnObj(I &arr) {
#ifdef GENDEBUG
    for (long i=0;i<stackSize;i++) if (stack[i]==&arr) ERROR("Duplicate class returned");//debug
#endif
    alloced--;stack.get(stackSize++)=&arr;
  }
};







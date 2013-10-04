//>Maker.cpp


#ifndef QUICKHASHH
#define QUICKHASHH




template <class D> struct HashCell {
  WRD mark;
  WRD dependents;
  D data;
  ULL hashCode;
};
typedef HashCell<int> HashCellInt;
typedef HashCell<long> HashCellLong;
typedef HashCell<ULL> HashCellULL;
#define WriteHashCellInt(str,c,stri) do { \
  Writeint(str,(c).data,stri); \
  WriteULL(str,(c).hashCode,stri); \
} while(0)

#define ReadHashCellInt(str,c,stri) do { \
  Readint(str,(c).data,stri); \
  ReadULL(str,(c).hashCode,stri); \
} while(0)
#define WriteHashCellLong(str,c,stri) do { \
  Writelong(str,(c).data,stri); \
  WriteULL(str,(c).hashCode,stri); \
} while(0)

#define ReadHashCellLong(str,c,stri) do { \
  Readlong(str,(c).data,stri); \
  ReadULL(str,(c).hashCode,stri); \
} while(0)
#define WriteHashCellULL(str,c,stri) do { \
  WriteULL(str,(c).data,stri); \
  WriteULL(str,(c).hashCode,stri); \
} while(0)

#define ReadHashCellULL(str,c,stri) do { \
  ReadULL(str,(c).data,stri); \
  ReadULL(str,(c).hashCode,stri); \
} while(0)


#define WriteHashTableToFile_s(fil,celltypeasword,ht) do { \
  String str;long stri=0; \
  WriteHashTable(str,celltypeasword,ht,stri); \
  WriteToFile(fil,str,stri); \
} while(0)
#define ReadHashTableFromFile_s(fil,celltypeasword,ht) do { \
  String str;long stri=0; \
  ReadFromFile(fil,str); \
  ReadHashTable(str,celltypeasword,ht,stri); \
} while(0)

#define WriteHashTable(str,celltypeasword,ht,stri) do { \
  Array<celltypeasword > a; \
  (ht).asArray(a); \
  WriteArray(str,celltypeasword,a,stri); \
} while(0)
#define ReadHashTable(str,celltypeasword,ht,stri) do { \
  Array<celltypeasword > a; \
  ReadArray(str,celltypeasword,a,stri); \
  (ht).fromArray(a); \
} while(0)

#define WriteHashTableToFile(fil,celltypeasword,ht) do { \
  Array<celltypeasword > a; \
  (ht).asArray(a); \
  String str;long stri=0; \
  long i=a.num(); \
  Writelong(str,i,stri); \
  WriteToFile(fil,str,stri);stri=0; \
  IterateArray(celltypeasword,v,a, \
    Write##celltypeasword(str,v,stri); \
    WriteToFile(fil,str,stri);stri=0; \
  ); \
} while(0)
#define ReadHashTableFromFile(fil,celltypeasword,ht) do { \
  Array<celltypeasword > a; \
  String str;long stri; \
  ReadFromFile(fil,str);stri=0; \
  long i; \
  Readlong(str,i,stri); \
  a.setUpperBound(i-1); \
  IterateArray(celltypeasword,v,a, \
    ReadFromFile(fil,str);stri=0; \
    Read##celltypeasword(str,v,stri); \
  ); \
  (ht).fromArray(a); \
} while(0)


#define WriteHashTableLongToFile(fil,ht) WriteHashTableToFile(fil,HashCellLong,ht)
#define ReadHashTableLongFromFile(fil,ht) ReadHashTableFromFile(fil,HashCellLong,ht)

#define WriteHashTableLong(str,ht,stri) WriteHashTable(str,HashCellLong,ht,stri)
#define ReadHashTableLong(str,ht,stri) ReadHashTable(str,HashCellLong,ht,stri)


#define WriteHashTableIntToFile(fil,ht) WriteHashTableToFile(fil,HashCellInt,ht)
#define ReadHashTableIntFromFile(fil,ht) ReadHashTableFromFile(fil,HashCellInt,ht)

#define WriteHashTableInt(str,ht,stri) WriteHashTable(str,HashCellInt,ht,stri)
#define ReadHashTableInt(str,ht,stri) ReadHashTable(str,HashCellInt,ht,stri)









template <class D> class QuickHashTableContext : public ManagedSetItem {
  DefCC("#C");
  QuickHashTableContext(ContextIndex &aci) : ci(aci) {
    hashRandOffsetsa=hashRandOffsets.a;
    numHashRandOffsets=0;
    addToRandOffsets();
  }

public:
  ContextIndex ci;

  Array<HashCell<D> > tmpHashCellArr;
  Array<HashCell<D> > tmpHashCellArr2;
  Array<ULL> hashRandOffsets;
  ULL *hashRandOffsetsa;
  long numHashRandOffsets;



  static ManagedArray<QuickHashTableContext> contexts;
  static QuickHashTableContext *construct(ContextIndex &ctxt,QuickHashTableContext *nul,WRD typcc) {return(NEWCONSb(QuickHashTableContext,(ctxt)));}
  virtual void destroy() {_DELETE(QuickHashTableContext,this);}

  virtual ~QuickHashTableContext() {}


  void addToRandOffsets(long numAdd=0x400) {
    long numWas=numHashRandOffsets,newNum=numWas+numAdd;
    hashRandOffsets.setUpperBound(newNum);
    for (;numWas<newNum;numWas++) hashRandOffsets.a[numWas]=RandULL();

    hashRandOffsetsa=hashRandOffsets.a;
    numHashRandOffsets=newNum;
  }

#define ensureRunOffset(ctxt,iter) if ((ctxt).numHashRandOffsets<=(iter)) (ctxt).addToRandOffsets() // do not use in if's

};


#ifdef MAINCPPFILE
template <class D> ManagedArray<QuickHashTableContext<D> > QuickHashTableContext<D>::contexts;
#endif















#define qh_hashIndex(hash) ((hash)&hashTableMask)
#define qh_hashEntry(hash) (HashTablea[qh_hashIndex(hash)])
#define qh_hashEntryData(hash) (qh_hashEntry(hash).data)

#define qh_hashCellEmpty(cell) (((cell).mark&0xfffe)!=HashMark)
#define qh_hashCellFilled(cell) ((cell).mark==HashMark)
#define qh_hashCellDeleted(cell) ((cell).mark==HashMark+1)

#define qh_hashCellIsForHash(cell,hash) (qh_hashCellFilled(cell)&&((cell).hashCode==(hash)))

#define qh_nextHash(hash,hashRandOffsetsa,iter) ((hash)^=((hashRandOffsetsa)[(iter)++]))
#define qh_nextHashNoChange(hash,hashRandOffsetsa,iter) ((hash)^((hashRandOffsetsa)[iter]))




template <class D> class QuickHashTable {// super simple, but efficient, Hash table -- curently no remove
  PubDefCC("Q#");
public:
  QuickHashTableContext<D> &ctxt;

  HashCell<D> *HashTablea;
  
  long HashNum,numZombies,HashTableSize,hashTableMask,nextClearOffset,initialSize;
  WRD HashMark;
  D notFoundDataValue;

  float rehashShrinkTriggerSizeFactor,rehashGrowTriggerSizeFactor,rehashToSizeFactor;

  HashCell<D> *hashCellPtr;

  QuickHashTable(ContextIndex &aci,D anotFoundDataValue,long ainitialSize=1024) : ctxt(QuickHashTableContext<D>::contexts.get(aci.i)) {
    PRFLFN("QuickHashTable::QuickHashTable");

    HashTablea=NULL;
    
    notFoundDataValue=anotFoundDataValue;
    rehashShrinkTriggerSizeFactor=7.0f;rehashGrowTriggerSizeFactor=1.5f;
    rehashToSizeFactor=3.0f;
    init(initialSize=ainitialSize);
  }
  
  virtual ~QuickHashTable() {
    if (HashTablea) {
        DELETEA(MakeCC("hc"),HashTablea,HashTableSize);
    }
  }

  long numCellsFilled() {return(HashNum);}
  long numZombieCells() {return(numZombies);}
  long numCellsNonEmpty() {return(HashNum+numZombies);}
  float fillFactor() {return(float(HashNum+numZombies)/HashTableSize);}

  D operator[](ULL hash) {return(find(hash));}

  D find(ULL hash) {
    PRFLFN("QuickHashTable::find");

    hashCellPtr=&qh_hashEntry(hash);
    if (qh_hashCellEmpty(*hashCellPtr)) {
      hashCellPtr=NULL;
      return(notFoundDataValue);
    }
    else if (qh_hashCellIsForHash(*hashCellPtr,hash)) return(hashCellPtr->data);
    else {
      ULL *hashRandOffsetsa=ctxt.hashRandOffsetsa,tmpHash=qh_nextHashNoChange(hash,hashRandOffsetsa,0);
      for (long iter=1;true;qh_nextHash(tmpHash,hashRandOffsetsa,iter)) {
        if (qh_hashCellEmpty(*(hashCellPtr=&qh_hashEntry(tmpHash)))) {
          hashCellPtr=NULL;
//          prnt("%d,",iter);
          return(notFoundDataValue);
        }
        else if (qh_hashCellIsForHash(*hashCellPtr,hash)) {
//          prnt("%d,",iter);
          return(hashCellPtr->data);
        }
        ensureRunOffset(ctxt,iter);
      }
    }
    return(notFoundDataValue);// never reached
  }

  void add(ULL hash,D data) {
    PRFLFN("QuickHashTable::add");

#ifdef GENDEBUG
    this->find(hash);
    if (hashCellPtr) SERROR("Duplicate hash added to table");
#endif
    ULL tmpHash=hash;
    ULL *hashRandOffsetsa=ctxt.hashRandOffsetsa;
    for (long iter=0;qh_hashCellFilled(*(hashCellPtr=&qh_hashEntry(tmpHash)));qh_nextHash(tmpHash,hashRandOffsetsa,iter)) {
        if (!++hashCellPtr->dependents) {
            SERROR("Hash dependents wrapped around a WRD");
        }
        ensureRunOffset(ctxt,iter);
    }
    hashCellPtr->data=data;
    hashCellPtr->hashCode=hash;
    HashNum++;
    if (qh_hashCellEmpty(*hashCellPtr)) {
        hashCellPtr->mark=HashMark;
        hashCellPtr->dependents=0;
        if ((HashNum+numZombies)*rehashGrowTriggerSizeFactor>HashTableSize) {
            // prnt("%d %f %d\n",HashNum,rehashTriggerSizeFactor,HashTableSize);
            rehash();
            find(hash);
        }
    }
    else {
        hashCellPtr->mark=HashMark;
        if (!++hashCellPtr->dependents) {
            SERROR("Hash dependents wrapped around a WRD (2)");
        }
        numZombies--;
    }
  }

  bool remove(ULL hash) {
    PRFLFN("QuickHashTable::remove");

    this->find(hash);
    if (!hashCellPtr) return(false);

    HashNum--;
    
    hashCellPtr=&qh_hashEntry(hash);
    if (!qh_hashCellIsForHash(*hashCellPtr,hash)) {
      ULL *hashRandOffsetsa=ctxt.hashRandOffsetsa,tmpHash=qh_nextHashNoChange(hash,hashRandOffsetsa,0);
      for (long iter=1;!qh_hashCellIsForHash(*hashCellPtr,hash);qh_nextHash(tmpHash,hashRandOffsetsa,iter)) {
        if (!(hashCellPtr->dependents--)) {
            hashCellPtr->mark=(HashMark+0xfffe)&0xffff;
            numZombies--;
        }
      }
    }
    if (hashCellPtr->dependents--) {
        numZombies++;
        hashCellPtr->mark++;
    }
    else {
        hashCellPtr->mark=(HashMark+0xfffe)&0xffff;
    }
    long sz=long(rehashShrinkTriggerSizeFactor*HashNum),newsz;
    for (newsz=8;newsz<sz;newsz<<=1);

    if (newsz<HashTableSize) {
        // prnt("%d %f %d\n",HashNum,rehashTriggerSizeFactor,HashTableSize);
        rehash();
    }
    hashCellPtr=NULL;
    return(true);
  }

  bool changeData(ULL hash,D newData) {
    (*this)[hash];
    if (hashCellPtr) {hashCellPtr->data=newData;return(true);}
    else return(false);
  }

  bool changeDataForLastFind(D newData) {
    if (hashCellPtr) {hashCellPtr->data=newData;return(true);}
    else return(false);
  }

  void clear() {
    PRFLFN("QuickHashTable::clear");

    HashCell<D> *hashCellPtr=&HashTablea[long((((ULL)HashTableSize)*nextClearOffset)/0x7fff)];
    HashCell<D> *endHashCellPtr=&HashTablea[long((((ULL)HashTableSize)*(nextClearOffset+1))/0x7fff)];
    while (hashCellPtr<endHashCellPtr) (hashCellPtr++)->mark=HashMark;
    HashMark=(HashMark+2)&0xffff;
    if ((++nextClearOffset)>=0x7fff) nextClearOffset=0;
    HashNum=0;
    numZombies=0;
  }

  void init(double size) {
    PRFLFN("QuickHashTable::init");

    long sz=long(size);
    long szWas=HashTableSize;
    for (HashTableSize=8;HashTableSize<sz;HashTableSize*=2);
    hashTableMask=HashTableSize-1;
    if (HashTablea) {
        if (HashTableSize!=szWas) {
            DELETEA(MakeCC("hc"),HashTablea,szWas);
            HashTablea=NEWA(MakeCC("hc"),HashCell<D>,HashTableSize);
        }
    }
    else {
        HashTablea=NEWA(MakeCC("hc"),HashCell<D>,HashTableSize);
    }
    memset(HashTablea,0,sizeof(*HashTablea)*HashTableSize);
    
    HashNum=0;
    numZombies=0;
    HashMark=2;
    nextClearOffset=0;
  }

  bool rehash() {
    PRFLFN("QuickHashTable::rehash");

    if ((rehashToSizeFactor>=rehashShrinkTriggerSizeFactor)||(rehashToSizeFactor<=rehashGrowTriggerSizeFactor)) return(false);
    
    Array<HashCell<D> > &hashTableWas=asHashTable(ctxt.tmpHashCellArr2);

    WRD markWas=HashMark;

  //  prnt("%f\n",rehashToSizeFactor);
    init(rehashToSizeFactor*HashNum);
    HashCell<D> *hashCell=hashTableWas.a-1;
    HashCell<D> *enHashCell=hashTableWas.a+hashTableWas.num();
    while ((++hashCell)<enHashCell) if (hashCell->mark==markWas) add(hashCell->hashCode,hashCell->data);

    hashTableWas.clear();
    return(true);
  }


    Array<HashCell<D> > &asHashTable(Array<HashCell<D> > &ret) {
        ret.setUpperBound(HashTableSize-1);
        ret.copy(0,*HashTablea,HashTableSize);
        return(ret);
    }

  Array<HashCell<D> > &asArray(Array<HashCell<D> > &ret) {
    PRFLFN("QuickHashTable::asArray");

    Array<HashCell<D> > &HashTableCopy=asHashTable(ctxt.tmpHashCellArr2);

    ret.clear();
    FilterToArray(HashCell<D>,c,HashTableCopy,      HashCell<D>,toc,ret,
      c.mark==HashMark,
      toc=c;
    );
    return(ret);
  }
  Array<D*> &asArray(Array<D*> &ret) {
    PRFLFN("QuickHashTable::asArray");

    Array<HashCell<D> > &HashTableCopy=asHashTable(ctxt.tmpHashCellArr2);

    ret.clear();
    FilterToArray(HashCell<D>,c,HashTableCopy,      D*,toc,ret,
      c.mark==HashMark,
      toc=&c.data;
    );
    return(ret);
  }
  Array<D> &asArray(Array<D> &ret) {
    PRFLFN("QuickHashTable::asArray");

    Array<HashCell<D> > &HashTableCopy=asHashTable(ctxt.tmpHashCellArr2);

    ret.clear();
    FilterToArray(HashCell<D>,c,HashTableCopy,      D,toc,ret,
      c.mark==HashMark,
      toc=c.data;
    );
    return(ret);
  }
  void fromArray(Array<HashCell<D> > &a) {
    PRFLFN("QuickHashTable::fromArray");

    init(initialSize);
    addArray(a);
  }
  void addArray(Array<HashCell<D> > &a) {
    PRFLFN("QuickHashTable::addArray");

    IterateArray(HashCell<D>,c,a,      add(c.hashCode,c.data););
  }
};












#endif

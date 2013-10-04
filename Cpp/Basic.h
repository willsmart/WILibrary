//>Maker.cpp

#ifndef BASICH
#define BASICH


extern bool genDebug;
#ifdef GENDEBUG
#define A
#ifdef MAINCPPFILE
bool genDebug=true;
#endif
#else
#define A .a
#ifdef MAINCPPFILE
bool genDebug=false;
#endif
#endif



#ifdef WIN32
#define sleepms(ms) Sleep(ms)
#else
#define sleepms(ms) usleep((ms)*1000)
#endif





#define VERERR(args) do {if (errs<100) {if (dumpFn&&!errs) dumpFn(dumpFnvp);errs++;prnt("Verify error in %s: ",fnstr);prnt args;prnt("\n");}} while(0)
#define VERFN1(str) VERFN(str,NULL,NULL)
#define VERFN(str,adumpFn,adumpFnvp) \
  const char *fnstr=(str); \
  long errs=0; \
  void(*dumpFn)(void *)=(adumpFn); \
  void *dumpFnvp=(adumpFnvp)
#define VERFNEND \
  if (errs>0) SERROR3("%d Verification errors in %s",errs,fnstr)

#define VERFNPASSPARAMS fnstr,errs,dumpFn,dumpFnvp
#define VERFNPARAMS const char *fnstr,long &errs,void(*dumpFn)(void*),void *dumpFnvp



#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

typedef unsigned char BYT;
typedef unsigned short WRD;
typedef unsigned int DWRD;
typedef unsigned long long ULL;

extern FILE *efil;
extern FILE *ofil;
extern void* g_poin;
extern long g_poinlong;
extern ULL g_alloced;
extern ULL g_maxAlloc;
extern ULL g_maxAlloced;
extern ULL g_numAlloc;
#define NUMCC (0x10000)
extern ULL g_typAlloced[NUMCC];
extern ULL g_typMaxAlloced[NUMCC];
extern ULL g_typNumAlloc[NUMCC];
#ifdef MAINCPPFILE
void* g_poin;
long g_poinlong;
ULL g_alloced=0,g_numAlloc=0,g_maxAlloced=0,g_maxAlloc=(67*0x40000000LL)/10;
ULL g_typAlloced[NUMCC],g_typMaxAlloced[NUMCC],g_typNumAlloc[NUMCC];
bool g_typAllocInited=false;
#define initTypAlloc() { \
  if (!g_typAllocInited) { \
    g_typAllocInited=true; \
    for (long i=0;i<NUMCC;i++) g_typAlloced[i]=g_typMaxAlloced[i]=g_typNumAlloc[i]=0; \
  } \
}
FILE *efil=stderr;
FILE *ofil=stdout;
#endif


#ifdef WIN32
#ifdef POCKETPC
#define ftell64(file) ftell(file)
#define fseek64(file,offset,origin) fseek(file,long(offset),origin)
#else
#define ftell64(file) _ftelli64(file)
#define fseek64(file,offset,origin) _fseeki64(file,offset,origin)
#endif
#else
#define stricmp strcasecmp
#define ftell64(file) ftello(file)
#define fseek64(file,offset,origin) fseeko(file,offset,origin)
#endif


bool prnt(const char *strFormat,...);
#ifdef MAINCPPFILE
bool prnt(const char *strFormat,...) {
  va_list pArgs;
  va_start(pArgs, strFormat);
  vfprintf(efil, strFormat, pArgs);
  va_end(pArgs);
  return(true);
}
#endif
bool oprnt(const char *strFormat,...);
#ifdef MAINCPPFILE
bool oprnt(const char *strFormat,...) {
  va_list pArgs;
  va_start(pArgs, strFormat);
  vfprintf(ofil, strFormat, pArgs);
  va_end(pArgs);
  return(true);
}

bool minidebug=false;
#endif

#define eoprnt(...) do {prnt(__VA_ARGS__);oprnt(__VA_ARGS__);} while(0)

extern bool minidebug;

#define dprnt(...) do{if (debug) prnt(__VA_ARGS__);}while(0)
#define minidebprnt(...) do{if (minidebug) prnt(__VA_ARGS__);}while(0)



void progressPrnt(const char *name,long num,long div=1000,long allocDiv=10000);
#ifdef MAINCPPFILE
void progressPrnt(const char *name,long num,long div,long allocDiv) {
  if ((num%div)==0) prnt("%s%d",name,num/div);
  if ((num%allocDiv)==0) prnt("[%dMB]",long(g_alloced>>20));
}
#endif


bool msbox(const char *strFormat,...);
#ifdef MAINCPPFILE
#ifdef ERRORSAREMESSAGEBOXES
bool msbox(const char *strFormat,...) {
  char buf[10000],*pc;
  va_list pArgs;
  va_start(pArgs, strFormat);
  vsprintf(buf, strFormat, pArgs);
  va_end(pArgs);
  wchar_t wbuf[10000],*wpc;
  for (pc=buf,wpc=wbuf;*pc;) *(wpc++)=*(pc++);
  *wpc=0;
  MessageBox(NULL,wbuf,L"..",MB_OK);
  return(true);
}
#else 
bool msbox(const char *strFormat,...) {
  va_list pArgs;
  va_start(pArgs, strFormat);
  prnt("Message box:  ");
  vfprintf(efil, strFormat, pArgs);
  prnt("\n");
  va_end(pArgs);
  return(true);
}
#endif
#endif

#define Makecc(c1,c2) (((WRD)(c1))+(c1?(((WRD)(c2))<<8):0))
#define MakeCC(pc) (((WRD)(*(pc)))+(*(pc)?(((WRD)(pc)[1])<<8):0))
#define DefCC(str) static WRD _CC() {return(MakeCC(str));}
#define PubDefCC(str) public:DefCC(str)private:
#define VerCCc(c) do { \
  switch(c) { \
    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9': \
    case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j': \
    case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't': \
    case 'u':case 'v':case 'w':case 'x':case 'y':case 'z': \
    case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J': \
    case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T': \
    case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z': \
    case '.':case '/':case '?':case '!':case ',':case '#': \
    break; \
    default:ERROR2("Bad char (%c) in CC",c);break; \
} \
} while(0)

#define VerCC(us) do{VerCCc((us)&0xff);VerCCc(((us)>>8)&0xff);}while(0)

void *DoNew(WRD typcc,void *addr,long sizePer,long num,const char *type,long line,const char *file);
void DoDelete(WRD typcc,const void *addr,long sizePer,long num,long line,const char *file);
void AllocNote(const char *str);
void printTypeAllocs();
void oprintTypeAllocs();
void eoprintTypeAllocs();
#ifdef DELETE
#undef DELETE
#endif

#define WRAPALLOC(precode,...) precode { \
        for (long ___i=10;___i>0;___i--) { \
          try { \
            {__VA_ARGS__} \
            break; \
          } \
          catch (std::bad_alloc&) { \
            if (___i>1) WARN("Bad alloc, trying again"); \
            else SERROR("Bad alloc"); \
            sleepms(5000); \
          } \
        } \
}

#define NEW(typ) ((typ*)DoNew(typ::_CC(),(void*)new typ,sizeof(typ),1,#typ,__LINE__,__FILE__))
#define NEW2(typ,baseTyp) ((typ*)DoNew(typ::_CC(),(void*)new typ,sizeof(baseTyp),1,#typ,__LINE__,__FILE__)) // covers over a difficulty getting accurate g_alloc values
#define NEWA(typcc,typ,num) ((typ*)DoNew(typcc,(void*)new typ[num],sizeof(typ),num,#typ,__LINE__,__FILE__))
#define NEWCONS(typ,tcons) ((typ*)DoNew(typ::_CC(),(void*)new tcons,sizeof(typ),1,#typ,__LINE__,__FILE__))
#define NEWCONSb(typ,args) ((typ*)DoNew(typ::_CC(),(void*)new typ args,sizeof(typ),1,#typ,__LINE__,__FILE__))
#define NEWCONS2(typ,tcons,baseTyp) ((typ*)DoNew(typ::_CC(),(void*)new tcons,sizeof(baseTyp),1,#typ,__LINE__,__FILE__))
#define NEWCONStemplate(typptr,tcons,...) (typptr DoNew(__VA_ARGS__::_CC(),(void*)new __VA_ARGS__ tcons,sizeof(__VA_ARGS__),1,#__VA_ARGS__,__LINE__,__FILE__))
#define WRAPNEW(typ,var) WRAPALLOC(typ *var=NULL;,var=NEW(typ);)
#define WRAPNEWCONSb(typ,var,tcons) WRAPALLOC(typ *var=NULL;,var=NEWCONSb(typ,tcons);)
#define WRAPNEWA(typcc,typ,var,num) WRAPALLOC(typ *var=NULL;,var=NEWA(typcc,typ,num);)
#define WRAPNEW_varExists(typ,var) WRAPALLOC(var=NULL;,var=NEW(typ);)
#define WRAPNEWCONSb_varExists(typ,var,tcons) WRAPALLOC(var=NULL;,var=NEWCONSb(typ,tcons);)
#define WRAPNEWA_varExists(typcc,typ,var,num) WRAPALLOC(var=NULL;,var=NEWA(typcc,typ,num);)
#define _DELETE(typ,addr) do{if (addr) {DoDelete(typ::_CC(),addr,sizeof(*addr),1,__LINE__,__FILE__);delete addr;}}while(0)
#define _DELETEA(typcc,addr,num) do{if (addr) {DoDelete(typcc,addr,sizeof(*addr),num,__LINE__,__FILE__);delete[] addr;}}while(0)
#define _DELETENA(typcc,addr,num) do{if (addr) {DoDelete(typcc,addr,sizeof(*addr),num,__LINE__,__FILE__);delete addr;}}while(0)
#define DELETE(typ,addr) do{if (addr) {DoDelete(typ::_CC(),addr,sizeof(*addr),1,__LINE__,__FILE__);delete addr;addr=NULL;}}while(0)
#define DELETEA(typcc,addr,num) do{if (addr) {DoDelete(typcc,addr,sizeof(*addr),num,__LINE__,__FILE__);delete[] addr;addr=NULL;}}while(0)
#define DELETENA(typcc,addr,num) do{if (addr) {DoDelete(typcc,addr,sizeof(*addr),num,__LINE__,__FILE__);delete addr;addr=NULL;}}while(0)
#define ALLOCNOTE(str) AllocNote(str);



#ifdef MAINCPPFILE

#define ALLOCDEBUG 0
FILE *g_allocfil=NULL;

void printTypeAllocs(bool too,bool toe) {
  bool fst=true;
  for (long a=0;a<256;a++) for (long b=0;b<256;b++) {long i=a+(b<<8);
    if (g_typAlloced[i]||g_typNumAlloc[i]) {
      if (toe) prnt("%s%c%c x%d(%dMB)",fst?"":", ",i&0xff,(i>>8)&0xff,long(g_typNumAlloc[i]),long(g_typAlloced[i]>>20));
      if (too) oprnt("%s%c%c x%d(%dMB)",fst?"":", ",i&0xff,(i>>8)&0xff,long(g_typNumAlloc[i]),long(g_typAlloced[i]>>20));
      fst=false;
    }
  }
  if (!fst) {if (toe) prnt("\n");if (too) oprnt("\n");}
}
void printTypeAllocs() {printTypeAllocs(false,true);}
void oprintTypeAllocs() {printTypeAllocs(true,false);}
void eoprintTypeAllocs() {printTypeAllocs(true,true);}

void *DoNew(WRD typcc,void *addr,long sizePer,long num,const char *type,long line,const char *file) {
  if (!num) {
    prnt("Empty alloc");
  }

  if (((typcc&0xff)<' ')||((typcc&0xff)>127)||(((typcc>>8)&0xff)<' ')||(((typcc>>8)&0xff)>127)) {
    int i=0;
    i=1;
  }
  g_alloced+=sizePer*num;
  if (g_maxAlloced<g_alloced) g_maxAlloced=g_alloced;
  g_numAlloc++;

  if (typcc) {
    initTypAlloc();
    g_typAlloced[typcc]+=sizePer*num;
    if (g_typMaxAlloced[typcc]<g_typAlloced[typcc]) g_typMaxAlloced[typcc]=g_typAlloced[typcc];
    g_typNumAlloc[typcc]++;
  }

  if (g_alloced>g_maxAlloc) {
    fprintf(efil,"Maximum allocation reached (%dMB). Quitting for your own protection.",int(g_maxAlloc>>20));
#ifdef ERRORSAREMESSAGEBOXES
    PostQuitMessage(0);
#else
    exit(1);
#endif
  }
#if ALLOCDEBUG==1
  fprintf(efil,"New:%p ",addr);
#elif ALLOCDEBUG==2
  fprintf(efil,"New %s[%d]:%p (%d,%d) ",type,num,addr,long(g_numAlloc),long(g_alloced>>20));
#elif ALLOCDEBUG==5
  fprintf(efil,"New %s[%d]:%p (%d,%d) (sz %d:%d)  ",type,num,addr,long(g_numAlloc),long(g_alloced>>20),sizePer,num*sizePer);
#elif ALLOCDEBUG==6
  fprintf(efil,"New %s[%d]:%p (sz %d:%d) %s[%d] mem now %d\n",type,num,addr,sizePer,num*sizePer,file,line,long(g_alloced>>20));
#elif ALLOCDEBUG==6
  fprintf(efil,"New %s[%d]:%p (sz %d:%d) %s[%d] mem now %d\n",type,num,addr,sizePer,num*sizePer,file,line,long(g_alloced>>20));
#elif ALLOCDEBUG==7
  fprintf(efil,"New %s[%d]:%p mem:%d     ",type,num,addr,long(g_alloced>>20));
  if (!(g_allocfil||(g_allocfil=fopen("Alloced.txt","w+")))) {
    fprintf(efil,"Could not create Alloced.txt!!!!\n");
    return(addr);
  }
  fseek(g_allocfil,0,SEEK_END);
  fprintf(g_allocfil,"Active mem!  %p %d %s[%d] (sz %d:%d) %s[%d] mem now %d\n",addr,num*sizePer,type,num,sizePer,num*sizePer,file,line,long(g_alloced>>20));
#endif
  return(addr);
}
void DoDelete(WRD typcc,const void *addr,long sizePer,long num,long line,const char *file) {

  g_alloced-=sizePer*num;
  g_numAlloc--;

  if (typcc) {
   initTypAlloc();
   g_typAlloced[typcc]-=sizePer*num;
   g_typNumAlloc[typcc]--;
  }

#if ALLOCDEBUG==1
  fprintf(efil,"Delete:%p ",addr);
#elif ALLOCDEBUG==2
  fprintf(efil,"Delete [%d]:%p (%d,%d) ",num,addr,long(g_numAlloc),long(g_alloced>>20));
#elif ALLOCDEBUG==5
  fprintf(efil,"Delete [%d]:%p (%d,%d) (sz %d:%d)   ",num,addr,long(g_numAlloc),long(g_alloced>>20),sizePer,num*sizePer);
#elif ALLOCDEBUG==6
  fprintf(efil,"Delete [%d]:%p (sz %d:%d) %s[%d] mem now %d\n",num,addr,sizePer,num*sizePer,file,line,long(g_alloced>>20));
#elif ALLOCDEBUG==7
  fprintf(efil,"Delete [%d]:%p mem:%d     ",num,addr,long(g_alloced>>20));
  if (!(g_allocfil||(g_allocfil=fopen("Alloced.txt","w+")))) {
    fprintf(efil,"Could not create or open Alloced.txt!!!!\n");
    return;
  }
  fseek(g_allocfil,0,SEEK_SET);
  char buf[1000],*pc;
  long pos,i,lastDealloc=-1,numDealloc;
  void *addr2;
  while (1) {
    pos=ftell(g_allocfil);
    if (pc=fgets(buf,1000,g_allocfil)) {
      if ((!memcmp(pc,"Active mem!  ",13))&&sscanf(pc+13,"%p %d",&addr2,&i)) {
        if (addr2==addr) {
          fseek(g_allocfil,pos,SEEK_SET);
          fprintf(g_allocfil,"Cleaned mem  ");
          fseek(g_allocfil,0,SEEK_END);
          fprintf(g_allocfil,"     Deleted ok [%d]:%p (sz %d:%d) %s[%d] mem now %d\n",num,addr,sizePer,num*sizePer,file,line,long(g_alloced>>20));
          if (i!=num*sizePer) fprintf(g_allocfil,"          !!Mismatched size (%d!=%d)\n",i,num*sizePer);
          break;
        }
      }
      else if ((!memcmp(pc,"Cleaned mem  ",13))&&sscanf(pc+13,"%p",&addr2)) {
        if (addr==addr2) {lastDealloc=pos;numDealloc=2;}
      }
      else if ((!memcmp(pc,"MEM DEL #",9))&&(sscanf(pc+9,"%d %p",&i,&addr2)==1)) {
        if (addr==addr2) {lastDealloc=pos;numDealloc=i+1;}
      }
    }
    else if (lastDealloc>=0) {
      fseek(g_allocfil,lastDealloc,SEEK_SET);
      fprintf(g_allocfil,"MEM DEL #%3d ",numDealloc+1);
      fseek(g_allocfil,0,SEEK_END);
      fprintf(g_allocfil,"    !!!!!!!Deleted again (number %d) [%d]:%p (sz %d:%d) %s[%d] mem now %d\n",numDealloc,num,addr,sizePer,num*sizePer,file,line,long(g_alloced>>20));
      break;
    }
    else {
      fprintf(g_allocfil,"    !!!!!!!!Deleted non allocated memory [%d]:%p (sz %d:%d) %s[%d] mem now %d\n",num,addr,sizePer,num*sizePer,file,line,long(g_alloced>>20));
      break;
    }
  }
#endif
}

void AllocNote(const char *str) {
#if ALLOCDEBUG==7
  fprintf(efil,"Note:%s     ",str);
  if (!(g_allocfil||(g_allocfil=fopen("Alloced.txt","w+")))) {
    fprintf(efil,"Could not create or open Alloced.txt!!!!\n");
    return;
  }
  fseek(g_allocfil,0,SEEK_END);
  fprintf(g_allocfil,">>Note:%s\n",str);
#endif
}

#endif





//#define TRACEFOPEN
#ifndef TRACEFOPEN
#define FOpen(fn,md) fopen(fn,md)
#define FClose(fil) do {fclose(fil);fil=NULL;}while(0)
#else
FILE *FOpen(const char *fn,const char *md);
void FClose(FILE *&fil);
#ifdef MAINCPPFILE
FILE *FOpen(const char *fn,const char *md) {FILE *fil=fopen(fn,md);prnt("  fopen(\"%s\" , \"%s\")->%p\n",fn,md,fil);return(fil);}
void FClose(FILE *&fil) {prnt("  fclose(%p)\n",fil);fclose(fil);fil=NULL;}
#endif
#endif







#define TRACE(str)
#define GTRACE(str)
#define TRACE1(str,var1)
#define GTRACE1(str,var1)
#define TRACE2(str,var1,var2)
#define GTRACE2(str,var1,var2)
#define PRINTTRACE
#define PRINTTRACEANDEXIT exit(0);
#define UPTO(str)



extern bool g_afterFault;
extern long g_breakAt,g_breakCnt;
void breakPoint();
#ifdef MAINCPPFILE
bool g_afterFault=false;
long g_breakAt=-1,g_breakCnt=0;
void breakPoint() {
  if ((++g_breakCnt)==g_breakAt) {
    fprintf(efil,"break at %ld\n",g_breakAt);
    getc(stdin);
    g_afterFault=true;
  }
}
#endif
#define BP breakPoint()
#define BP1(bat) do {g_breakAt=bat;prnt(".bp%d.",g_breakCnt+1);BP;} while(0)



#include "ErrorsAndNotes.h"




class RefCnt {
  DefCC("rc");
  long _refCnt;

public:

  RefCnt() {_refCnt=0;}


  virtual void destroy() {_DELETE(RefCnt,this);}// please provide your own, as this one may cause the g_alloced count to be off

  long getRefCnt() {return(_refCnt);}

  bool addRef(long add) {
    if ((_refCnt+=add)<0) WARN2("Reference count is below zero (%d)",_refCnt);
    if (_refCnt<=0) {destroy();return(true);}
    return(false);
  }
};





// the default relation for sets is OrderByPointer
#define OrderByPointer -1
#define NoOrder -2
#define OrderAsPrevious -3
#define OrderRelation(of,to,relation) ((relation)!=OrderByPointer?(of).orderRelationTo((to),(relation)):(&(of)<&(to)?-1:(&(of)==&(to)?0:1)))

class Ordered {
 public:
  // implementations of this can assume 'to' has same or similar class to 'this'
  virtual long orderRelationTo(Ordered &to,long relation=0) {return(this<&to?-1:(this>&to?1:0));}// note that relation -1 is reserved for pointer comparison
};

class ManagedSetItem : public RefCnt , public Ordered {};

struct QWRD {// should be an unsigned long long and one day will get round to changing the code
  DWRD l,h;
};
#define QWRDSEQUAL(a,b) (((a).l==(b).l)&&((a).h==(b).h))


#include "Constants.h"
#include "BasicConvert.h"


struct ContextIndex {
  long i;
};


extern ContextIndex g_contexti;
#ifdef MAINCPPFILE
ContextIndex g_contexti={0};
#endif





extern long long g_filSize;
extern char g_readFailed;
#ifdef MAINCPPFILE
long long g_filSize=0;
char g_readFailed='x';
#endif


#define Readlong(str,d,stri) do { \
  if (sizeof(long)==4) ReadDatum(str,d,stri,4); \
  else if (sizeof(long)==8) { \
    char buf[4],buf2[8]={0,0,0,0,0,0,0,0}; \
    ReadDatum(str,buf,stri,4); \
    buf2[0]=buf[0];buf2[1]=buf[1];buf2[2]=buf[2];buf2[3]=buf[3]; \
    d=*(long*)buf2; \
    if (d&0x80000000LL) d|=0xffffffff00000000LL; \
  } \
  else SERROR("Size of long should be 4 or 8"); \
} while (0)
#define Writelong(str,d,stri) do { \
  if (sizeof(long)==4) WriteDatum(str,d,stri,4); \
  else if (sizeof(long)==8) { \
    if ((d&0xffffffff80000000LL)&&((d&0xffffffff80000000LL)!=0xffffffff80000000LL)) SERROR("long overflow"); \
    char buf[8],buf2[4]; \
    (*(long*)buf)=d; \
    buf2[0]=buf[0];buf2[1]=buf[1];buf2[2]=buf[2];buf2[3]=buf[3]; \
    WriteDatum(str,buf2,stri,4); \
  } \
  else SERROR("Size of long should be 4 or 8"); \
} while (0)

#define Readfloat(str,d,stri) ReadDatum(str,d,stri,4)
#define Writefloat(str,d,stri) WriteDatum(str,d,stri,4)
#define ReadDWRD(str,d,stri) ReadDatum(str,d,stri,4)
#define WriteDWRD(str,d,stri) WriteDatum(str,d,stri,4)
#define Readdouble(str,d,stri) ReadDatum(str,d,stri,8)
#define Writedouble(str,d,stri) WriteDatum(str,d,stri,8)
#define ReadULL(str,d,stri) ReadDatum(str,d,stri,8)
#define WriteULL(str,d,stri) WriteDatum(str,d,stri,8)
#define Readbool(str,d,stri) ReadDatum(str,d,stri,1)
#define Writebool(str,d,stri) WriteDatum(str,d,stri,1)
#define Readchar(str,d,stri) ReadDatum(str,d,stri,1)
#define Writechar(str,d,stri) WriteDatum(str,d,stri,1)
#define ReadBYT(str,d,stri) ReadDatum(str,d,stri,1)
#define WriteBYT(str,d,stri) WriteDatum(str,d,stri,1)
#define Readshort(str,d,stri) ReadDatum(str,d,stri,2)
#define Writeshort(str,d,stri) WriteDatum(str,d,stri,2)
#define Readint(str,d,stri) ReadDatum(str,d,stri,4)
#define Writeint(str,d,stri) WriteDatum(str,d,stri,4)
#define ReadWRD(str,d,stri) ReadDatum(str,d,stri,2)
#define WriteWRD(str,d,stri) WriteDatum(str,d,stri,2)



#define ReadDatum(str,d,stri,dlen) do { \
  if (g_readFailed!='y') { \
    if (dlen!=sizeof(d)) SERROR3("Wrong size (%d != %d)",dlen,sizeof(d)); \
    memcpy(&(d),str.a+stri,sizeof(d)); \
    stri+=sizeof(d); \
  } \
  else memset(&(d),0,sizeof(d)); \
} while(0)

#define WriteDatum(str,d,stri,dlen) do { \
  if (dlen!=sizeof(d)) SERROR3("Wrong size (%d != %d)",dlen,sizeof(d)); \
  (str).ensureLength(stri+sizeof(d)); \
  memcpy(str.a+stri,&(d),sizeof(d)); \
  stri+=sizeof(d); \
} while(0)


#define _ReadArrayBlock(str,arr,stri,dlen) do { \
  if (dlen!=sizeof((arr).a[0])) SERROR3("Wrong size (%d != %d)",dlen,sizeof((arr).a[0])); \
  long ra_ub,ra_lb; \
  Readlong(str,ra_lb,stri); \
  Readlong(str,ra_ub,stri); \
  long ra_sz=(ra_ub+1-ra_lb)*sizeof((arr).a[0]); \
  if (ra_ub<(arr).lowerBound()) { \
    (arr).setLowerBound(ra_lb,true); \
    (arr).setUpperBound(ra_ub,true); \
  } \
  else { \
    (arr).setUpperBound(ra_ub,true); \
    (arr).setLowerBound(ra_lb,true); \
  } \
  memcpy((arr).a,(str).a+stri,ra_sz); \
  stri+=ra_sz; \
} while(0)


#define ReadArray(str,type,arr,stri) do { \
  if (g_readFailed!='y') { \
    long ra_ub,ra_lb; \
    Readlong(str,ra_lb,stri); \
    Readlong(str,ra_ub,stri); \
    if (ra_ub<(arr).lowerBound()) { \
      (arr).setLowerBound(ra_lb,true); \
      (arr).setUpperBound(ra_ub,true); \
    } \
    else { \
      (arr).setUpperBound(ra_ub,true); \
      (arr).setLowerBound(ra_lb,true); \
    } \
    IterateArray(type,o,arr,      Read##type(str,o,stri);); \
  } \
  else (arr).clear(); \
} while(0)

#define _WriteArrayBlock(str,arr,stri,dlen) do { \
  if (dlen!=sizeof((arr).a[0])) SERROR3("Wrong size (%d != %d)",dlen,sizeof((arr).a[0])); \
  long ra_lb=(arr).lowerBound(),ra_ub=(arr).upperBound(); \
  Writelong(str,ra_lb,stri); \
  Writelong(str,ra_ub,stri); \
  long ra_sz=(ra_ub+1-ra_lb)*sizeof((arr).a[0]); \
  (str).ensureLength(stri+ra_sz); \
  memcpy((str).a+stri,(arr).a,ra_sz); \
  stri+=ra_sz; \
} while(0)

#define WriteArray(str,type,arr,stri) do { \
  long ra_lb=(arr).lowerBound(),ra_ub=(arr).upperBound(); \
  Writelong(str,ra_lb,stri); \
  Writelong(str,ra_ub,stri); \
  IterateArray(type,o,arr,      Write##type(str,o,stri);); \
} while(0)

#define _ReadTAArrayBlock(str,arr,stri,dlen) do { \
  if (dlen!=sizeof((arr).a1[0])) SERROR3("Wrong size (%d != %d)",dlen,sizeof((arr).a1[0])); \
  long ra_ub,ra_lb; \
  Readlong(str,ra_lb,stri); \
  Readlong(str,ra_ub,stri); \
  long ra_n=(ra_ub+1-ra_lb),ra_isz=sizeof(*(arr).a1); \
  if (ra_ub<(arr).lb) { \
    TA_setLowerBound((arr),ra_lb); \
    TA_setUpperBound((arr),ra_ub); \
  } \
  else { \
    TA_setUpperBound((arr),ra_ub); \
    TA_setLowerBound((arr),ra_lb); \
  } \
  if (ra_n<=TinyArrayA1Size) { \
    memcpy((arr).a1,(str).a+stri,ra_n*ra_isz); \
  } \
  else { \
    memcpy((arr).a1,(str).a+stri,TinyArrayA1Size*ra_isz); \
    memcpy((arr).a2,(str).a+(stri+TinyArrayA1Size*ra_isz),(ra_n-TinyArrayA1Size)*ra_isz); \
  } \
  stri+=ra_n*ra_isz; \
} while(0)

#define ReadTAArray(str,type,arr,stri) do { \
  long ra_ub,ra_lb; \
  Readlong(str,ra_lb,stri); \
  Readlong(str,ra_ub,stri); \
  if (ra_ub<(arr).lb) { \
    TA_setLowerBound((arr),ra_lb); \
    TA_setUpperBound((arr),ra_ub); \
  } \
  else { \
    TA_setUpperBound((arr),ra_ub); \
    TA_setLowerBound((arr),ra_lb); \
  } \
  IterateTAArray(type,o,arr,      Read##type(str,o,stri);); \
} while(0)


#define _WriteTAArrayBlock(str,arr,stri,dlen) do { \
  if (dlen!=sizeof((arr).a1[0])) SERROR3("Wrong size (%d != %d)",dlen,sizeof((arr).a1[0])); \
  long ra_lb=(arr).lb,ra_ub=(arr).ubp1-1; \
  Writelong(str,ra_lb,stri); \
  Writelong(str,ra_ub,stri); \
  long ra_n=(ra_ub+1-ra_lb),ra_isz=sizeof(*(arr).a1); \
  (str).ensureLength(stri+ra_n*ra_isz); \
  if (ra_n<=TinyArrayA1Size) { \
    memcpy((str).a+stri,(arr).a1,ra_n*ra_isz); \
  } \
  else { \
    memcpy((str).a+stri,(arr).a1,TinyArrayA1Size*ra_isz); \
    memcpy((str).a+(stri+TinyArrayA1Size*ra_isz),(arr).a2,(ra_n-TinyArrayA1Size)*ra_isz); \
  } \
  stri+=ra_n*ra_isz; \
} while(0)

#define WriteTAArray(str,type,arr,stri) do { \
  long ra_lb=(arr).lb,ra_ub=(arr).ubp1-1; \
  Writelong(str,ra_lb,stri); \
  Writelong(str,ra_ub,stri); \
  IterateTAArray(type,o,arr,      Write##type(str,o,stri);); \
} while(0)


#define ReadString(str,s,stri) do { \
  if (sizeof(char)!=1) SERROR("char should be size 1"); \
  long rs_n;Readlong(str,rs_n,stri); \
  (s).ensureLength(rs_n); \
  memcpy((s).a,(str).a+stri,rs_n); \
  (s).setLength(rs_n); \
  stri+=rs_n; \
} while (0)

#define WriteString(str,s,stri) do { \
  if (sizeof(char)!=1) SERROR("char should be size 1"); \
  long fm_sz=(s).length(); \
  (str).ensureLength(stri+4+fm_sz); \
  Writelong(str,fm_sz,stri); \
  memcpy((str).a+stri,(s).a,fm_sz); \
  stri+=fm_sz; \
} while (0)

#define WriteToFile(fil,str,len) do { \
  g_filSize+=8+(len); \
  if (fil) { \
    char fm_tag[4]="blk"; \
    long fm_len=(len); \
    fwrite(fm_tag,1,4,fil); \
    fwrite(&fm_len,4,1,fil); \
    fwrite((str).a,1,fm_len,fil); \
  } \
} while(0)

#define ReadFromFile(fil,str) do { \
  if (g_readFailed!='y') { \
    char fm_tag[4]="blk",fm_tag2[4]; \
    long fm_len=0; \
    fread(fm_tag2,1,4,fil); \
    if (fm_tag2[3]||strcmp(fm_tag,fm_tag2)) { \
      if (g_readFailed) SERROR("Not a block"); \
      else { \
        prnt("(Not a block)"); \
        g_readFailed='y'; \
        break; \
      } \
    } \
    fread(&fm_len,4,1,fil); \
    (str).setLength(fm_len); \
    fread((str).a,1,fm_len,fil); \
  } \
  else str=""; \
} while(0)



#define WRITEBITS(task,val,nbits,fil,ull,shft) do { \
    dprnt(" \"%s\"+%qd(%d)",task,val,nbits); \
    if ((nbits)>=64-(shft)) { \
        (ull)|=((val)&rightOnes[64-(shft)])<<(shft); \
        fwrite(&(ull),8,1,fil); \
        (ull)=val>>(64-(shft)); \
        (shft)-=64-(nbits); \
    } \
    else { \
        (ull)|=((val&rightOnes[nbits])<<(shft)); \
        (shft)+=(nbits); \
    } \
} while(0)


#define READBITS(task,typ,toval,nbits,pull,shft) do { \
    if ((nbits)>=64-(shft)) { \
        (toval)=(typ)((((*((pull)++))>>(shft))&rightOnes[64-(shft)])|((rightOnes[(nbits)-(64-(shft))]&*(pull))<<(64-(shft)))); \
        (shft)-=64-(nbits); \
    } \
    else { \
        (toval)=(typ)(((*(pull))>>(shft))&rightOnes[nbits]); \
        (shft)+=(nbits); \
    } \
    dprnt(" \"%s\">%qd(%d)",task,toval,nbits); \
} while(0)



#define LERP(a,b,f) ((a)*(1-(f))+(b)*(f))
#define DELERP(a,b,v) (((v)-(a))/((b)-(a)))
#define ANGLE(x,y) (fabs(x)<fabs(y)?((y)>0?((x)>0?atan((x)/(y)):(floatPI*2)+atan((x)/(y))):((y)?floatPI+atan((x)/(y)):0)):((x)>0?(floatPI*0.5f)-atan((y)/(x)):((x)?(floatPI*1.5f)-atan((y)/(x)):0)))


#endif




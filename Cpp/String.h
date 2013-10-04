//>Maker.cpp

#ifndef STRINGH
#define STRINGH

#include "Array.h"


#undef NOTEENABLE
#define NOTEENABLE(a)

template <class I> class StringBuf;

extern StringBuf<char> g_formatBuf;


extern long g_stringPadder;
#ifdef MAINCPPFILE
long g_stringPadder=' ';
#endif

#define STRING_CHECK_DISALLOW_ZEROS if (mayContainZeros) ERROR("Function unsafe for strings that may contain zeros")

template <class I> class StringBuf : public BasicAllocableArray<I>, public RefCnt {
  PubDefCC("sb");
  long _len;

 public:
  bool mayContainZeros;

  long len;// do not change (but will reset to ==_len anyway)

  StringBuf(long initSize=1,bool amayContainZeros=false) : BasicAllocableArray<I>(MakeCC("s."),initSize,NULL) {this->_a[0]=0;len=_len=0;mayContainZeros=amayContainZeros;}
  
  StringBuf(const I *pc) : BasicAllocableArray<I>(MakeCC("s."),1,NULL) {this->_a[0]=0;len=_len=0;mayContainZeros=false;(*this)=pc;}
  
  virtual void destroy() {_DELETE(StringBuf,this);}
  static StringBuf<I> *construct(ContextIndex &ci,StringBuf<I> *nul,WRD typcc) {return(NEW(StringBuf<I>));}

  void ensureLength(long length) {
    if (length+1>this->_aSize) this->setSize(length+1,false,false);
  }

  void setLength(long length) {
    ensureLength(length);
    this->_a[_len=len=length]=0;
  }

  I &operator[](long index) {
    if (index+1>=this->_aSize) this->setSize(index+2);
    if (_len<=index) {
      do {this->_a[_len++]=(I)g_stringPadder;} while (_len<=index);
      len=_len;
      this->_ensurePtr(this->_a+len);
      this->_a[len]=0;
    }
    return(this->_a[index]);
  }

  static long lengthOf(const I *buf) {
    long ret;
    if (sizeof(I)==1) ret=long(strlen((char*)buf));
    else for (ret=0;buf[ret]!=0;ret++);
    return(ret);
  }

  long length() {return(mayContainZeros?_len:(len=_len=lengthOf(this->_a)));}

  void clear() {this->_a[0]=0;len=_len=0;}

  bool operator==(StringBuf<I> &cmp) {return(relationTo(cmp)==0);}

  bool operator==(const I *cmp) {return(relationTo(cmp)==0);}

  bool operator!=(StringBuf<I> &cmp) {return(relationTo(cmp)!=0);}

  bool operator!=(const I *cmp) {return(relationTo(cmp)!=0);}

  bool operator<(StringBuf<I> &cmp) {return(relationTo(cmp)<0);}

  bool operator<(const I *cmp) {return(relationTo(cmp)<0);}

  bool operator>(StringBuf<I> &cmp) {return(relationTo(cmp)>0);}

  bool operator>(const I *cmp) {return(relationTo(cmp)>0);}

  bool startsWith(const I *cmp) {
    STRING_CHECK_DISALLOW_ZEROS;
    long cmpLen=lengthOf(cmp);
    length();
    return((_len>=cmpLen)&&!memcmp(this->_a,cmp,sizeof(I)*cmpLen));
  }

  bool endsWith(const I *cmp) {
    STRING_CHECK_DISALLOW_ZEROS;
    long cmpLen=lengthOf(cmp);
    length();
    return((_len>=cmpLen)&&!memcmp(this->_a+(_len-cmpLen),cmp,sizeof(I)*cmpLen));
  }

  bool contains(const I *cmp) {return(indexOf(cmp)!=-1);}
  long indexOf(const I *cmp) {
    STRING_CHECK_DISALLOW_ZEROS;
    if (sizeof(I)==1) {
      const I *pc=(const I*)strstr((char*)this->_a,(char*)cmp);
      return(pc?long(pc-this->_a):-1);
    }
    else {SERROR("Not supported");return(-1);}
  }

  long relationTo(StringBuf<I> &cmp) {
    STRING_CHECK_DISALLOW_ZEROS;
    length();cmp.length();
    long ret=memcmp(this->_a,cmp._a,sizeof(I)*(_len<cmp._len?_len+1:cmp._len+1));
    if (ret==0) ret=(_len<cmp._len?-1:(_len>cmp._len?1:0));
    return(ret);
  }

  long relationTo(const I *cmp) {
    STRING_CHECK_DISALLOW_ZEROS;
    length();
    long cmpLen=lengthOf(cmp);
    long ret=memcmp(this->_a,cmp,sizeof(I)*(_len<cmpLen?_len:cmpLen));
    if (ret==0) ret=(_len<cmpLen?-1:(_len>cmpLen?1:0));
    return(ret);
  }

  StringBuf<I> &operator=(StringBuf<I> &copy) {
    copy.length();
    len=_len=copy._len;
    this->setSize(_len+1);
    ARRmemcpy(this->_a,copy._a,(_len+1)*sizeof(I));
    return(*this);
  }

  StringBuf<I> &operator=(const I *copy) {
    STRING_CHECK_DISALLOW_ZEROS;
    _len=len=lengthOf(copy);
    this->setSize(_len+1,false,false);
    ARRmemcpy(this->_a,copy,(_len+1)*sizeof(I));
    return(*this);
  }

#define CHANGECHARTYPE(typefrom,from,typeto) (((typeto)(from))==(from)?(typeto)(from):'?')

  StringBuf<I> &copyFrom(const char *copy) {
    STRING_CHECK_DISALLOW_ZEROS;
    _len=len=long(strlen(copy));
    this->setSize(_len+1,false,false);
    I *to;
    for (to=this->_a;*copy;) *(to++)=*(copy++);
    *to=*copy;
    return(*this);
  }

  StringBuf<I> &copyTo(char *to) {
    STRING_CHECK_DISALLOW_ZEROS;
    I *from;
    for (from=this->_a;*from;from++,to++) *to=CHANGECHARTYPE(I,*from,char);
    *to=CHANGECHARTYPE(I,*from,char);
    return(*this);
  }

#ifdef WIN32
#ifdef WCHAR_MAX
  StringBuf<I> &copyFrom(const WCHAR *copy) {
    STRING_CHECK_DISALLOW_ZEROS;
    _len=len=long(wcslen(copy));
    this->setSize(_len+1,false,false);
    I *to;
    for (to=_a;*copy;to++,copy++) *to=CHANGECHARTYPE(WCHAR,*copy,I);
    *to=CHANGECHARTYPE(WCHAR,*copy,I);
    return(*this);
  }

  StringBuf<I> &copyTo(WCHAR *to) {
    STRING_CHECK_DISALLOW_ZEROS;
    I *from;
    for (from=_a;*from;from++,to++) *to=CHANGECHARTYPE(I,*from,WCHAR);
    *to=CHANGECHARTYPE(I,*from,WCHAR);
    return(*this);
  }
#endif
#endif

  StringBuf<I> &memCpy(const I *copy,long ilen) {
    _len=len=ilen;
    this->setSize(_len+1,false,false);
    ARRmemcpy(this->_a,copy,_len*sizeof(I));
    this->_a[_len]=0;
    return(*this);
  }

  StringBuf<I> &memCat(const I *copy,long ilen) {
    if (_len+ilen+1>=this->_aSize) this->setSize(_len+ilen+1);
    ARRmemcpy(this->_a+_len,copy,ilen*sizeof(I));
    _len+=ilen;
    len=_len;
    this->_a[_len]=0;
    return(*this);
  }

  StringBuf<I> &operator+=(StringBuf<I> &add) {
    if (_len+add._len+1>=this->_aSize) this->setSize(_len+add._len+1);
    ARRmemcpy(this->_a+_len,add._a,(add._len+1)*sizeof(I));
    _len+=add._len;
    len=_len;
    return(*this);
  }

  StringBuf<I> &operator+=(const I *add) {
    STRING_CHECK_DISALLOW_ZEROS;
    long addLen=lengthOf(add);
    if (_len+addLen+1>=this->_aSize) this->setSize(_len+addLen+1);
    ARRmemcpy(this->_a+_len,add,(addLen+1)*sizeof(I));
    _len+=addLen;
    len=_len;
    return(*this);
  }

  StringBuf<I> &vformat(const char *strFormat,va_list pArgs) {
    if (sizeof(I)!=1) return(*this);
    g_formatBuf.ensureLength(10000);
    vsprintf(g_formatBuf._a, strFormat, pArgs);
    memCpy(g_formatBuf._a,long(strlen(g_formatBuf.a)));
    return(*this);
  }
  StringBuf<I> &format(const char *strFormat,...) {
    if (sizeof(I)!=1) return(*this);
    g_formatBuf.ensureLength(10000);
    va_list pArgs;
    va_start(pArgs, strFormat);
    vsprintf(g_formatBuf.a, strFormat, pArgs);
    va_end(pArgs);
    copyFrom(g_formatBuf.a);
    return(*this);
  }

  StringBuf<I> &addFormat(const char *strFormat,...) {
    if (sizeof(I)!=1) return(*this);
    g_formatBuf.ensureLength(10000);
    va_list pArgs;
    va_start(pArgs, strFormat);
    vsprintf(g_formatBuf.a, strFormat, pArgs);
    va_end(pArgs);
    copyFrom(g_formatBuf.a);
    return(*this);
  }
};


#ifdef MAINCPPFILE
StringBuf<char> g_formatBuf;
#endif


typedef StringBuf<char> String;
#ifdef WCHAR_MIN
typedef StringBuf<wchar_t> WString;
#endif

typedef TinyArray<char> TAString;

extern void copyTAString(TinyArray<char> &fromarr,String &s);
extern void copyTAString(String &s,TinyArray<char> &toarr);
extern void copyTAString(const char *s,TinyArray<char> &toarr);

extern int strcmpta(TinyArray<char> s1,TinyArray<char> s2);




#ifdef MAINCPPFILE


void copyTAString(TinyArray<char> &fromarr,String &s) {
  long n=TA_num(fromarr);
  s.setLength(n);
  if (n<=TinyArrayA1Size) memcpy(s.a,fromarr.a1,sizeof(char)*n);
  else {
    memcpy(s.a,fromarr.a1,sizeof(char)*TinyArrayA1Size);
    memcpy(s.a+TinyArrayA1Size,fromarr.a2,sizeof(char)*(n-TinyArrayA1Size));
  }
}

void copyTAString(String &s,TinyArray<char> &toarr) {
  long n=s.length();
  TA_setLowerBound(toarr,0);
  TA_setUpperBound(toarr,n-1);
  if (n<=TinyArrayA1Size) memcpy(toarr.a1,s.a,sizeof(char)*n);
  else {
    memcpy(toarr.a1,s.a,sizeof(char)*TinyArrayA1Size);
    memcpy(toarr.a2,s.a+TinyArrayA1Size,sizeof(char)*(n-TinyArrayA1Size));
  }
}

void copyTAString(const char *s,TinyArray<char> &toarr) {
  long n=long(strlen(s));
  TA_setLowerBound(toarr,0);
  TA_setUpperBound(toarr,n-1);
  if (n<=TinyArrayA1Size) memcpy(toarr.a1,s,sizeof(char)*n);
  else {
    memcpy(toarr.a1,s,sizeof(char)*TinyArrayA1Size);
    memcpy(toarr.a2,s+TinyArrayA1Size,sizeof(char)*(n-TinyArrayA1Size));
  }
}

int strcmpta(TinyArray<char> s1,TinyArray<char> s2) {
  if (sizeof(char)!=1) SERROR("weird");
  if (s1.lb<s2.lb) return(-1);
  if (s1.lb>s2.lb) return(-1);
  long sharelen=(s1.ubp1<s2.ubp1?s1.ubp1:s2.ubp1)-s1.lb;
  int ret=memcmp(s1.a1,s2.a1,sharelen>sizeof(s1.a1)?sizeof(s1.a1):sharelen);
  if (ret!=0) return(ret);
  if (sharelen<=sizeof(s1.a1)) return(s1.ubp1>s2.ubp1?1:(s1.ubp1<s2.ubp1?-1:0));
  ret=memcmp(s1.a2,s2.a2,sharelen-sizeof(s1.a1));
  return(ret!=0?ret:(s1.ubp1>s2.ubp1?1:(s1.ubp1<s2.ubp1?-1:0)));
}

#endif











char charFromHexDigit(long h);
long charToHexDigit(char c);

long escapeChar(char c,char *&to);
char unescapeChar(char *&from);
char *escape(const char *s,String &ret);
char *unescape(const char *s,String &to);
char *unescape(const char *s,char *to);



#ifdef MAINCPPFILE

char charFromHexDigit(long h) {return((char)(h<0?'?':(h<10?'0'+h:(h<16?'A'+h-10:'?'))));}
long charToHexDigit(char c) {return((c>='0')&&(c<='9')?c-'0':((c>='a')&&(c<='f')?c+10-'a':((c>='A')&&(c<='F')?c+10-'A':-1)));}

long escapeChar(char c,char *&to) {
  switch(c) {
    case '$':case '&':case '+':case ',':case '/':case ':':case ';':case '=':case '?':
    case '@':case ' ':case '"':case '<':case '>':case '#':case '%':case '{':case '}':
      case '|':case '\\':case '^':case '~':case '[':case ']':case '`':case '\'':
        if (to) {
          to[0]='%';to[1]=charFromHexDigit(c/0x10);to[2]=charFromHexDigit(c%0x10);to+=3;
        }
        return(3);
    default:
      if (to) *(to++)=c;
      return(1);
  }
}
char unescapeChar(const char *&from) {
  switch(*from) {
    case '%':
      if (!(from[1]&&from[2])) {
        from+=(from[1]?2:1);
        return(0);
      }
      from+=3;
      return((char)(charToHexDigit(*(from-2))*0x10+charToHexDigit(*(from-1))));
    case '+':from++;return(' ');
    case 0:return(0);
    default:return(*(from++));
  }
}

char *escape(const char *s,String &ret) {
  long len=0;
  char *to=NULL;
  const char *swas=s;
  for (;*s;len+=escapeChar(*(s++),to));
  ret.ensureLength(len);
  for (to=ret.a,s=swas;*s;len+=escapeChar(*(s++),to));
  *to=0;
  return(ret.a);
}

char *unescape(const char *s,String &to) {
  to.ensureLength(long(strlen(s)));
  return(unescape(s,to.a));
}

char *unescape(const char *s,char *to) {
  char *to2;
  const char *from;
  for (to2=to,from=s;*from;*(to2++)=unescapeChar(from));
  *to2=0;
  return(to);
}

#endif






#endif

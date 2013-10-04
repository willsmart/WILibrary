//>Maker.cpp


#ifndef BASICCONVERTH
#define BASICCONVERTH


#undef NOTEENABLE
#define NOTEENABLE(a)


#define ISNUMERICCHAR(c) (((c)>='0')&&((c)<='9'))
#define ISHEXCHAR(c) ((((c)>='0')&&((c)<='9'))||(((c)>='a')&&((c)<='f'))||(((c)>='A')&&((c)<='F')))
#define VALUEOFHEXCHAR(c) (ISHEXCHAR(c)?(((c)>='0')&&((c)<='9')?c-'0':(c+10-(((c)>='a')&&((c)<='f')?'a':'A'))):-1)
#define ISALPHACHAR(c) ((((c)>='a')&&((c)<='z'))||(((c)>='A')&&((c)<='Z')))
#define ISALPHANUMCHAR(c) (ISNUMERICCHAR(c)||ISALPHACHAR(c))
#define ISWHITECHAR(c) (((c)==' ')||((c)=='\t')||((c)=='\r')||((c)=='\n'))


char readNumber(const char *&pc,long *pl,long long *pll,double *pd);
bool readNumber(const char *&pc,long &l);
bool readNumber(const char *&pc,long long &ll);
bool readNumber(const char *&pc,double &d);
void writeInteger(char *&pc,long long ll);
void writeIntegerAsHex(char *&pc,long long ll,long digits);



#ifdef MAINCPPFILE

char readNumber(const char *&pc,long *pl,long long *pll,double *pd) {
  long long fst;
    long negMul=1;
  bool isFlt=false,isLong;
  long exp=0,decPlc=0;
  const char *pcWas=pc;

  if ((*pc=='0')&&(pc[1]=='x')) {
    if (asciiToHex[(BYT)pc[2]]<0) return(0);
    pc+=2;
    for (fst=0;asciiToHex[(BYT)*pc]>=0;pc++) fst=(fst<<4)+(asciiToHex[(BYT)*pc]);
  }
  else {
    if (*pc=='-') {negMul=-1;pc++;}
    if (!((*pc=='.')||ISNUMERICCHAR(*pc))) {pc=pcWas;return(0);}

    for (fst=0;ISNUMERICCHAR(*pc);pc++) fst=(fst*10)+(*pc-'0');

    if ((*pc=='.')&&ISNUMERICCHAR(pc[1])) {
      isFlt=true;
      pc++;
      for (;ISNUMERICCHAR(*pc);pc++,decPlc++) fst=(fst*10)+(*pc-'0');
    }
    fst*=negMul;
    /*    
    if ((*pc=='e')||(*pc=='E')) {
      isFlt=true;
      pc++;
      if (*pc=='-') {expNegMul=-1;pc++;}
      if (!ISNUMERICCHAR(*pc)) {pc=pcWas;return(0);}
      for (exp=0;ISNUMERICCHAR(*pc);pc++) exp=(exp*10)+(*pc-'0');
      exp*=expNegMul;
    } 
    */   
  }
  isLong=(((fst&0xffffffff80000000LL)==0)||((fst&0xffffffff80000000LL)==0xffffffff80000000LL));
  if (pl&&isLong) *pl=long(fst);
  if (pll) *pll=fst;
  if (pd) *pd=double(fst)*pow(10.0,exp-decPlc);
  return(isFlt?'f':(isLong?'l':'L'));
}

bool readNumber(const char *&pc,long &l) {
  const char *pcWas=pc;
  if (readNumber(pc,&l,NULL,NULL)!='l') {pc=pcWas;return(false);}
  return(true);
}

bool readNumber(const char *&pc,long long &ll) {
  const char *pcWas=pc;
  if ((readNumber(pc,NULL,&ll,NULL)&~('a'-'A'))!='L') {pc=pcWas;return(false);}
  return(true);
}

bool readNumber(const char *&pc,double &d) {
  const char *pcWas=pc;
  if (readNumber(pc,NULL,NULL,&d)==0) {pc=pcWas;return(false);}
  return(true);
}

void writeInteger(char *&pc,long long ll) {
  long long ll2=ll;
  if (ll2<0) {
    *(pc++)='-';
    ll2=-ll2;
  }
  long long shftOne;
  long cnt;
  for (shftOne=1,cnt=0;ll2/10>=shftOne;shftOne*=10,cnt++);
  pc[cnt+1]=0;
  for (;cnt>=0;cnt--) {
    pc[cnt]=char((ll2%10)+'0');
    ll2/=10;
  }
}

void writeIntegerAsHex(char *&pc,long long ll,long digits) {
  unsigned long long ull=(unsigned long long)ll,ull2;
  *(pc++)='0';*(pc++)='x';
  if (digits==-1) {
    for (ull2=ull,digits=0;ull2;digits++,ull2>>=4);
    if (!digits) digits=1;
  }
  pc[digits]=0;
  for (digits--;digits>=0;digits--,ull>>=4) pc[digits]=char(ull&0xf)+((ull&0xf)<10?'0':('a'-10));
}

#endif

#endif


//>Maker.cpp

#pragma once


//#include "Schema.h"

typedef TinyArray<ULL> BigUInt;



class BigUIntFunctions {
public:
  static BigUInt zero() {BigUInt ret;memset(&ret,0,sizeof(ret));return(ret);}
  static BigUInt one() {BigUInt ret;memset(&ret,0,sizeof(ret));ret.a1[0]=1;ret.ubp1=1;return(ret);}
  static BigUInt fromLong(long i) {if (i<0) SERROR("negative");return(fromULL((ULL)i));}
  static BigUInt fromULL(ULL ull) {BigUInt v;memset(&v,0,sizeof(v));v.a1[0]=ull;v.ubp1=1;return(v);}
  static BigUInt copy(BigUInt &bui) {BigUInt v;TA_init(v);copyTAArray(bui,v);return(v);}

  static BigUInt &addInplace(BigUInt &a,BigUInt &b) {
    if (b.ubp1>a.ubp1) TA_setUpperBound(a,b.ubp1-1);
    ULL carry=0,o,r2;
    IterateTAArray(ULL,ull,a,
      if (ullIndex>=b.ubp1) {
        if (!carry) break;
        o=0;
      }
      else o=TA_A(b,ullIndex);
      r2=ull+carry;
      carry=(r2<ull?1:0);
      ull=r2;
      r2=ull+o;
      carry+=(r2<ull?1:0);
      ull=r2;
    );
    if (carry) {
      TA_setUpperBound(a,a.ubp1);
      TA_A(a,a.ubp1-1)=carry;
    }
    return(a);
  }

  static BigUInt &addInplace(BigUInt &a,long b) {
    if (a.ubp1==0) a=fromLong(b);
    else {
      ULL carry=0,o,r2;
      IterateTAArray(ULL,ull,a,
        o=(ullIndex?0:(ULL)b);
        r2=ull+carry;
        carry=(r2<ull?1:0);
        ull=r2;
        r2=ull+o;
        carry+=(r2<ull?1:0);
        ull=r2;
      );
      if (carry) {
        TA_setUpperBound(a,a.ubp1);
        TA_A(a,a.ubp1-1)=carry;
      }
    }
    return(a);
  }

  static BigUInt &multiplyInplace(BigUInt &a,BigUInt &b) {BigUInt ret=multiply(a,b);TA_clear(a);a=ret;return(a);}
  static BigUInt &divideInplace(BigUInt &q,BigUInt &d) {BigUInt ret=divide(q,d);TA_clear(q);q=ret;return(q);}
  static BigUInt subtract(BigUInt &a,BigUInt &b) {BigUInt ret;TA_init(ret);copyTAArray(a,ret);subtractInplace(ret,b);return(ret);}
  static BigUInt add(BigUInt &a,BigUInt &b) {BigUInt ret;TA_init(ret);copyTAArray(a,ret);addInplace(ret,b);return(ret);}

  static void shiftLeftInplace(BigUInt &v) {
    if (!TA_num(v)) return;
    if (TA_A(v,v.ubp1-1)&0x8000000000000000LL) TA_setUpperBound(v,v.ubp1);
    ULL *pv=NULL;
    for (long i=v.ubp1-1;i>=v.lb;i--) {
      ULL &vv=TA_A(v,i);
      if (pv&&(vv&0x8000000000000000LL)) (*pv)|=1;
      vv<<=1;
      pv=&vv;
    }
  }
  static void shiftRightInplace(BigUInt &v,bool errorOnUnderflow=false) {
    if (!TA_num(v)) return;
    if (errorOnUnderflow&&(v.lb==0)&&(TA_A(v,0)&1)) SERROR("Underflow");
    if ((v.lb>0)&&(TA_A(v,v.lb)&1)) TA_setLowerBound(v,v.lb-1);
    ULL *pv=NULL;
    for (long i=v.lb;i<v.ubp1;i++) {
      ULL &vv=TA_A(v,i);
      if (pv&&(vv&1)) (*pv)|=0x8000000000000000LL;
      vv>>=1;
      pv=&vv;
    }
  }

  static BigUInt multiply(BigUInt &a,BigUInt &b) {
    BigUInt ret=zero(),bb=zero();copyTAArray(b,bb);
    //ULL bit=1;
    for (long i=0;i<a.ubp1;i++) for (ULL bit=1,av=TA_A(a,i);bit&&((i<a.ubp1-1)||(bit<=av));bit<<=1) {
      if (av&bit) addInplace(ret,bb);
      shiftLeftInplace(bb);
    }
//    if (bb.a2) prnt("buia1.");
    TA_clear(bb);
    return(ret);
  }

  static BigUInt &subtractInplace(BigUInt &a,BigUInt &minus) {
    clipInplace(minus);
    clipInplace(a);
    if (minus.ubp1>a.ubp1) SERROR("Would be negative");
    if (minus.ubp1==0) return(a);
    else if (a.ubp1==1) {
      if (minus.a1[0]>a.a1[0]) SERROR("Would be negative");
      else a.a1[0]-=minus.a1[0];
    }
    else if (a.ubp1<=4) {
      if (minus.ubp1==1) {
        if (minus.a1[0]<=a.a1[0]) a.a1[0]-=minus.a1[0];
        else {
          a.a1[0]-=minus.a1[0];
          for (long i=1;!(a.a1[i]--);i++);
        }
      }
      else {
        if ((minus.ubp1==a.ubp1)&&isLess(a,minus)) SERROR("Would be negative");
        ULL borrow=0,nborrow;
        for (long i=0;i<minus.ubp1;i++) {
          ULL &aa=a.a1[i],&mm=minus.a1[i];
          nborrow=0;
          if (borrow) {
            if (aa<borrow) nborrow++;
            aa-=borrow;
          }
          if (aa<mm) nborrow++;
          aa-=mm;
          borrow=nborrow;
        }
        if (borrow) for (long i=minus.ubp1;!(a.a1[i]--);i++);
      }
    }
    else {
      if (minus.ubp1==1) {
        if (minus.a1[0]<=a.a1[0]) a.a1[0]-=minus.a1[0];
        else {
          a.a1[0]-=minus.a1[0];
          for (long i=1;!(TA_A(a,i)--);i++);
        }
      }
      else {
        if ((minus.ubp1==a.ubp1)&&isLess(a,minus)) SERROR("Would be negative");
        ULL borrow=0,nborrow;
        for (long i=0;i<minus.ubp1;i++) {
          ULL &aa=TA_A(a,i),&mm=TA_A(minus,i);
          nborrow=0;
          if (borrow) {
            if (aa<borrow) nborrow++;
            aa-=borrow;
          }
          if (aa<mm) nborrow++;
          aa-=mm;
          borrow=nborrow;
        }
        if (borrow) for (long i=minus.ubp1;!(TA_A(a,i)--);i++);
      }
    }
    clipInplace(a);

    return(a);
  }


  static BigUInt divide(BigUInt &aq,BigUInt &d) {
    BigUInt ret=zero(),dd=zero(),q;
    TA_init(q);copyTAArray(aq,q);
    if (isEqual(d,ret)) SERROR("Divide by zero");
    if (q.ubp1<d.ubp1) return(ret);
    if (q.ubp1==1) {ret.a1[0]=q.a1[0]/d.a1[0];ret.ubp1=1;return(ret);}
    if (isLess(q,d)) return(ret);
    copyTAArray(d,dd);
    long shft;for (shft=0;isLess(dd,q);shft++,shiftLeftInplace(dd));
    TA_setUpperBound(ret,shft/64);
    for (long i=ret.ubp1-1;i>=0;i--) {
      ULL &v=TA_A(ret,i);
      for (ULL bit=shiftOneULL[i==ret.ubp1-1?(shft&63):63];bit;bit>>=1,shft--) {
        if (isGreaterOrEqual(q,dd)) {
          v|=bit;
          subtractInplace(q,dd);
        }
        shiftRightInplace(dd);
      }
    }
//    if (dd.a2) prnt("buia2.");
    TA_clear(dd);
    clipInplace(ret);

    return(ret);
  }

  static bool isLess(BigUInt &a,BigUInt &b) {
    for (long i=b.ubp1-1;i>=a.ubp1;i--) if (TA_A(b,i)) return(true);
    for (long i=a.ubp1-1;i>=b.ubp1;i--) if (TA_A(a,i)) return(false);
    for (long i=(a.ubp1<b.ubp1?a.ubp1:b.ubp1)-1;i>=0;i--) {
      ULL &aa=TA_A(a,i),&bb=TA_A(b,i);
      if (aa>bb) return(false);
      if (aa<bb) return(true);
    }
    return(false);
  }
  static bool isLessOrEqual(BigUInt &a,BigUInt &b) {
    for (long i=b.ubp1-1;i>=a.ubp1;i--) if (TA_A(b,i)) return(true);
    for (long i=a.ubp1-1;i>=b.ubp1;i--) if (TA_A(a,i)) return(false);
    for (long i=(a.ubp1<b.ubp1?a.ubp1:b.ubp1)-1;i>=0;i--) {
      ULL &aa=TA_A(a,i),&bb=TA_A(b,i);
      if (aa>bb) return(false);
      if (aa<bb) return(true);
    }
    return(true);
  }
  static bool isGreater(BigUInt &a,BigUInt &b) {return(isLess(b,a));}
  static bool isGreaterOrEqual(BigUInt &a,BigUInt &b) {return(isLessOrEqual(b,a));}
  static bool isEqual(BigUInt &a,BigUInt &b) {
    for (long i=b.ubp1-1;i>=a.ubp1;i--) if (TA_A(b,i)) return(false);
    for (long i=a.ubp1-1;i>=b.ubp1;i--) if (TA_A(a,i)) return(false);
    for (long i=(a.ubp1<b.ubp1?a.ubp1:b.ubp1)-1;i>=0;i--) {
        ULL v=TA_A(a,i);
      if (v!=TA_A(b,i)) return(false);
    }
    return(true);
  }
  static long double asLongDouble(BigUInt &a) {
    long double ret=0;
    int p=0;
    for (long i=0;i<a.ubp1;i++) for (ULL bit=1,va=TA_A(a,i);bit;bit<<=1,p++) if (va&bit) ret+=pow((long double)2,p);
    return(ret);
  }
  static double asDouble(BigUInt &a) {
    double ret=0;
    int p=0;
    for (long i=0;i<a.ubp1;i++) for (ULL bit=1,va=TA_A(a,i);bit;bit<<=1,p++) if (va&bit) ret+=pow(2.0,p);
    return(ret);
  }

  static void clipInplace(BigUInt &v) {
    long i;
    for (i=v.ubp1-1;i>=0;i--) if (TA_A(v,i)) break;
    if (i!=v.ubp1-1) TA_setUpperBound(v,i);
  }



  static BigUInt perm(BigUInt &n,long k) {
//    prnt("perm %d from %f\n",k,asDouble(n));
    if (k==0) return(one());
    BigUInt kbui=fromLong(k),onebui=one();
    if ((k<0)||(isGreater(kbui,n))) return(zero());
    BigUInt nn=zero(),ret=zero();
    copyTAArray(n,nn);
    copyTAArray(n,ret);
    for (;k>1;k--) {
      subtractInplace(nn,onebui);
      multiplyInplace(ret,nn);
    }
    //if (nn.a2) prnt("buia3.");
    TA_clear(nn);
    return(ret);
  }

  static BigUInt choose(BigUInt &n,long k) {
//    prnt("choose %d from %f\n",k,asDouble(n));
    if (k==0) return(one());
    BigUInt kbui=fromLong(k);
    if ((k<0)||(isGreater(kbui,n))) return(zero());
    BigUInt ret=perm(n,k),kf=kbui;
    for (k--;k>1;k--) {kbui=fromLong(k);multiplyInplace(kf,kbui);}
    divideInplace(ret,kf);
    //if (kf.a2) prnt("buia4.");
    TA_clear(kf);
    return(ret);
  }



  static char *decimalStr(BigUInt &bui,String &ret) {
    BigUInt m=fromLong(10),ten=fromLong(10);
    long digits=1;while (isGreaterOrEqual(bui,m)) {multiplyInplace(m,ten);digits++;}
    TA_clear(m);
    ret.ensureLength(digits);
    BigUInt d=zero();copyTAArray(bui,d);
    for (long i=digits-1;i>=0;i--) {
      BigUInt s=zero();copyTAArray(d,s);
      divideInplace(d,ten);
      BigUInt m=multiply(d,ten);
      subtractInplace(s,m);
      if (isGreaterOrEqual(s,ten)) SERROR("Should be less than ten");
      ret.a[i]=char(s.a1[0]+'0');
      TA_clear(s);
      TA_clear(m);
    }
    TA_clear(d);
    ret.setLength(digits);
    return(ret.a);
  }




  static char *decimalStr(BigUInt &bui,String &ret,BigUInt &decimalPoint) {
//    String s,s2;decimalStr(bui,s);s+=" @ ";s+=decimalStr(decimalPoint,s2);prnt("\n%s\n",s.a);

    BigUInt ten=fromLong(10),d=divide(bui,decimalPoint);
//      decimalStr(d,s);prnt("\n%s\n",s.a);
    decimalStr(d,ret);
    multiplyInplace(d,decimalPoint);
    BigUInt f=subtract(bui,d);
    TA_clear(d);
    bool doneDot=false;
    while (true) {
//      decimalStr(f,s);prnt("\nf0 : %s\n",s.a);
      clipInplace(f);
      if (!f.ubp1) break;
      multiplyInplace(f,ten);
//      decimalStr(f,s);prnt("fm : %s\n",s.a);
      BigUInt ff=divide(f,decimalPoint);
//      decimalStr(f,s);prnt("f  : %s\n",s.a);
//      decimalStr(ff,s);prnt("fdd: %s\n",s.a);
      clipInplace(ff);
      char c=0;
      if (ff.ubp1==0) c='0';
      else if ((ff.ubp1>1)||(ff.a1[0]>=10)) SERROR("Should be < 10");
      else c=char(ff.a1[0]+'0');
      if (!doneDot) {ret+=".";doneDot=true;}
      ret.addFormat("%c",c);
//      decimalStr(f,s);prnt("f  : %s\n",s.a);
      multiplyInplace(ff,decimalPoint);
//      decimalStr(f,s);prnt("f  : %s\n",s.a);
//      decimalStr(ff,s);prnt("fmd: %s\n",s.a);
      subtractInplace(f,ff);
//      decimalStr(f,s);prnt("fo :%s\n",s.a);
      TA_clear(ff);
    }
    TA_clear(f);
    return(ret.a);
  }

};










template<class T> T quickSelectCBigUInt(Array<T> &arr,double pcnt,Array<T> &tmparr) {
  if (pcnt<=0) {T minv=arr A[0];IterateArray(T,v,arr,  if (v.v<minv.v) minv=v;);return(minv);}
  if (pcnt>=100) {T maxv=arr A[0];IterateArray(T,v,arr,  if (v.v>maxv.v) maxv=v;);return(maxv);}

  if (&tmparr!=&arr) tmparr=arr;
  T* a=tmparr.a;
  long left=0,right=arr.num()-1;//,ub=right;
  BigUInt totc=BigUIntFunctions::fromULL(0),storec=BigUIntFunctions::fromULL(0);
  IterateArray(T,v,tmparr,   BigUIntFunctions::addInplace(totc,v.c););
  BigUInt leftc=BigUIntFunctions::fromULL(0),rightc=BigUIntFunctions::copy(totc),aimc=BigUIntFunctions::copy(totc);
  BigUInt pcbui=BigUIntFunctions::fromULL((ULL)(pcnt*1000));
  BigUInt hundthoubui=BigUIntFunctions::fromULL((ULL)(100000));
  BigUIntFunctions::multiplyInplace(aimc,pcbui);
  BigUIntFunctions::divideInplace(aimc,hundthoubui);
  BigUIntFunctions::clipInplace(totc);
  if (!totc.ubp1) SERROR("No counts!");

  String s;
  while (true) {
    prnt("[%d]%s   <=   ",     left,BigUIntFunctions::decimalStr(leftc,s));
    prnt("%s    <    ",     BigUIntFunctions::decimalStr(aimc,s));
    prnt("[%d]%s\n",     right,BigUIntFunctions::decimalStr(rightc,s));

    if (left>right) SERROR("QuickSelectC Error, left>right");
    if (left==right) return(a[left]);
    if (BigUIntFunctions::isLess(aimc,leftc)||BigUIntFunctions::isGreaterOrEqual(aimc,rightc)) SERROR("QuickSelectC Error, aimc<leftc or aimc>=rightc");

    long pivot=RandInt(right+1-left)+left;

    T pv=a[pivot];
    a[pivot]=a[right];
    long storeIndex=left;
    copyTAArray(leftc,storec);
    for (long i=left;i<right;i++) {
      if (a[i].v<pv.v) {
        BigUIntFunctions::addInplace(storec,a[i].c);
        if (i==storeIndex) storeIndex++;
        else {T tmp=a[i];a[i]=a[storeIndex];a[storeIndex++]=tmp;}
      }
    }
    if (storeIndex!=right) a[right]=a[storeIndex];
    a[storeIndex]=pv;

    if (BigUIntFunctions::isLess(aimc,storec)) {right=storeIndex-1;copyTAArray(storec,rightc);}
    else {
      BigUIntFunctions::addInplace(storec,pv.c);
      if (BigUIntFunctions::isGreaterOrEqual(aimc,storec)) {
        left=storeIndex+1;
        copyTAArray(storec,leftc);
      }
      else {
        TA_clear(totc);
        TA_clear(storec);
        TA_clear(leftc);
        TA_clear(rightc);
        TA_clear(aimc);
        return(pv);
      }
    }
  }
}












typedef TinyArray<BigUInt> SchemaCount;

void clearSC(SchemaCount &sc);
void copySC(SchemaCount &from,SchemaCount &to);

#ifdef MAINCPPFILE
void clearSC(SchemaCount &sc) {
  IterateTAArray(BigUInt,bui,
    sc,
    //if (bui.a2) prnt("buia6.");
    TA_clear(bui);
  );
  TA_clear(sc);
}

void copySC(SchemaCount &from,SchemaCount &to) {
  clearSC(to);
  TA_setLowerBound(to,0);
  TA_setUpperBound(to,from.ubp1-1);
  TA_setLowerBound(to,from.lb);
  for (long i=from.lb;i<from.ubp1;i++) {
    BigUInt bui=TA_A(from,i);
    copyTAArray(bui,TA_A(to,i));
  }
}
#endif


#define InterimSchemaCountSize 1024
class InterimSchemaCount : public RefCnt {
  PubDefCC("sc");
public:
  BigUInt a[InterimSchemaCountSize];
  long lb,ubp1;
  InterimSchemaCount() {lb=ubp1=0;memset(a,0,sizeof(a[0])*InterimSchemaCountSize);}
  static InterimSchemaCount *construct(ContextIndex &ci,InterimSchemaCount *nul,WRD typcc) {return(NEW(InterimSchemaCount));}
  virtual void destroy() {clear();_DELETE(InterimSchemaCount,this);}
  void clear() {
    for (long i=lb;i<ubp1;i++) {
      //if (a[i].a2) prnt("buia1.");
      TA_clear(a[i]);
    }
    lb=ubp1=0;
  }
  long num() {return(ubp1-lb);}
  long upperBound() {return(ubp1-1);}
  long lowerBound() {return(lb);}
  BigUInt &operator[](long i) {return(a[i]);}

  void zero() {clear();}
  void setAsOne(long size) {clear();lb=size;ubp1=size+1;a[size]=BigUIntFunctions::one();}
  void clip() {
    for (long i=lb;i<ubp1;i++) BigUIntFunctions::clipInplace(a[i]);
    while ((lb<ubp1)&&!a[lb].ubp1) lb++;
    while ((lb<ubp1)&&!a[ubp1-1].ubp1) ubp1--;
    if (lb==ubp1) lb=ubp1=0;
  }
  void copyTo(SchemaCount &sc) {
    clearSC(sc);
    clip();
    TA_setLowerBound(sc,0);
    TA_setUpperBound(sc,ubp1-1);
    TA_setLowerBound(sc,lb);
    for (long i=lb;i<ubp1;i++) copyTAArray(a[i],TA_A(sc,i));
  }
  void copyFrom(SchemaCount &sc) {
    setBounds(sc.lb,sc.ubp1-1);
    for (long i=lb;i<ubp1;i++) copyTAArray(TA_A(sc,i),a[i]);
  }
  void copyFrom(InterimSchemaCount &sc) {
    setBounds(sc.lb,sc.ubp1-1);
    for (long i=lb;i<ubp1;i++) copyTAArray(sc.a[i],a[i]);
  }


  void setBounds(long alb,long aub) {
    if (aub<alb-1) SERROR("Bad bounds");
    if (alb<lb) lb=alb;
    if (aub>=ubp1) ubp1=aub+1;
    while (lb<alb) {TA_clear(a[lb]);lb++;}
    while (ubp1-1>aub) {ubp1--;TA_clear(a[ubp1]);}
  }
  void ensureBounds(long alb,long aub) {
    if (aub<alb-1) SERROR("Bad bounds");
    if (alb<lb) lb=alb;
    if (aub>=ubp1) ubp1=aub+1;
  }


  void add(InterimSchemaCount &sc) {
    for (long i=(lb<sc.lb?sc.lb:lb),u=(ubp1>sc.ubp1?sc.ubp1:ubp1);i<u;i++) BigUIntFunctions::addInplace(a[i],sc.a[i]);
    while (lb>sc.ubp1) lb--;
    while (lb>sc.lb) {lb--;copyTAArray(sc.a[lb],a[lb]);}
    while (ubp1<sc.lb) ubp1++;
    while (ubp1<sc.ubp1) {copyTAArray(sc.a[ubp1],a[ubp1]);ubp1++;}
  }
  void add(SchemaCount &sc) {
    for (long i=(lb<sc.lb?sc.lb:lb),u=(ubp1>sc.ubp1?sc.ubp1:ubp1);i<u;i++) BigUIntFunctions::addInplace(a[i],TA_A(sc,i));
    while (lb>sc.ubp1) lb--;
    while (lb>sc.lb) {lb--;copyTAArray(TA_A(sc,lb),a[lb]);}
    while (ubp1<sc.lb) ubp1++;
    while (ubp1<sc.ubp1) {copyTAArray(TA_A(sc,ubp1),a[ubp1]);ubp1++;}
  }

  void addOne(long size) {
    if (size<lb) {lb=size;a[size]=BigUIntFunctions::one();}
    else if (size>=ubp1) {ubp1=size+1;a[size]=BigUIntFunctions::one();}
    else {
      BigUInt v=BigUIntFunctions::one();
      BigUIntFunctions::addInplace(a[size],v);
    }
  }

  void subtract(InterimSchemaCount &sc) {
    clip();
    sc.clip();
    if (ubp1<sc.ubp1) SERROR("Would be negative");
    if (lb>sc.lb) lb=sc.lb;
    for (long i=sc.lb;i<sc.ubp1;i++) BigUIntFunctions::subtractInplace(a[i],sc.a[i]);
  }

  void subtract(SchemaCount &sc) {
    clip();
    if (ubp1<sc.ubp1) SERROR("Would be negative");
    if (lb>sc.lb) lb=sc.lb;
    for (long i=sc.lb;i<sc.ubp1;i++) BigUIntFunctions::subtractInplace(a[i],TA_A(sc,i));
  }

  void multiply(InterimSchemaCount &sca,InterimSchemaCount &scb) {
    clear();
    lb=sca.lb+scb.lb;
    ubp1=sca.ubp1+scb.ubp1-1;
    for (long i=sca.lb,j;i<sca.ubp1;i++) for (j=scb.lb;j<scb.ubp1;j++) {
      BigUInt m=BigUIntFunctions::multiply(sca.a[i],scb.a[j]);
      BigUIntFunctions::addInplace(a[i+j],m);
      TA_clear(m);
    }
  }
  void multiply(InterimSchemaCount &sc,BigUInt &by,long bySize) {
    clear();
    lb=sc.lb+bySize;
    ubp1=sc.ubp1+bySize;
    for (long i=sc.lb;i<sc.ubp1;i++) a[i+bySize]=BigUIntFunctions::multiply(sc.a[i],by);
  }
  void multiply(InterimSchemaCount &sc,long by,long bySize) {
    BigUInt biby=BigUIntFunctions::fromLong(by);
    multiply(sc,biby,bySize);
  }
  void multiply(long by,long bySize) {
    BigUInt biby=BigUIntFunctions::fromLong(by);
    multiply(biby,bySize);
  }
  void multiply(BigUInt &by,long bySize) {
    for (long i=ubp1-1;i>=lb;i--) {
      BigUIntFunctions::multiplyInplace(a[i],by);
      if (bySize) {a[i+bySize]=a[i];TA_init(a[i]);}
    }
    lb+=bySize;
    ubp1+=bySize;
  }
  void multiplyBySize(long bySize) {
    for (long i=ubp1-1;i>=lb;i--) if (bySize) {a[i+bySize]=a[i];TA_init(a[i]);}
    lb+=bySize;
    ubp1+=bySize;
  }
  static InterimSchemaCount tmpmulsc;
  void multiply(InterimSchemaCount &sc) {
    tmpmulsc.copyFrom(*this);
    multiply(tmpmulsc,sc);
  }

  void shiftRight() {
    for (long i=lb;i<ubp1;i++) BigUIntFunctions::shiftRightInplace(a[i]);
    clip();
  }
  void doubleSize() {
    for (long i=ubp1-1;i>=lb;i--) if (i) {a[i*2]=a[i];a[i]=BigUIntFunctions::zero();}
    lb*=2;if (ubp1) ubp1=ubp1*2-1;
    clip();
  }

  static void prntSC(SchemaCount &sc,const char *prompt="Counts [%d %d] : ",const char *after="\n") {
    prnt(prompt,sc.lb,sc.ubp1-1);
    bool fst=true;
    IterateTAArray(BigUInt,bui,sc,
		   prnt("%s[%d]x%g",fst?"":", ",buiIndex,BigUIntFunctions::asDouble(bui));fst=false;
    );
    if (fst) prnt("empty");
    prnt(after);
  }
  void prntSC(const char *prompt="Counts [%d %d] : ",const char *after="\n") {
    prnt(prompt,lb,ubp1-1);
    bool fst=true;
    for (long i=lb;i<ubp1;i++) {
      prnt("%s[%d]x%g",fst?"":", ",i,BigUIntFunctions::asDouble(a[i]));fst=false;
      //prnt(" (%d %d [%d %d %d %d] %p)",a[i].lb,a[i].ubp1,a[i].a1[0],a[i].a1[1],a[i].a1[2],a[i].a1[3],a[i].a2);
    }
    if (fst) prnt("empty");
    prnt(after);
  }

  void write(String &s,long &stri) {
    clip();
    Writelong(s,lb,stri);
    Writelong(s,ubp1,stri);
    for (long i=lb;i<ubp1;i++) WriteTAArray(s,ULL,a[i],stri);
  }

  void read(String &s,long &stri) {
    clear();
    Readlong(s,lb,stri);
    Readlong(s,ubp1,stri);
    for (long i=lb;i<ubp1;i++) ReadTAArray(s,ULL,a[i],stri);
  }



  void perm(long n) {
    zero();
    lb=0;ubp1=n+1;
    BigUInt nbui=BigUIntFunctions::fromLong(n);
    for (long size=lb;size<ubp1;size++) a[size]=BigUIntFunctions::perm(nbui,size);
  }

  void choose(long n) {
    zero();
    lb=0;ubp1=n+1;
    BigUInt nbui=BigUIntFunctions::fromLong(n);
    for (long size=lb;size<ubp1;size++) a[size]=BigUIntFunctions::choose(nbui,size);
  }


  void choose(InterimSchemaCount &sc,long r) {
    sc.clip();
    zero();
    if (!sc.ubp1) return;
    lb=sc.lb*r;ubp1=(sc.ubp1-1)*r+1;
    for (long size=lb;size<ubp1;size++) a[size]=sc.choose(size,r);
    clip();
  }

  BigUInt choose(long totSize,long r,long prevsz=-1,long accumSize=0) {
    long maxFutureTotSize=0,minFutureTotSize=0,maxSize=0,minSize=0,ii,i,size;
    BigUInt ret=BigUIntFunctions::zero();
    if (r<=0) SERROR("Bad arg");
    else if (r==1) {
      size=totSize-accumSize;
      if ((size>=lb)&&(size<ubp1)) copyTAArray(a[size],ret);
    }
    else {
      for (i=ubp1-1,ii=0;(i>=lb)&&(ii<r-1);i--) if (a[i].ubp1) {maxFutureTotSize+=i;ii++;}
      for (;(i>=lb)&&(ii<r);i--) if (a[i].ubp1) {maxSize=i;ii++;}
      if (ii<r) return(BigUIntFunctions::zero());
      for (i=prevsz+1,ii=0;!ii;i++) if (a[i].ubp1) {minSize=i;ii++;}
      for (;ii<r;i++) if (a[i].ubp1) {minFutureTotSize+=i;ii++;}

      if (minSize<totSize-accumSize-maxFutureTotSize) minSize=totSize-accumSize-maxFutureTotSize;
      if (maxSize>totSize-accumSize-minFutureTotSize) maxSize=totSize-accumSize-minFutureTotSize;

      for (size=minSize;size<=maxSize;size++) {
        BigUInt future=choose(totSize,r-1,size,accumSize+size);
        BigUInt futAndMe=BigUIntFunctions::multiply(future,a[size]);
        BigUIntFunctions::addInplace(ret,futAndMe);
        TA_clear(future);
        TA_clear(futAndMe);

        for (long reps=2;reps<=r;reps++) {
          maxFutureTotSize=minFutureTotSize=0;
          for (i=ubp1-1,ii=0;(i>=lb)&&(ii<r-reps);i--) if (a[i].ubp1) {maxFutureTotSize+=i;ii++;}
          for (i=size+1,ii=0;(i<ubp1)&&(ii<r-reps);i--) if (a[i].ubp1) {minFutureTotSize+=i;ii++;}

          if ((size*reps>=totSize-accumSize-maxFutureTotSize)&&(size*reps<=totSize-accumSize-minFutureTotSize)) {
            if (reps<r) future=choose(totSize,r-reps,size,accumSize+size*reps);
            else future=BigUIntFunctions::one();
            BigUInt me=BigUIntFunctions::choose(a[size],reps);
            futAndMe=BigUIntFunctions::multiply(future,me);
            BigUIntFunctions::addInplace(ret,futAndMe);
            TA_clear(future);
            TA_clear(me);
            TA_clear(futAndMe);
          }
        }
      }
    }
    return(ret);
  }


 









  static void getNonDescendingCombosfake(InterimSchemaCount &c,long maxReps,Array<InterimSchemaCount*> &repcs,Array<Array<InterimSchemaCount*>*> &tmpaaisc,ClassStore<InterimSchemaCount> &store,ArrayStore<InterimSchemaCount*> &arrstore) {
    if (c.ubp1==c.lb) SERROR("Empty count");
    if (tmpaaisc.num()) SERROR("Should be cleared");

    repcs.setUpperBound(maxReps-1);
    IterateArray(InterimSchemaCount*,repc,repcs,
      repc=&(InterimSchemaCount&)store;
      if (repcIndex==0) repc->copyFrom(c);
      else repc->multiply(c,*repcs A[repcIndex-1]);
    );
  }



  static void getNonDescendingCombos(InterimSchemaCount &c,long maxReps,Array<InterimSchemaCount*> &repcs,Array<Array<InterimSchemaCount*>*> &tmpaaisc,ClassStore<InterimSchemaCount> &store,ArrayStore<InterimSchemaCount*> &arrstore) {
    PRFLFN("ISC::getNonDescendingCombos");

    bool debug=false;//g_afterFault;

    if (debug) {c.prntSC("Get non-desc for");prnt("   x %d\n",maxReps);}


    if (c.ubp1==c.lb) SERROR("Empty count");
    if (tmpaaisc.num()) SERROR("Should be cleared");

    tmpaaisc.setUpperBound(maxReps-1);// i.e. index 0 is rep 1, 1 is rep 2,..
    IterateArray(Array<InterimSchemaCount*>*,aisc,tmpaaisc,
      long r=aiscIndex+1;
      aisc=&(Array<InterimSchemaCount*>&)arrstore;
      aisc->setUpperBound(c.ubp1-1-c.lb);// i.e. index 0 is c.lb, 1 is c.lb+1,...
      IterateArray(InterimSchemaCount*,isc,*aisc,
        isc=&(InterimSchemaCount&)store;
        isc->clear();
        isc->setBounds(c.lb*r,(c.ubp1-1)*r);// i.e. range for r reps
        if (debug) {prnt("x%d upto:%d   ",r,iscIndex+c.lb);isc->prntSC("");}
      );
    );

    InterimSchemaCount &tmpisc=(InterimSchemaCount&)store;

    for (long r=1;r<=maxReps;r++) {// total reps
      dprnt("r:%d\n",r);
      for (long newMaxr=1;newMaxr<=r;newMaxr++) {// reps of last size
        dprnt("  newMaxr:%d\n",newMaxr);
        for (long newMax=c.lb+(r==newMaxr?0:1);newMax<c.ubp1;newMax++) {// last size
          dprnt("  newMax:%d\n",newMax);
          if (c.a[newMax].ubp1>0) {// i.e. the count for this size is non zero
            BigUInt newMaxc=BigUIntFunctions::zero(),tmpbui2=BigUIntFunctions::zero();// newMaxc is count of newMaxr reps of c.a[newMax]

            if (newMaxr==1) copyTAArray(c.a[newMax],newMaxc);// single rep
            else {
              copyTAArray(c.a[newMax],tmpbui2);// we want C(c.a[newMax]+newMaxr-1 , newMaxr) for combos of newMaxr things from a set of c.a[newMax], disregarding order but allowing duplicates
              BigUIntFunctions::addInplace(tmpbui2,newMaxr-1);
              newMaxc=BigUIntFunctions::choose(tmpbui2,newMaxr);// multiple reps (choose(c[newMax]+newMaxr-1,newMaxr) for newMaxr things from c[newMax] things, order unimportant, repetition allowed)
              TA_clear(tmpbui2);
            }
            long newMaxsz=newMax*newMaxr;

            dprnt("     %d things from a set of %f = %f\n",newMaxr,BigUIntFunctions::asDouble(c.a[newMax]),BigUIntFunctions::asDouble(newMaxc));

            InterimSchemaCount &newisc=*(*tmpaaisc A[r-1])A[newMax-c.lb];// the output count (i.e. totals of r things with last thing as newMax)

            if (r==newMaxr) BigUIntFunctions::addInplace(newisc.a[newMaxsz],newMaxc);// just add tmpbui
            else {
              InterimSchemaCount &previsc=*(*tmpaaisc A[r-newMaxr-1])A[newMax-1-c.lb];// must multiply by the previous entry (i.e. totals or r-newMaxr things with last thing less than newMax)
              tmpisc.multiply(previsc,newMaxc,newMaxsz);
              newisc.add(tmpisc);
            }

            if (debug) newisc.prntSC("      newisc:");

            TA_clear(newMaxc);
          }
        }
      }

      InterimSchemaCount *pisc=NULL;

      for (long i=c.lb;i<c.ubp1;i++) {// accumulate (i.e. going from totals of r things with last as newMax, to totals or r things with last less than or equal to newMax)
        InterimSchemaCount &acc=*(*tmpaaisc A[r-1])A[i-c.lb];
        if (pisc) acc.add(*pisc);
        pisc=&acc;
        acc.clip();

        if (debug) {prnt("   cumul : x%d upto:%d   ",r,i);acc.prntSC("");}
      }
    }

    tmpisc.clear();store.returnObj(tmpisc);


    FilterToArray(Array<InterimSchemaCount*>*,aisc,tmpaaisc,       InterimSchemaCount*,saveisc,repcs,
      true,
      saveisc=(*aisc)A[c.ubp1-1-c.lb];(*aisc)A[c.ubp1-1-c.lb]=NULL;
      saveisc->clip();
      IterateArray(InterimSchemaCount*,isc,*aisc,      if (isc) {isc->clear();store.returnObj(*isc);});
      aisc->clear();
      arrstore.returnArr(*aisc);
    );
    tmpaaisc.clear();
  }









/*


  struct BigUInt2 {
    BigUInt i,irep;
  };









  static Array<BigUInt2> tmpica1,tmpica2;

#define TOBI(j,k,r) ((*arr2) A[(r)-1+reps*((j)-minj+numj*((k)-mink))])
#define FROMBI(j,k,r) ((*arr1) A[(r)-1+reps*((j)-minj+numj*((k)-mink))])
#define SWAPBI do {arrtmp=arr1;arr1=arr2;arr2=arrtmp;} while(0)

  void getNonDescendingCombos(InterimSchemaCount &c,long reps) {
    Array<BigUInt2> *arr1=&tmpica1,*arr2=&tmpica2,*arrtmp;
    long mini=c.lb,maxi=c.ubp1-1;
    long minj=mini,maxj=maxi,numj=maxj+1-minj;
    long mink=minj,maxk=maxj,maxmaxk=reps*maxj,maxnumk=maxmaxk+1-mink;
    arr1->setUpperBound(maxnumk*numj*reps-1,true);
    arr2->setUpperBound(maxnumk*numj*reps-1,true);
    arr1->zero(0,arr1->num());
    arr2->zero(0,arr2->num());

    for (long i=mini;i<=maxi;i++) {
      copyTAArray(c.a[i],FROMBI(i,i,1).irep);
      FROMBI(i,i,1).i=BigUIntFunctions::one();
    }

    long maxkwas=maxk;
    for (long rep=2;rep<=reps;rep++) {
      for (long i=mini;i<=maxi;i++) {
        for (long j=minj;j<=i;j++) {
          for (long k=mink;k<=maxk;k++) {
            for (long r=1;r<rep;r++) {
              BigUInt2 &from=FROMBI(j,k,r);
              if (from.irep.ubp1) {
                if (i==j) {
                  BigUInt2 &to=TOBI(i,k+i,r+1);
                  to.i=from.i;
                  TA_init(from.i);
                  TA_clear(from.irep);
                  to.irep=BigUIntFunctions::choose(c.a[i],r+1);
                }
                else {
                  BigUInt2 &to=TOBI(i,k+i,1);
                  to.i=BigUIntFunctions::multiply(from.i,from.irep);
                  TA_clear(from.i);
                  TA_clear(from.irep);
                  copyTAArray(c.a[i],to.irep);
                }
                if (k+i>maxk) maxk=k+i;
              }
            }
          }
        }
      }
      SWAPBI;
    }

    clear();
    lb=minj;ubp1=maxk+1;

    for (long k=lb;k<ubp1;k++) {
      BigUInt acc;
      TA_init(acc);
      for (long j=minj;j<=maxj;j++) {
        for (long r=1;r<=reps;r++) {
          BigUInt2 bi=FROMBI(j,k,r);
          if (bi.irep.ubp1) {
            BigUIntFunctions::addInplace(acc,BigUIntFunctions::multiplyInplace(bi.i,bi.irep));
            TA_clear(bi.i);
            TA_clear(bi.irep);
          }
        }
      }
      a[k]=acc;
    }
  }
#undef TOBI
#undef FROMBI
#undef SWAPBI
*/
};

#ifdef MAINCPPFILE
InterimSchemaCount InterimSchemaCount::tmpmulsc;
//Array<InterimSchemaCount::BigUInt2> InterimSchemaCount::tmpica1;
//Array<InterimSchemaCount::BigUInt2> InterimSchemaCount::tmpica2;
#endif









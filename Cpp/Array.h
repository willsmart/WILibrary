//>Maker.cpp

#ifndef ARRAYH
#define ARRAYH

#include "BasicArray.h"



#undef NOTEENABLE
#define NOTEENABLE(a)

#ifdef GENDEBUG
#define ForLoopA(var) [var##Index] // use this to bound check in arrays
#define ForLoopA0(var) [var##Index] // use this to bound check in arrays
#else
#define ForLoopA(var) .a[var##Index-var##LowerBound] // use this to enable quick for loops
#define ForLoopA0(var) .a[var##Index] // use this to enable quick for loops
#endif

#define ForEachMacroInArray(var,arr,valueMacro,preTestMacro,postTestMacro) \
  for (long var##LowerBound=(arr).lowerBound(),var##UpperBound=(arr).upperBound(),var##Index=var##LowerBound; \
    preTestMacro(var##Index<=var##UpperBound)&&(&(var=(valueMacro)))postTestMacro; \
    var##Index++)

#define ForEachMacroInArray0(var,arr,valueMacro,preTestMacro,postTestMacro) \
  for (long var##UpperBound=(arr).upperBound(),var##Index=0; \
    preTestMacro(var##Index<=var##UpperBound)&&(&(var=(valueMacro)))postTestMacro; \
    var##Index++)

// these are the forms to use when doing a loop that may involve deleting the current object (which is detected by the upper bound changing to a lower value)
#define ForEachMacroInArray_(var,arr,valueMacro,preTestMacro,postTestMacro) \
  for (long var##LowerBound=(arr).lowerBound(),var##UpperBound=(arr).upperBound(),var##Index=var##LowerBound,var##tmpub; \
    preTestMacro(var##Index<=var##UpperBound)&&(&(var=(valueMacro)))postTestMacro; \
    var##Index+=(var##UpperBound<=(var##tmpub=(arr).upperBound())?1:0),var##UpperBound=var##tmpub)

#define ForEachMacroInArray0_(var,arr,valueMacro,preTestMacro,postTestMacro) \
  for (long var##UpperBound=(arr).upperBound(),var##Index=0,var##tmpub; \
    preTestMacro(var##Index<=var##UpperBound)&&(&(var=(valueMacro)))postTestMacro; \
    var##Index+=(var##UpperBound<=(var##tmpub=(arr).upperBound())?1:0),var##UpperBound=var##tmpub)




// use these if arr.lowerBound could possibly be non-zero
#define ForEachInArrayLB(var,arr) ForEachMacroInArray(var,arr,&(arr)ForLoopA(var),(!(var=NULL))&&,)
#define ForEachInArrayLB_(var,arr) ForEachMacroInArray_(var,arr,&(arr)ForLoopA(var),(!(var=NULL))&&,)
#define ForEachValueInArrayLB(var,arr) ForEachMacroInArray(var,arr,(arr)ForLoopA(var),,)
#define ForEachValueInArrayLB_(var,arr) ForEachMacroInArray_(var,arr,(arr)ForLoopA(var),,)
#define ForEachIndexInArrayLB(index,arr) \
  for (long index##UpperBound=((index=(arr).lowerBound())*0+(arr).upperBound()); \
    (index<=index##UpperBound); \
    index++)
#define ForEachFieldInArrayLB(var,arr,field) ForEachMacroInArray(var,arr,((arr)ForLoopA(var))field,,)
#define ForEachFieldInArrayLB_(var,arr,field) ForEachMacroInArray_(var,arr,((arr)ForLoopA(var))field,,)
                        

// use these if arr.lowerbound is always zero (these are the default as I have never yet used thge lower bound feature of my arrays, never needed to!)
#define ForEachInArray(var,arr) ForEachMacroInArray0(var,arr,&(arr)ForLoopA0(var),(!(var=NULL))&&,)
#define ForEachInArray_(var,arr) ForEachMacroInArray0_(var,arr,&(arr)ForLoopA0(var),(!(var=NULL))&&,)
#define ForEachValueInArray(var,arr) ForEachMacroInArray0(var,arr,(arr)ForLoopA0(var),,)
#define ForEachValueInArray_(var,arr) ForEachMacroInArray0_(var,arr,(arr)ForLoopA0(var),,)
#define ForEachIndexInArray(index,arr) \
  for (long index##UpperBound=(index=0)+(arr).upperBound(); \
    (index<=index##UpperBound); \
    index++)
#define ForEachFieldInArray(var,arr,field) ForEachMacroInArray0(var,arr,((arr)ForLoopA0(var))field,,)
#define ForEachFieldInArray_(var,arr,field) ForEachMacroInArray0_(var,arr,((arr)ForLoopA0(var))field,,)
                         

// use these for managed arrays, or anything else
#define ForEachInArrayM(var,arr) ForEachMacroInArray0(var,arr,&(arr)[var##Index],(!(var=NULL))&&,)
#define ForEachInArrayM_(var,arr) ForEachMacroInArray0_(var,arr,&(arr)[var##Index],(!(var=NULL))&&,)
#define ForEachValueInArrayM(var,arr) ForEachMacroInArray0(var,arr,(arr)[var##Index],,)
#define ForEachValueInArrayM_(var,arr) ForEachMacroInArray0_(var,arr,(arr)[var##Index],,)
#define ForEachFieldInArrayM(var,arr,field) ForEachMacroInArray0(var,arr,((arr)[var##Index])field,,)
#define ForEachFieldInArrayM_(var,arr,field) ForEachMacroInArray0_(var,arr,((arr)[var##Index])field,,)
                         



// equiv to FilterToArray(typ,obj,arr,  typ,toobj,arr, filter, toobj=obj)
#define FilterArray(typ,obj,arr,filter) do { \
    typ *obj##__base=(arr).a; \
    long obj##Index;long obj##__j=-1;long obj##__n=(arr).num(); \
    for (obj##Index=0;obj##Index<obj##__n;obj##Index++) { \
      typ &obj=obj##__base[obj##Index]; \
      if ((filter)&&(obj##Index!=(++obj##__j))) obj##__base[obj##__j]=obj; \
    } \
    (arr).setUpperBound((arr).lowerBound()+obj##__j); \
} while(0)


#define FilterToArray(typ,obj,arr,totyp,toobj,toarr,filter,...) do { \
    long obj##Index;long toobj##Index=-1;long obj##__n=(arr).num(); \
    if ((void*)&(arr)!=(void*)&(toarr)) (toarr).setUpperBound(obj##__n-1); \
    totyp *toobj##__tobase=(toarr).a; \
    typ *obj##__frombase=(arr).a; \
    for (obj##Index=0;obj##Index<obj##__n;obj##Index++) { \
      typ &obj=obj##__frombase[obj##Index]; \
      if (filter) { \
        totyp &toobj=toobj##__tobase[++toobj##Index]; \
        __VA_ARGS__ \
      } \
    } \
    if ((toarr).lowerBound()+toobj##Index!=obj##__n-1) (toarr).setUpperBound((toarr).lowerBound()+toobj##Index); \
} while(0)



#define FilterToArray_embeddedFilter(typ,obj,arr,totyp,toobj,toarr,...) do { \
  long obj##Index;long toobj##Index=0;long obj##__n=(arr).num(); \
    if ((void*)&(arr)!=(void*)&(toarr)) (toarr).setUpperBound(obj##__n-1); \
    totyp *toobj##__tobase=(toarr).a; \
    typ *obj##__frombase=(arr).a; \
    for (obj##Index=0;obj##Index<obj##__n;obj##Index++) { \
      typ &obj=obj##__frombase[obj##Index]; \
	  bool toobj##Keep=true; \
	  totyp &toobj=toobj##__tobase[toobj##Index]; \
	  __VA_ARGS__ \
	  if (toobj##Keep) toobj##Index++; \
    } \
    if ((toarr).lowerBound()+toobj##Index!=obj##__n) (toarr).setUpperBound((toarr).lowerBound()+toobj##Index-1); \
} while(0)


#define IterateArray(typ,obj,arr,...) do { \
  typ*obj##__base=(arr).a; \
  long obj##Index;long obj##__n=(arr).num(); \
  bool obj##ArrayChanged=false; \
  for (obj##Index=0;obj##Index<obj##__n;obj##Index++) { \
    typ&obj=obj##__base[obj##Index]; \
    __VA_ARGS__ \
    if (obj##ArrayChanged) {obj##ArrayChanged=false;obj##__n=(arr).num();obj##__base=(arr).a;} \
  } \
} while(0)

#define IterateArrayLB(typ,obj,arr,...) do { \
  typ*obj##__base=(arr).a; \
  long obj##Index;long obj##__n=(arr).num();long obj##__lb=(arr).lowerBound(); \
  bool obj##ArrayChanged=false; \
  for (obj##Index=obj##__lb;obj##Index<obj##__n+obj##__lb;obj##Index++) { \
    typ&obj=obj##__base[obj##Index-obj##__lb]; \
    __VA_ARGS__ \
    if (obj##ArrayChanged) {obj##ArrayChanged=false;obj##__n=(arr).num();obj##__base=(arr).a;} \
  } \
} while(0)

#define IterateArrayLBA(typ,obj,arr,...) do { \
  long obj##Index;long obj##__n=(arr).num();long obj##__lb=(arr).lowerBound(); \
  bool obj##ArrayChanged=false; \
  for (obj##Index=obj##__lb;obj##Index<obj##__n+obj##__lb;obj##Index++) { \
    typ&obj=(arr)[obj##Index]; \
    __VA_ARGS__ \
    if (obj##ArrayChanged) {obj##ArrayChanged=false;obj##__n=(arr).num();} \
  } \
} while(0)



// op is < for ascending, > for descending
#define arrHeapSort(arr,field,op,firsti,lasti,tmpObj) for( long __outeri=lasti-firsti;__outeri>0;__outeri--) arrHeapSort2(arr,field,op,firsti,__outeri,tmpObj);

#define arrHeapSort2(arr,field,op,firsti,arr_ubound,tempObj) \
  do { \
    long __i;long __o; \
    long __lChild;long __rChild;long __mChild;long __root; \
    __root = (arr_ubound-1)/2; \
    for(__o=__root;__o>=0;__o--) { \
      for(__i=__root;__i>=0;__i--) { \
        __lChild = (2*__i)+1; \
        __rChild = (2*__i)+2; \
        if ((__lChild <= arr_ubound) && (__rChild <= arr_ubound)) { \
          __mChild=(arr[firsti+__lChild]field op arr[firsti+__rChild]field?__rChild:__lChild); \
        } \
        else { \
          __mChild=(__rChild > arr_ubound?__lChild:__rChild); \
        } \
        if (arr[firsti+__i]field op arr[firsti+__mChild]field) { \
          tempObj = arr[firsti+__i]; \
          arr[firsti+__i] = arr[firsti+__mChild]; \
          arr[firsti+__mChild] = tempObj; \
        } \
      } \
    } \
    tempObj = arr[firsti]; \
    arr[firsti+0] = arr[firsti+arr_ubound]; \
    arr[firsti+arr_ubound] = tempObj; \
  } while(0)





#define linearSort(typ,arr,retarr,intFromobjMacro,tmpLongArr) do { \
  Array<typ> &ls_arr=(arr),&ls_retarr=(retarr); \
  ls_retarr.setUpperBound(ls_arr.upperBound()); \
  if (ls_arr.num()) { \
    long ls_mini;long ls_maxi; \
    do { \
      typ &obj=ls_arr.a[0]; \
      ls_mini=ls_maxi=(intFromobjMacro); \
    } while(0); \
    IterateArray(typ,obj,ls_arr, \
      long ls_n=(intFromobjMacro); \
      if (ls_n<ls_mini) ls_mini=ls_n; \
      if (ls_n>ls_maxi) ls_maxi=ls_n; \
    ); \
    Array<long> &ls_tmpa=(tmpLongArr); \
    ls_tmpa.setUpperBound(ls_maxi-ls_mini); \
    ls_tmpa.zero(0,ls_tmpa.num()); \
    IterateArray(typ,obj,ls_arr, \
      ls_tmpa.a[(intFromobjMacro)-ls_mini]++; \
    ); \
    long ls_accum=0;long ls_tmpi; \
    IterateArray(long,i,ls_tmpa,     ls_tmpi=i;i=ls_accum;ls_accum+=ls_tmpi;); \
    IterateArray(typ,obj,ls_arr, \
      ls_retarr.a[ls_tmpa.a[(intFromobjMacro)-ls_mini]++]=obj; \
    ); \
  } \
} while(0)




// both arra and arrb must be sorted on field
#define IntersectArray(typ,field,arra,arrb,toarr) do { \
  long ia_j=-1;long ia_na=(arra).num();long ia_nb=(arrb).num(); \
  if ((&(toarr)!=&(arra))&&(&(toarr)!=&(arrb))) (toarr).setUpperBound((ia_na<ia_nb?ia_na:ia_nb)-1); \
  typ*ia_aa=(arra).a; \
  typ*ia_bb=(arrb).a; \
  typ*ia_to=(toarr).a; \
  while(ia_na&&ia_nb) { \
    if (((*ia_aa)field)<((*ia_bb)field)) {ia_aa++;ia_na--;} \
    else { \
      if (((*ia_bb)field)==((*ia_aa)field)) {ia_to[++ia_j]=*(ia_aa++);ia_na--;} \
      ia_bb++;ia_nb--; \
    } \
  } \
  (toarr).setUpperBound(ia_j); \
} while(0)


// both arra and arrb must be sorted on field
#define UnionArray(typ,field,arra,arrb,toarr) do { \
  long ua_na=(arra).num();long ua_nb=(arrb).num();long ua_na2=ua_na;long ua_nb2=ua_nb;long ua_nto=0; \
  typ *ua_aa=(arra).a; \
  typ *ua_bb=(arrb).a; \
 \
  while(ua_na2&&ua_nb2) { \
    if (((*ua_aa)field)<((*ua_bb)field)) {ua_nto++;ua_aa++;ua_na2--;} \
    else { \
      if (((*ua_bb)field)==((*ua_aa)field)) {ua_aa++;ua_na2--;} \
      ua_nto++;ua_bb++;ua_nb2--; \
    } \
  } \
  ua_nto+=ua_na2+ua_nb2; \
 \
  (toarr).setUpperBound(ua_nto-1); \
  typ *ua_to=(toarr).a; \
  ua_aa=(arra).a+(ua_na-1);ua_bb=(arrb).a+(ua_nb-1); \
  long ua_j=ua_nto; \
 \
  while(ua_na&&ua_nb) { \
    if (((*ua_aa)field)>((*ua_bb)field)) {ua_to[--ua_j]=*(ua_aa--);ua_na--;} \
    else { \
      if (((*ua_bb)field)==((*ua_aa)field)) {ua_aa--;ua_na--;} \
      ua_to[--ua_j]=*(ua_bb--);ua_nb--; \
    } \
  } \
  if (ua_na&&(&(toarr)!=&(arra))) (toarr).copy(0,*(arra).a,ua_na); \
  else if (ua_nb&&(&(toarr)!=&(arrb))) (toarr).copy(0,*(arrb).a,ua_nb); \
} while (0)


#define VerifySorted(typ,field,arr,nodup) do { \
  if (nodup) IterateArray(typ,vs_v,arr,     if (vs_vIndex&&((vs_v field)==((arr).a[vs_vIndex-1]field))) SERROR("Not sorted (duplicate)"););  \
  IterateArray(typ,vs_v,arr,     if (vs_vIndex&&((vs_v field)<((arr).a[vs_vIndex-1]field))) SERROR("Not sorted"););  \
} while(0)

// both subs and sup must be sorted on field
#define ArrayIsSubset(typ,field,sub,sup,retBool) do { \
  long ais_na=(sub).num();long ais_nb=(sup).num(); \
  if (ais_na>ais_nb) retBool=false; \
  else { \
    typ *ais_aa=(sub).a; \
    typ *ais_bb=(sup).a; \
    while(ais_na&&ais_nb) { \
      if (((*ais_aa)field)<((*ais_bb)field)) break; \
      else { \
        if (((*ais_bb)field)==((*ais_aa)field)) {ais_aa++;ais_na--;} \
        ais_bb++;ais_nb--; \
      } \
    } \
    retBool=(!ais_na); \
  } \
} while(0)

#define VerifySubset(typ,field,sub,sup) do { \
  VerifySorted(typ,field,sub,true); \
  VerifySorted(typ,field,sup,true); \
  bool vs_b;ArrayIsSubset(typ,field,sub,sup,vs_b); \
  if (!vs_b) SERROR("Not subset"); \
} while(0)
#define VerifyNotSubset(typ,field,sub,sup) do { \
  VerifySorted(typ,field,sub,true); \
  VerifySorted(typ,field,sup,true); \
  bool vs_b;ArrayIsSubset(typ,field,sub,sup,vs_b); \
  if (vs_b) SERROR("Is subset"); \
} while(0)

#define IsSubset(typ,subObjIndexMacro,subnum,supObjIndexMacro,supnum,retBool) do { \
  long ais_na=(subnum);long ais_nb=(supnum); \
  if (ais_na>ais_nb) retBool=false; \
  else { \
    long subObjIndex=0;long supObjIndex=0; \
    while((subObjIndex<ais_na)&&(supObjIndex<ais_nb)) { \
      typ &subObj=(subObjIndexMacro); \
      typ &supObj=(supObjIndexMacro); \
      if (subObj<supObj) break; \
      else { \
        if (subObj==supObj) subObjIndex++; \
        supObjIndex++; \
      } \
    } \
    retBool=(subObjIndex==ais_na); \
  } \
} while(0)



// both arra and arrb must be sorted on field
#define ArrayIsDisjoint(typ,field,arra,arrb,retBool) do { \
  long aid_na=(arra).num();long aid_nb=(arrb).num(); \
  if (!(aid_na||aid_nb)) retBool=false; \
  else { \
    typ *aid_aa=(arra).a; \
    typ *aid_bb=(arrb).a; \
    retBool=true; \
    while(aid_na&&aid_nb) { \
      if (((*aid_aa)field)<((*aid_bb)field)) {aid_aa++;aid_na--;} \
      else if (((*aid_bb)field)<((*aid_aa)field)) {aid_bb++;aid_nb--;} \
      else {retBool=false;break;} \
    } \
  } \
} while(0)






#define FindArrayElement(ValueTyp,findValue,valueMacroForindex,indexLowerBound,indexUpperBound,retIndex,retInsertIndex,findLeftmost) do { \
  long fae_lb=indexLowerBound;long fae_ub=indexUpperBound;long fae_mid;long index; \
  ValueTyp fae_lbv;ValueTyp fae_ubv;ValueTyp fae_fv=(findValue); \
  do { \
    if (fae_lb>fae_ub) {retIndex=-1;retInsertIndex=fae_lb;break;} \
    index=fae_lb;fae_lbv=(valueMacroForindex); \
    if (fae_lbv==fae_fv) {retIndex=retInsertIndex=fae_lb;break;} \
    if (fae_lbv>fae_fv) {retIndex=-1;retInsertIndex=fae_lb;break;} \
    if (fae_lb==fae_ub) {retIndex=-1;retInsertIndex=fae_ub+1;break;} \
    index=fae_ub;fae_ubv=(valueMacroForindex); \
    if (fae_ubv==fae_fv) {retIndex=retInsertIndex=fae_ub;break;} \
    if (fae_ubv<fae_fv) {retIndex=-1;retInsertIndex=fae_ub+1;break;} \
    retIndex=-1; \
    while (fae_lb<fae_ub-1) { \
      fae_mid=index=(fae_lb+fae_ub)/2; \
      ValueTyp val=(valueMacroForindex); \
      if ((val<fae_lbv)||(val>fae_ubv)) SERROR("Not sorted"); \
      else if (val==fae_fv) {retIndex=retInsertIndex=fae_mid;break;} \
      else if (val<fae_fv) {fae_lb=fae_mid;fae_lbv=val;} \
      else {fae_ub=fae_mid;fae_ubv=val;} \
    } \
    if (retIndex==-1) retInsertIndex=fae_ub; \
  } while (0); \
  if (findLeftmost) { \
    if (retIndex>=0) { \
      retIndex--; \
      fae_lb=indexLowerBound; \
      while (retIndex>=fae_lb) { \
        index=retIndex; \
        ValueTyp val=(valueMacroForindex); \
        if (val!=fae_fv) break; \
        retIndex--; \
      } \
      retIndex++; \
    } \
  } \
} while (0)



template<class I> class Array;// below
// example uses (and nice templates)
template<class I> void intersectArray(Array<I> &a,Array<I> &b,Array<I> &to);
template<class I> void unionArray(Array<I> &a,Array<I> &b,Array<I> &to);
template<class I> bool arrayIsSubset(Array<I> &sub,Array<I> &sup);
template<class I> bool arrayIsProperSubset(Array<I> &sub,Array<I> &sup);
template<class I> bool arrayIsDisjoint(Array<I> &a,Array<I> &b);
template<class I,class V> I *findArrayElement(Array<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I,class V> long findArrayElementIndex(Array<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I,class V> long findArrayElementInsertionPoint(Array<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I,class V> I *findArrayElement(Array<I*> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I,class V> long findArrayElementIndex(Array<I*> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I,class V> long findArrayElementInsertionPoint(Array<I*> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I> I *findArrayElement(Array<I> &a,I findValue,bool findLeftMost=false);
template<class I> long findArrayElementIndex(Array<I> &a,I findValue,bool findLeftMost=false);
template<class I> long findArrayElementInsertionPoint(Array<I> &a,I findValue,bool findLeftMost=false);
template<class I> long findArrayElementIndexAndInsertionPoint(Array<I> &a,I findValue,long &retInsertionPoint,bool findLeftMost=false);
#ifdef MAINCPPFILE
template <class I> void intersectArray(Array<I> &a,Array<I> &b,Array<I> &to) {IntersectArray(I,,a,b,to);}
template <class I> void unionArray(Array<I> &a,Array<I> &b,Array<I> &to) {UnionArray(I,,a,b,to);}
template <class I> bool arrayIsSubset(Array<I> &sub,Array<I> &sup) {bool ret;ArrayIsSubset(I,,sub,sup,ret);return(ret);}
template<class I> bool arrayIsProperSubset(Array<I> &sub,Array<I> &sup) {return((sub.num()<sup.num())&&arrayIsSubset(sub,sup));}
template <class I> bool arrayIsDisjoint(Array<I> &a,Array<I> &b) {bool ret;ArrayIsDisjoint(I,,a,b,ret);return(ret);}
template<class I,class V> I *findArrayElement(Array<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost) {
  long foundAtIndex=findArrayElementIndex(a,findValue,sampleElement,sampleValue,findLeftMost);
  return(foundAtIndex==-1?NULL:(a.a+foundAtIndex));
}
template<class I,class V> long findArrayElementIndex(Array<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost) {
  DWRD offs=(DWRD)(((char*)&sampleValue)-(char*)&sampleElement);
  long foundAtIndex,insertAtIndex;
  FindArrayElement(V,findValue,*(V*)(((char*)(a.a+index))+offs),a.lowerBound(),a.upperBound(),foundAtIndex,insertAtIndex,findLeftMost);
  return(foundAtIndex);
}
template<class I,class V> long findArrayElementInsertionPoint(Array<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost) {
  DWRD offs=(DWRD)(((char*)&sampleValue)-(char*)&sampleElement);
  long foundAtIndex,insertAtIndex;
  FindArrayElement(V,findValue,*(V*)(((char*)(a.a+index))+offs),a.lowerBound(),a.upperBound(),foundAtIndex,insertAtIndex,findLeftMost);
  return(insertAtIndex);
}
template<class I,class V> I *findArrayElement(Array<I*> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost) {
  long foundAtIndex=findArrayElementIndex(a,findValue,sampleElement,sampleValue,findLeftMost);
  return(foundAtIndex==-1?NULL:(a.a[foundAtIndex]));
}
template<class I,class V> long findArrayElementIndex(Array<I*> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost) {
  DWRD offs=(DWRD)(((char*)&sampleValue)-(char*)&sampleElement);
  long foundAtIndex,insertAtIndex;
  FindArrayElement(V,findValue,*(V*)(((char*)(a.a[index]))+offs),a.lowerBound(),a.upperBound(),foundAtIndex,insertAtIndex,findLeftMost);
  return(foundAtIndex);
}
template<class I,class V> long findArrayElementInsertionPoint(Array<I*> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost) {
  DWRD offs=(DWRD)(((char*)&sampleValue)-(char*)&sampleElement);
  long foundAtIndex,insertAtIndex;
  FindArrayElement(V,findValue,*(V*)(((char*)(a.a[index]))+offs),a.lowerBound(),a.upperBound(),foundAtIndex,insertAtIndex,findLeftMost);
  return(insertAtIndex);
}
template<class I> I *findArrayElement(Array<I> &a,I findValue,bool findLeftMost) {
  long foundAtIndex=findArrayElementIndex(a,findValue,findLeftMost);
  return(foundAtIndex==-1?NULL:(a.a+foundAtIndex));
}
template<class I> long findArrayElementIndex(Array<I> &a,I findValue,bool findLeftMost) {
  long foundAtIndex,insertAtIndex;
  FindArrayElement(I,findValue,a.a[index],a.lowerBound(),a.upperBound(),foundAtIndex,insertAtIndex,findLeftMost);
  return(foundAtIndex);
}
template<class I> long findArrayElementInsertionPoint(Array<I> &a,I findValue,bool findLeftMost) {
  long foundAtIndex,insertAtIndex;
  FindArrayElement(I,findValue,a.a[index],a.lowerBound(),a.upperBound(),foundAtIndex,insertAtIndex,findLeftMost);
  return(insertAtIndex);
}
template<class I> long findArrayElementIndexAndInsertionPoint(Array<I> &a,I findValue,long &retInsertionPoint,bool findLeftMost) {
  long foundAtIndex;
  FindArrayElement(I,findValue,a.a[index],a.lowerBound(),a.upperBound(),foundAtIndex,retInsertionPoint,findLeftMost);
  return(foundAtIndex);
}
#endif


// alessthanbmacro is something like arr[a]<arr[b] to just sort ascending
#define arrHeapSortb(typ,arr,objalessthanobjbmacro,firsti,lasti,tmpObj) do { \
  for( long __outeri=lasti-firsti;__outeri>0;__outeri--) arrHeapSortb2(typ,arr,objalessthanobjbmacro,firsti,__outeri,tmpObj); \
} while(0)

#define arrHeapSortb2(typ,arr,objalessthanobjbmacro,firsti,arr_ubound,tempObj) \
  do { \
    long __i;long __o; \
    long __lChild;long __rChild;long __mChild;long __root; \
    __root = (arr_ubound-1)/2; \
    for(__o=__root;__o>=0;__o--) { \
      for(__i=__root;__i>=0;__i--) { \
        __lChild = (2*__i)+1; \
        __rChild = (2*__i)+2; \
        if ((__lChild <= arr_ubound) && (__rChild <= arr_ubound)) { \
          typ&obja=arr[firsti+__lChild]; \
          typ&objb=arr[firsti+__rChild]; \
          __mChild=((objalessthanobjbmacro)?__rChild:__lChild); \
        } \
        else { \
          __mChild=(__rChild > arr_ubound?__lChild:__rChild); \
        } \
        do { \
          typ&obja=arr[firsti+__i]; \
          typ&objb=arr[firsti+__mChild]; \
          if (objalessthanobjbmacro) { \
            tempObj = arr[firsti+__i]; \
            arr[firsti+__i] = arr[firsti+__mChild]; \
            arr[firsti+__mChild] = tempObj; \
          } \
        } while(0); \
      } \
    } \
    tempObj = arr[firsti]; \
    arr[firsti+0] = arr[firsti+arr_ubound]; \
    arr[firsti+arr_ubound] = tempObj; \
  } while(0)




#define arrBinSearch(ValueType,macroForValuei,asearchFor,afirsti,alasti,boolFound,insertAt) \
  do {                                                                        \
    ValueType firstVal;                                                        \
    ValueType lastVal;                                                        \
    ValueType midVal;                                                        \
    ValueType searchFor=(asearchFor);                                        \
    long i;long firsti=(afirsti);long lasti=(alasti);                                \
    boolFound=false;                                                        \
    i=firsti;firstVal=(macroForValuei);                                        \
    if (searchFor<firstVal) insertAt=firsti;                                \
    else {                                                                \
      i=lasti;lastVal=(macroForValuei);                                        \
      if (searchFor>lastVal) insertAt=lasti+1;                        \
      else {                                                                \
        for (i=(lasti+firsti)/2,midVal=(macroForValuei);firsti>=lasti;i=(lasti+firsti)/2,midVal=(macroForValuei)) { \
          if (searchFor==midVal) {                                        \
            for (i++;(i<=lasti)&&((macroForValuei)==searchFor);i++);        \
            boolFound=true;insertAt=i;break;                                \
          }                                                                \
          else if (searchFor<midVal) {lasti=--i;lastVal=(macroForValuei);} \
          else {firsti=++i;firstVal=(macroForValuei);}                        \
        }                                                                \
        if (!boolFound) insertAt=firsti;                                \
      }                                                                        \
    }                                                                        \
  } while(0)


// eg arrBinSearch(long,incomes[i],customerIncome,0,num-1,foundFlag,insertAt);



template <class I> class Array : public BasicArray<I> {
  PubDefCC("A.");
  ContextIndex ci;
 public:
  Array(WRD typcc,ContextIndex &aci,long alowestIndex,bool aautoExtendDown,long ahighestIndex,bool aautoExtendUp,const char *aname=NULL) :
       ci(aci),
    BasicArray<I>(typcc,alowestIndex,aautoExtendDown,ahighestIndex,aautoExtendUp,aname) {
          VerCC(typcc);
  }

  Array(WRD typcc,long alowestIndex,bool aautoExtendDown,long ahighestIndex,bool aautoExtendUp,const char *aname=NULL) :
       ci(g_contexti),
    BasicArray<I>(typcc,alowestIndex,aautoExtendDown,ahighestIndex,aautoExtendUp,aname) {
          VerCC(typcc);
  }

  Array(WRD typcc,ContextIndex &aci,long ahighestIndex=-1,bool aautoExtendUp=true,const char *aname=NULL) : BasicArray<I>(typcc,ahighestIndex,aautoExtendUp,aname),
        ci(aci) {
          VerCC(typcc);
  }

  Array(WRD typcc=MakeCC("?a"),long ahighestIndex=-1,bool aautoExtendUp=true,const char *aname=NULL) : BasicArray<I>(typcc,ahighestIndex,aautoExtendUp,aname) ,
        ci(g_contexti) {
          VerCC(typcc);
  }

  Array(const Array<I> &copy) : BasicArray<I>(MakeCC("!a")) {
    SERROR("Array copy constructor not supported");
  }

  I &operator[](long index) {
    FNNOTE2("Array[%d]",index);
    if ((index<this->lowestIndex)||(index>this->highestIndex)) ERROR4("Not currently allowed to index outside bounds (%d vs [%d,%d])",index,this->lowestIndex,this->highestIndex);
    return(this->itemAt(index));
  }

  I &get(long index) {
    FNNOTE2("Array.get(%d)",index);
    return(this->itemAt(index));
  }

  I &getExisting(long index) {
    if ((index<this->lowestIndex)||(index>this->highestIndex)) ERROR4("Indexed %d, which is outside [%d,%d]",index,this->lowestIndex,this->highestIndex);
    return(this->itemAt(index));
  }

  void operator+=(const I &item) {this->itemAt(this->highestIndex+1)=item;}
  void operator+=(const I *pitem) {if (pitem) this->itemAt(this->highestIndex+1)=*pitem;}

  Array<I> &operator=(Array<I> &arr) {(*(BasicArray<I>*)this)=arr;return(*this);}

 protected:
  // copy entire contents to a new block, called from setSize
  virtual void moveWholeArray(I *newa,long newSize) {
    FNNOTE3("Array::moveWholeArray %d->%d",this->_aSize,newSize);
    if (newSize<this->highestIndex+1-this->lowestIndex) this->highestIndex=this->lowestIndex+newSize-1;
    BasicAllocableArray<I>::moveWholeArray(newa,newSize);
  }
};




template <class I> class StorableArray : public Array<I>, public ManagedSetItem {
  PubDefCC("sA");
public:
  static StorableArray<I> *construct(ContextIndex &ci,StorableArray<I> *nul,WRD typcc) {return(NEWCONSb(StorableArray<I>,(typcc)));}
  virtual void destroy() {StorableArray<I> *addr=this;DELETE(StorableArray<I>,addr);}

  StorableArray(WRD typcc,long alowestIndex,bool aautoExtendDown,long ahighestIndex,bool aautoExtendUp,const char *aname=NULL) :
    Array<I>(typcc,alowestIndex,aautoExtendDown,ahighestIndex,aautoExtendUp,aname) {}

  StorableArray(WRD typcc=MakeCC("?A"),long ahighestIndex=-1,bool aautoExtendUp=true,const char *aname=NULL) : Array<I>(typcc,ahighestIndex,aautoExtendUp,aname) {}
};













template <class I> class ManagedArray : public BasicArray<I*> {// able to hold ManagedArrayItem's
//  static I __zero;
  ContextIndex ctxt;
 public:
   WRD childTypcc;

  ManagedArray(WRD typcc,WRD childTypcc,ContextIndex &actxt,long alowestIndex,bool aautoExtendDown,long ahighestIndex,bool aautoExtendUp,const char *aname=NULL,bool keepAValid=false) :
       ctxt(actxt),
    BasicArray<I*>(typcc,alowestIndex,aautoExtendDown,ahighestIndex,aautoExtendUp,aname) {
      this->childTypcc=childTypcc;
    if (!keepAValid) this->a=NULL;
  }

  ManagedArray(WRD typcc,WRD childTypcc,long alowestIndex,bool aautoExtendDown,long ahighestIndex,bool aautoExtendUp,const char *aname=NULL,bool keepAValid=false) :
       ctxt(g_contexti),
    BasicArray<I*>(typcc,alowestIndex,aautoExtendDown,ahighestIndex,aautoExtendUp,aname) {
      this->childTypcc=childTypcc;
    if (!keepAValid) this->a=NULL;
  }

  ManagedArray(WRD typcc,WRD childTypcc,ContextIndex &actxt,long ahighestIndex=-1,bool aautoExtendUp=true,const char *aname=NULL,bool keepAValid=false) :
       ctxt(actxt),
    BasicArray<I*>(typcc,ahighestIndex,aautoExtendUp,aname) {
      this->childTypcc=childTypcc;
    if (!keepAValid) this->a=NULL;
  }

  ManagedArray(WRD typcc=MakeCC("M?"),WRD childTypcc=MakeCC("m?"),long ahighestIndex=-1,bool aautoExtendUp=true,const char *aname=NULL,bool keepAValid=false) :
       ctxt(g_contexti),
    BasicArray<I*>(typcc,ahighestIndex,aautoExtendUp,aname) {
      this->childTypcc=childTypcc;
    if (!keepAValid) this->a=NULL;
  }

  virtual ~ManagedArray() {this->clear();}

  I &operator[](long index) {
    FNNOTE2("ManagedArray[%d]",index);
    if ((index<this->lowestIndex)||(index>this->highestIndex)) ERROR4("Not currently allowed to index outside bounds (%d vs [%d,%d])",index,this->lowestIndex,this->highestIndex);
    I *item=this->itemAt(index);
    if (!item) {
      if (!(item=I::construct(ctxt,(I *)NULL,childTypcc))) ERROR2("Item at %d has been dreferenced using [], but is null, and no constructor is available",index);
      this->copy(index,item,1);
    }
    return(*item);
  }

  I &get(long index) {
    FNNOTE2("ManagedArray.get(%d)",index);
    I *item=this->itemAt(index);
    if (!item) {
      if (!(item=I::construct(ctxt,(I *)NULL,childTypcc))) ERROR2("Item at %d has been dreferenced using [], but is null, and no constructor is available",index);
      this->copy(index,item,1);
    }
    return(*item);
  }


  ManagedArray<I> &operator=(ManagedArray<I> &arr) {(*(BasicArray<I*>*)this)=arr;return(*this);}

  void set(const long index,I &item) {
    FNNOTE2("ManagedArray::set %d",index);
    this->extendToRegion(index,index);
    I* pitem=&item;
    this->copy(index,pitem,1);
  }

  void set(const long index,I *item) {
    FNNOTE2("ManagedArray::set %d",index);
    this->extendToRegion(index,index);
    this->copy(index,item,1);
  }

  void operator+=(I &item) {set(this->highestIndex+1,item);}
  void operator+=(const I *pitem) {set(this->highestIndex+1,pitem);}

  // override functions:

  virtual void clearItems(long start,long num) {// called whenever items are nullified
    FNNOTE4("ManagedArray::clearItems %d+%d    (%d)",start,num,this->_aSize);
    if (num<0) ERROR2("clearItems: num is negative (%d)",num);
    if ((start<this->lowerBound())||(start+num>this->upperBound()+1)) ERROR5("clearItems: out of bounds (indexed [%d,%d] in [%d,%d] )",start,start+num-1,this->lowerBound(),this->upperBound());
    I **aa=this->_a+(start-this->lowestIndex);
    while (num--) {
      this->_ensurePtr(aa);
      if (*aa) (*aa)->addRef(-1);
      *(aa++)=NULL;
    }
  }

 protected:

  virtual void copied(long start,long num) {// called whenever items are duplicated
    FNNOTE4("ManagedArray::copied %d+%d  (%d)",start,num,this->_aSize);
    I **aa=this->_a+(start-this->lowestIndex);
    while (num--) {
      this->_ensurePtr(aa);
      if (*aa) (*aa)->addRef(1);
      aa++;
    }
  }

  // copy entire contents to a new block, called from setSize
  virtual void moveWholeArray(I **newa,long newSize) {
    FNNOTE3("ManagedArray::moveWholeArray %d->%d",this->_aSize,newSize);
    if (newSize<this->highestIndex+1-this->lowestIndex) {
      clearItems((newSize-1)+this->lowestIndex,this->highestIndex+1-this->lowestIndex-newSize);
      this->highestIndex=this->lowestIndex+newSize-1;
    }
    BasicAllocableArray<I*>::moveWholeArray(newa,newSize);
  }
};










template <class I> class StorableManagedArray : public ManagedArray<I>, public ManagedSetItem {
 public:
  static StorableManagedArray<I> *construct(ContextIndex &actxt,StorableManagedArray<I> *nul,WRD typcc) {
    return(NEWCONSb(StorableManagedArray<I>,(typcc,MakeCC("m?"),actxt)));
  }
  virtual void destroy() {_DELETE(StorableManagedArray<I>,this);}

  StorableManagedArray(WRD typcc,WRD childTypcc,ContextIndex &actxt,long alowestIndex,bool aautoExtendDown,long ahighestIndex,bool aautoExtendUp,const char *aname=NULL,bool keepAValid=false) :
    ManagedArray<I>(typcc,childTypcc,actxt,alowestIndex,aautoExtendDown,ahighestIndex,aautoExtendUp,aname,keepAValid) {}

  StorableManagedArray(WRD typcc,WRD childTypcc,long alowestIndex,bool aautoExtendDown,long ahighestIndex,bool aautoExtendUp,const char *aname=NULL,bool keepAValid=false) :
    ManagedArray<I>(typcc,childTypcc,alowestIndex,aautoExtendDown,ahighestIndex,aautoExtendUp,aname,keepAValid) {}

  StorableManagedArray(WRD typcc,WRD childTypcc,ContextIndex &actxt,long ahighestIndex=-1,bool aautoExtendUp=true,const char *aname=NULL,bool keepAValid=false) :
    ManagedArray<I>(typcc,childTypcc,actxt,ahighestIndex,aautoExtendUp,aname,keepAValid) {}

  StorableManagedArray(WRD typcc=MakeCC("M?"),WRD childTypcc=MakeCC("m?"),long ahighestIndex=-1,bool aautoExtendUp=true,const char *aname=NULL,bool keepAValid=false) :
    ManagedArray<I>(typcc,childTypcc,ahighestIndex,aautoExtendUp,aname,keepAValid) {}
};










template<class T> T quickSelect(Array<T> &arr,long k,Array<T> &tmparr) {
  if (k<=0) {T minv=arr A[0];IterateArray(T,v,arr,  if (v<minv) minv=v;);return(minv);}
  if (k>=arr.num()-1) {T maxv=arr A[0];IterateArray(T,v,arr,  if (v>maxv) maxv=v;);return(maxv);}

  if (&tmparr!=&arr) tmparr=arr;
  T* a=tmparr.a;
  long left=0,right=arr.num()-1;

  while (true) {
    long pivot=RandInt(right+1-left)+left;

    T pv=a[pivot];
    a[pivot]=a[right];
    long storeIndex=left;
    for (long i=left;i<right;i++) {
      if (a[i]<pv) {
        if (i==storeIndex) storeIndex++;
        else {T tmp=a[i];a[i]=a[storeIndex];a[storeIndex++]=tmp;}
      }
    }
    if (storeIndex!=right) a[right]=a[storeIndex];
    a[storeIndex]=pv;

    if (storeIndex==k) return(pv);
    else if (k<storeIndex) right=storeIndex-1;
    else left=storeIndex+1;
  }
}


template<class T> T quickSelectC(Array<T> &arr,double pcnt,Array<T> &tmparr) {
  if (pcnt<=0) {T minv=arr A[0];IterateArray(T,v,arr,  if (v.v<minv.v) minv=v;);return(minv);}
  if (pcnt>=100) {T maxv=arr A[0];IterateArray(T,v,arr,  if (v.v>maxv.v) maxv=v;);return(maxv);}

  if (&tmparr!=&arr) tmparr=arr;
  T* a=tmparr.a;
    long left=0,right=arr.num()-1;//,ub=right;
  ULL totc=0;IterateArray(T,v,tmparr,   totc+=v.c;);
  ULL leftc=0,rightc=totc,aimc=pcnt*totc/100;

  if (totc==0) SERROR("No counts!");

  while (true) {
    if (left>right) SERROR("QuickSelectC Error, left>right");
    if (left==right) return(a[left]);
    if ((aimc<leftc)||(aimc>=rightc)) SERROR("QuickSelectC Error, aimc<leftc or aimc>=rightc");

    long pivot=RandInt(right+1-left)+left;

    T pv=a[pivot];
    a[pivot]=a[right];
    long storeIndex=left;
    ULL storec=leftc;
    for (long i=left;i<right;i++) {
      if (a[i].v<pv.v) {
        storec+=a[i].c;
        if (i==storeIndex) storeIndex++;
        else {T tmp=a[i];a[i]=a[storeIndex];a[storeIndex++]=tmp;}
      }
    }
    if (storeIndex!=right) a[right]=a[storeIndex];
    a[storeIndex]=pv;

    if (aimc<storec) {right=storeIndex-1;rightc=storec;}
    else if (aimc>=storec+pv.c) {left=storeIndex+1;leftc=storec+pv.c;}
    else return(pv);
  }
}









#ifndef TinyArrayQuantum
#define TinyArrayQuantum 4
#define TinyArrayA1Size 4
#endif

#define TinyArrayA2Size(lb,ubp1) ((ubp1)-(lb)<=TinyArrayA1Size?0:((((ubp1)-1-(lb)-TinyArrayA1Size)/TinyArrayQuantum+1)*TinyArrayQuantum))

template<class I> struct TinyArray {
  I a1[TinyArrayA1Size],*a2;
  long lb,ubp1;
};
extern long tmpTAIndex;
#ifdef MAINCPPFILE
long tmpTAIndex;
#endif

#ifdef GENDEBUG
#define TA_A(ta,i) (((tmpTAIndex=(i)-(ta).lb)<0)||(tmpTAIndex>=(ta).ubp1-(ta).lb)? \
  (SERROR4("Tiny array index out of bounds (%d not in [%d,%d))",tmpTAIndex+(ta).lb,(ta).lb,(ta).ubp1)?(ta).a1[0]:(ta).a1[0]): \
  (tmpTAIndex<TinyArrayA1Size?(ta).a1[tmpTAIndex]:(ta).a2[tmpTAIndex-TinyArrayA1Size]))
#else
#define TA_A(ta,i) ((tmpTAIndex=(i)-(ta).lb)<TinyArrayA1Size?(ta).a1[tmpTAIndex]:(ta).a2[tmpTAIndex-TinyArrayA1Size])
#endif

#define TA_num(ta) ((ta).ubp1-(ta).lb)
#define TA_upperBound(ta) ((ta).ubp1-1)
#define TA_lowerBound(ta) ((ta).lb)

#define TA_CC MakeCC("ta")
#define TA_init(ta) memset(&(ta),0,sizeof(ta))
#define TA_clear(ta) do { \
  long sz=TinyArrayA2Size((ta).lb,(ta).ubp1);if (sz) DELETEA(TA_CC,(ta).a2,sz); \
  TA_init(ta); \
} while(0)


/*
#define TA_checkBounds(ptr,num) do { \
  if (!(((ptr>=arr.a1)&&(ptr+num<=arr.a1+TinyArrayA1Size))|| \
    ((ptr>=arr.a2)&&(ptr+num<=arr.a2+(arr.ubp1-arr.lb-TinyArrayA1Size)))|| \
      ((ptr>=na2)&&(ptr+num<=na2+(n-TinyArrayA1Size))))) SERROR("TinyArray internal error: Pointer appears to be outside bounds"); \
} while(0)
*/

#define TA_memmovecpyNoCheckBounds(to,from,num,fn) do {long _num=(num);if (_num>0) fn(to,from,sizeof(I)*_num);} while(0)
#define TA_memclrNoCheckBounds(ptr,num) do {long _num=(num);if (_num>0) memset(ptr,0,sizeof(I)*_num);} while(0)
/*
#ifdef GENDEBUG
#define TA_memmovecpy(to,from,num,fn) do { \
  long mmc_num=(num); \
  if (mmc_num>0) { \
    I *mmc_to=(to),*mmc_from=(from); \
    TA_checkBounds(mmc_to,mmc_num); \
    TA_checkBounds(mmc_from,mmc_num); \
    fn(mmc_to,mmc_from,sizeof(I)*mmc_num); \
  } \
} while(0)

#define TA_memclr(ptr,num) do { \
  long mc_num=(num); \
  if (mc_num>0) { \
    I *mc_ptr=(ptr); \
    TA_checkBounds(mc_ptr,mc_num); \
    memset(mc_ptr,0,sizeof(I)*mc_num); \
  } \
} while(0)
#else
*/
#define TA_memmovecpy(to,from,num,fn) TA_memmovecpyNoCheckBounds(to,from,num,fn)
#define TA_memclr(ptr,num) TA_memclrNoCheckBounds(ptr,num)
//#endif

#define TA_memmovecpy2(to1,to1n,to2,from,fromn,fn) do { \
  long mmc2_fromn=(fromn); \
  if (mmc2_fromn>0) { \
    long mmc2_to1n=(to1n); \
    I *mmc2_to1=(to1);I *mmc2_to2=(to2);I *mmc2_from=(from); \
    if (mmc2_to1n<=0) { \
      TA_memmovecpy(mmc2_to2-mmc2_to1n,mmc2_from,mmc2_fromn,fn); \
    } \
    else if (mmc2_fromn>mmc2_to1n) { \
      if ((mmc2_to2>=mmc2_from+mmc2_fromn)||(mmc2_to2+(mmc2_fromn-mmc2_to1n)<=mmc2_from)) { \
        TA_memmovecpy(mmc2_to2,mmc2_from+mmc2_to1n,mmc2_fromn-mmc2_to1n,fn); \
        TA_memmovecpy(mmc2_to1,mmc2_from,mmc2_to1n,fn); \
      } \
      else { \
        TA_memmovecpy(mmc2_to1,mmc2_from,mmc2_to1n,fn); \
        TA_memmovecpy(mmc2_to2,mmc2_from+mmc2_to1n,mmc2_fromn-mmc2_to1n,fn); \
      } \
    } \
    else TA_memmovecpy(mmc2_to1,mmc2_from,mmc2_fromn,fn); \
  } \
} while(0)

#define TA_memclr2(to1,to1n,to2,clrn) do { \
  long mc2_clrn=(clrn); \
  if (mc2_clrn>0) { \
    long mc2_to1n=(to1n); \
    I *mc2_to1=(to1);I *mc2_to2=(to2); \
    if (mc2_to1n<=0) { \
      TA_memclr(mc2_to2-mc2_to1n,mc2_clrn); \
    } \
    else if (mc2_clrn>mc2_to1n) { \
      TA_memclr(mc2_to2,mc2_clrn-mc2_to1n); \
      TA_memclr(mc2_to1,mc2_to1n); \
    } \
    else TA_memclr(mc2_to1,mc2_clrn); \
  } \
} while(0)

#define TA_memcpyNoCheckBounds(to,from,num) TA_memmovecpyNoCheckBounds(to,from,num,memcpy)
#define TA_memmoveNoCheckBounds(to,from,num) TA_memmovecpyNoCheckBounds(to,from,num,memmove)
#define TA_memcpy(to,from,num) TA_memmovecpy(to,from,num,memcpy)
#define TA_memmove(to,from,num) TA_memmovecpy(to,from,num,memmove)
#define TA_memcpy2(to1,to1n,to2,from,fromn) TA_memmovecpy2(to1,to1n,to2,from,fromn,memcpy)
#define TA_memmove2(to1,to1n,to2,from,fromn) TA_memmovecpy2(to1,to1n,to2,from,fromn,memmove)





template<class I> void TA_setUpperBound(TinyArray<I> &arr,long ub) {
  if (ub<arr.lb-1) SERROR3("Set to lower than lower bound minus one (%d<%d)",ub,arr.lb);
  long szw=TinyArrayA2Size(arr.lb,arr.ubp1),sz=TinyArrayA2Size(arr.lb,ub+1);
  long nw=arr.ubp1-arr.lb,n=ub+1-arr.lb;
  I *na2=NULL;
  if (szw!=sz) {
    if (szw) {
      if (sz) {
        WRAPNEWA_varExists(TA_CC,I,na2,sz);
        if (n<=nw) TA_memcpy(na2,arr.a2,n-TinyArrayA1Size);
        else {
          TA_memcpy(na2,arr.a2,nw-TinyArrayA1Size);
          TA_memclr(na2+(nw-TinyArrayA1Size),n-nw);
        }
        DELETEA(TA_CC,arr.a2,szw);
        arr.a2=na2;
      }
      else {
        DELETEA(TA_CC,arr.a2,szw);
        arr.a2=NULL;
      }
    }
    else {
      if (nw<TinyArrayA1Size) TA_memclr(arr.a1+nw,TinyArrayA1Size-nw);
      WRAPNEWA_varExists(TA_CC,I,arr.a2,sz);
      arr.ubp1=ub+1;
      TA_memclr(arr.a2,n-TinyArrayA1Size);
    }
  }
  else if (n>nw) {
    if (nw<TinyArrayA1Size) TA_memclr(arr.a1+nw,TinyArrayA1Size-nw);
    arr.ubp1=ub+1;
    if (sz) {
      if (nw<=TinyArrayA1Size) TA_memclr(arr.a2,n-TinyArrayA1Size);
      else TA_memclr(arr.a2+(nw-TinyArrayA1Size),n-nw);
    }
  }
  arr.ubp1=ub+1;
}



template<class I> void TA_setLowerBound(TinyArray<I> &arr,long lb) {
  if (lb>=arr.ubp1+1) SERROR3("Set to higher than upper bound plus one (%d>%d)",lb,arr.ubp1);
  long szw=TinyArrayA2Size(arr.lb,arr.ubp1),sz=TinyArrayA2Size(lb,arr.ubp1);
  long nw=arr.ubp1-arr.lb,n=arr.ubp1-lb;
  I *na2=NULL;
  if (n>nw) {// i.e. shift away from 0
    if (sz!=szw) {
      WRAPNEWA_varExists(TA_CC,I,na2,sz);
      TA_memcpy(na2+(n-nw),arr.a2,nw-TinyArrayA1Size);
      DELETEA(TA_CC,arr.a2,szw);
      arr.a2=na2;
      arr.lb=lb;
    }
    else {
      arr.lb=lb;
      TA_memmove(arr.a2+(n-nw),arr.a2,nw-TinyArrayA1Size);
    }
    TA_memmove2(arr.a1+(n-nw),TinyArrayA1Size-(n-nw),arr.a2,arr.a1,(nw<TinyArrayA1Size?nw:TinyArrayA1Size));
    TA_memclr2(arr.a1,TinyArrayA1Size,arr.a2,n-nw);
  }
  else if (nw>n) {// i.e. shift toward 0
    TA_memmove(arr.a1,arr.a1+(nw-n),TinyArrayA1Size-(nw-n));
    long offs=TinyArrayA1Size-(nw-n);if (offs<0) offs=0;long skipped=offs-(TinyArrayA1Size-(nw-n));
    if (sz!=szw) {
      I *na2=NULL;
      if (sz) {
        WRAPNEWA_varExists(TA_CC,I,na2,sz);
      }
      TA_memcpy2(arr.a1+offs,TinyArrayA1Size-offs,na2,arr.a2+skipped,(nw-TinyArrayA1Size)-skipped);
      DELETEA(TA_CC,arr.a2,szw);
      arr.a2=na2;
    }
    else TA_memmove2(arr.a1+offs,TinyArrayA1Size-offs,arr.a2,arr.a2+skipped,(nw-TinyArrayA1Size)-skipped);
  }
  arr.lb=lb;
}



template<class I> bool TAArrayIsEqual(TinyArray<I> &a,TinyArray<I> &b) {
  return((a.lb==b.lb)&&(a.ubp1==b.ubp1)&&!
    (TA_num(a)<=TinyArrayA1Size?
    memcmp(a.a1,b.a1,sizeof(I)*TA_num(a)):
  (memcmp(a.a1,b.a1,sizeof(I)*TinyArrayA1Size)||memcmp(a.a2,b.a2,sizeof(I)*(TA_num(a)-TinyArrayA1Size)))));
}



template<class I> void TA_append(TinyArray<I> &append,TinyArray<I> &to) {
  long nto=TA_num(to),nfrom=TA_num(append);
  if (nfrom) {
    TA_setUpperBound(to,to.ubp1+nfrom-1);
    if (nto<TinyArrayA1Size) {
      if (nfrom<=TinyArrayA1Size) TA_memcpy2(to.a1+nto,TinyArrayA1Size-nto,to.a2,append.a1,nfrom);
      else {
        TA_memcpy2(to.a1+nto,TinyArrayA1Size-nto,to.a2,append.a1,TinyArrayA1Size);
        TA_memcpy(to.a2+nto,append.a2,nfrom-TinyArrayA1Size);
      }
    }
    else {
      if (nfrom<=TinyArrayA1Size) TA_memcpy(to.a2+(nto-TinyArrayA1Size),append.a1,nfrom);
      else {
        TA_memcpy(to.a2+(nto-TinyArrayA1Size),append.a1,TinyArrayA1Size);
        TA_memcpy(to.a2+nto,append.a2,nfrom-TinyArrayA1Size);
      }
    }
  }
}


template<class I> void TA_insert(TinyArray<I> &arr,long at,I &objs,long num) {
  long n=TA_num(arr);
  TA_setUpperBound(arr,n-1+num);
  if (at>=TinyArrayA1Size) {
    TA_memmove(arr.a2+(at+num-TinyArrayA1Size),arr.a2+(at-TinyArrayA1Size),n-at);
    TA_memcpy(arr.a2+(at-TinyArrayA1Size),&objs,num);
  }
  else if (n+num>TinyArrayA1Size) {
    TA_memmove(arr.a2+num,arr.a2,n-TinyArrayA1Size);
    TA_memmove2(arr.a1+(at+num),TinyArrayA1Size-(num+at),arr.a2,arr.a1+at,TinyArrayA1Size-at);
    TA_memcpy2(arr.a1+at,TinyArrayA1Size-at,arr.a2,&objs,num);
  }
  else {
    TA_memmove(arr.a1+(at+num),arr.a1+at,n-at);
    TA_memcpy(arr.a1+at,&objs,num);
  }
}

template<class I> void TA_remove(TinyArray<I> &arr,long from,long num) {
  long n=TA_num(arr);
  if (from+num>n) num=n-from;
  if (num>0) {
    if (from+num<TinyArrayA1Size) TA_memmove(arr.a1+from,arr.a1+(from+num),TinyArrayA1Size-(from+num));
    if (from<TinyArrayA1Size) TA_memmove2(arr.a1+(TinyArrayA1Size-num),num,arr.a2,arr.a2,n-TinyArrayA1Size);
    else TA_memmove(arr.a2+(from-TinyArrayA1Size),arr.a2+(from+num-TinyArrayA1Size),n-(from+num));
  }
  TA_setUpperBound(arr,n-1-num);
}

#define ForEachMacroInTAArray(var,arr,valueMacro,preTestMacro,postTestMacro) \
  for (long var##LowerBound=(arr).lb,var##UpperBound=(arr).ubp1-1,var##Index=var##LowerBound; \
    preTestMacro(var##Index<=var##UpperBound)&&(&(var=(valueMacro)))postTestMacro; \
    var##Index++)


#define ForEachInTAArray(var,arr) ForEachMacroInTAArray(var,arr,&TA_A(arr,var##Index),(!(var=NULL))&&,)
#define ForEachValueInTAArray(var,arr) ForEachMacroInTAArray(var,arr,TA_A(arr,var##Index),,)
#define ForEachIndexInTAArray(index,arr) \
  for (long index##UpperBound=((index=(arr).lb)*0+(arr).ubp1-1); \
    (index<=index##UpperBound); \
    index++)




#define FilterTAArray(typ,obj,arr,filter) do { \
  long fa_j=(arr).lb-1; \
  typ *fa_p;ForEachInTAArray(fa_p,arr) { \
    typ &obj=*fa_p;long obj##Index=fa_pIndex; \
    if ((filter)&&(obj##Index!=(++fa_j))) TA_A(arr,fa_j)=obj; \
  } \
  TA_setUpperBound(arr,fa_j); \
} while(0)


#define FilterToTAArray(typ,obj,arr,  totyp,toobj,toarr,  filter,  ...) do { \
  if (((void*)&(arr))!=((void*)&(toarr))) { \
    TA_setLowerBound(toarr,(arr).lb); \
    TA_setUpperBound(toarr,(arr).ubp1-1); \
  } \
  long toobj##Index=(arr).lb-1; \
  typ *fa_p;ForEachInTAArray(fa_p,arr) { \
    typ &obj=*fa_p;long obj##Index=fa_pIndex; \
    if (filter) { \
      totyp &toobj=TA_A(toarr,++toobj##Index); \
      __VA_ARGS__ \
    } \
  } \
  TA_setUpperBound(toarr,(arr).lb+toobj##Index); \
} while(0)


#define FilterToTAArray_embeddedFilter(typ,obj,arr,  totyp,toobj,toarr,  ...) do { \
  if (((void*)&(arr))!=((void*)&(toarr))) { \
    TA_setLowerBound(toarr,(arr).lb); \
    TA_setUpperBound(toarr,(arr).ubp1-1); \
  } \
  long toobj##Index=(arr).lb; \
  typ *fa_p;ForEachInTAArray(fa_p,arr) { \
    typ &obj=*fa_p;long obj##Index=fa_pIndex; \
    totyp &toobj=TA_A(toarr,toobj##Index); \
	bool toobj##Keep=true; \
    __VA_ARGS__ \
	if (toobj##Keep) toobj##Index++; \
  } \
  TA_setUpperBound(toarr,(arr).lb+toobj##Index-1); \
} while(0)



#define FilterNormalArrayToTAArray(typ,obj,arr,  totyp,toobj,toarr,  filter,  ...) do { \
  TA_setLowerBound(toarr,(arr).lowerBound()); \
  TA_setUpperBound(toarr,(arr).upperBound()); \
  long toobj##Index=(arr).lowerBound()-1; \
  typ *fa_p;ForEachInArray(fa_p,arr) { \
    typ &obj=*fa_p;long obj##Index=fa_pIndex; \
    if (filter) { \
      totyp &toobj=TA_A(toarr,++toobj##Index); \
      __VA_ARGS__ \
    } \
  } \
  TA_setUpperBound(toarr,toobj##Index); \
} while(0)


#define FilterTAArrayToNormalArray(typ,obj,arr,  totyp,toobj,toarr,  filter,  ...) do { \
  (toarr).setLowerBound((arr).lb); \
  (toarr).setUpperBound((arr).ubp1-1); \
  long toobj##Index=(arr).lb-1;long fatna_lb=(arr).lb; \
  typ *fa_p;ForEachInTAArray(fa_p,arr) { \
    typ &obj=*fa_p;long obj##Index=fa_pIndex; \
    if (filter) { \
      totyp &toobj=toarr.a[(++toobj##Index)-fatna_lb]; \
      __VA_ARGS__ \
    } \
  } \
  toarr.setUpperBound(toobj##Index); \
} while(0)



#define IterateTAArray(typ,obj,arr,...) do { \
  TinyArray<typ> &ita_arr=(arr); \
  long ita_num=TA_num(ita_arr); \
  long ita_num1=(ita_num>TinyArrayA1Size?TinyArrayA1Size:ita_num); \
  long ita_i;long obj##Index; \
  bool obj##ArrayChanged=false; \
  for (ita_i=0;ita_i<ita_num1;ita_i++) { \
    obj##Index=ita_i+ita_arr.lb; \
    typ&obj=ita_arr.a1[ita_i]; \
    __VA_ARGS__ \
    if (obj##ArrayChanged) { \
      obj##ArrayChanged=false; \
      ita_num=TA_num(ita_arr); \
      ita_num1=(ita_num>TinyArrayA1Size?TinyArrayA1Size:ita_num); \
    } \
  } \
  if (ita_i==TinyArrayA1Size) for (;ita_i<ita_num;ita_i++) { \
    obj##Index=ita_i+ita_arr.lb; \
    typ&obj=ita_arr.a2[ita_i-TinyArrayA1Size]; \
    __VA_ARGS__ \
    if (obj##ArrayChanged) { \
      obj##ArrayChanged=false; \
      ita_num=TA_num(ita_arr); \
    } \
  } \
} while(0)

// use this one if applyMacroToEachobj is too large to expand twice
#define IterateTAArray2(typ,arr,...) do { \
  typ *fa_p;ForEachInTAArray(fa_p,arr) { \
    typ &obj=*fa_p; \
long obj##Index=fa_pIndex; \
    __VA_ARGS__ \
  } \
} while(0)



template<class I> void copyTAArray(TinyArray<I> &fromarr,Array<I> &toarr) {
  toarr.setLowerBound(fromarr.lb); 
  toarr.setUpperBound(fromarr.ubp1-1);
  if (TA_num(fromarr)>TinyArrayA1Size) {
    memcpy(toarr.a,fromarr.a1,sizeof(*fromarr.a1)*TinyArrayA1Size);
    memcpy(toarr.a+TinyArrayA1Size,fromarr.a2,sizeof(*fromarr.a1)*(TA_num(fromarr)-TinyArrayA1Size));
  }
  else memcpy(toarr.a,fromarr.a1,sizeof(*fromarr.a1)*TA_num(fromarr));
}

template<class I> void copyTAArray(TinyArray<I> &fromarr,TinyArray<I> &toarr) {
  long sz=TinyArrayA2Size(fromarr.lb,fromarr.ubp1),szw=TinyArrayA2Size(toarr.lb,toarr.ubp1);
  toarr.lb=fromarr.lb;
  toarr.ubp1=fromarr.ubp1;
  memcpy(toarr.a1,fromarr.a1,sizeof(*fromarr.a1)*TinyArrayA1Size);
  if (sz!=szw) {
    DELETEA(TA_CC,toarr.a2,szw);
    if (sz) {
      WRAPNEWA_varExists(TA_CC,I,toarr.a2,sz);
    }
  }
  if (sz) memcpy(toarr.a2,fromarr.a2,sizeof(*fromarr.a1)*(TA_num(fromarr)-TinyArrayA1Size));
}

template<class I> void copyTAArray(Array<I> &fromarr,TinyArray<I> &arr) {
    I *na2=NULL;//long n=0;// needed in the following macros
  TA_setLowerBound(arr,0);
  TA_setUpperBound(arr,fromarr.upperBound());
  TA_setLowerBound(arr,fromarr.lowerBound());
  TA_memcpy2(arr.a1,TinyArrayA1Size,arr.a2,fromarr.a,TA_num(arr));
}

template<class I> void copyTAArray(I* fromarr,long fromnum,TinyArray<I> &arr) {
  I *na2=NULL;//long n=0;// needed in the following macros
  TA_setLowerBound(arr,0);
  TA_setUpperBound(arr,fromnum-1);
  TA_memcpy2(arr.a1,TinyArrayA1Size,arr.a2,fromarr,fromnum);
}




template<class I> ULL makeTAArrayHashCode(TinyArray<I> &a,char op='+') {
  if (a.ubp1<=TinyArrayA1Size) return(makeArrayHashCode(a.a1,sizeof(I)*a.ubp1,op));
  else {
    return(makeArrayHashCode(a.a2,sizeof(I)*(TA_num(a)-TinyArrayA1Size),op,sizeof(I)*TinyArrayA1Size,
      makeArrayHashCode(a.a1,sizeof(I)*TinyArrayA1Size,op))
    );
  }
}



template<class I> bool arrayIsSubset(Array<I> &sub,TinyArray<I> &sup) {
  bool ret;IsSubset(I,sub.a[subObjIndex],sub.num(),TA_A(sup,supObjIndex),TA_num(sup),ret);
  return(ret);
}
template<class I> bool arrayIsSubset(TinyArray<I> &sub,TinyArray<I> &sup) {
  bool ret;IsSubset(I,TA_A(sub,subObjIndex),TA_num(sub),TA_A(sup,supObjIndex),TA_num(sup),ret);
  return(ret);
}
template<class I> bool arrayIsSubset(TinyArray<I> &sub,Array<I> &sup) {
  bool ret;IsSubset(I,TA_A(sub,subObjIndex),TA_num(sub),sup.a[supObjIndex],sup.num(),ret);
  return(ret);
}
template<class I> bool arrayIsProperSubset(Array<I> &sub,TinyArray<I> &sup) {return((sub.num()<TA_num(sup))&&arrayIsSubset(sub,sup));}
template<class I> bool arrayIsProperSubset(TinyArray<I> &sub,TinyArray<I> &sup) {return((TA_num(sub)<TA_num(sup))&&arrayIsSubset(sub,sup));}
template<class I> bool arrayIsProperSubset(TinyArray<I> &sub,Array<I> &sup) {return((TA_num(sub)<sup.num())&&arrayIsSubset(sub,sup));}



// both arra and arrb must be sorted on field
#define IntersectTAArray(typ,field,arra,arrb,toarr) do { \
  long ia_j=-1;long ia_na=TA_num(arra);long ia_nb=TA_num(arrb);long ia_ia=0;long ia_ib=0; \
  if ((&(toarr)!=&(arra))&&(&(toarr)!=&(arrb))) TA_setUpperBound(toarr,(ia_na<ia_nb?ia_na:ia_nb)-1); \
  while((ia_ia<ia_na)&&(ia_ib<ia_nb)) { \
    if ((TA_A(arra,ia_ia)field)<(TA_A2(arrb,ia_ib)field)) ia_ia++; \
    else { \
      if ((TA_A(arra,ia_ia)field)==(TA_A2(arrb,ia_ib)field)) {ia_j++;TA_A(toarr,ia_j)=TA_A2(arra,ia_ia);ia_ia++;} \
      ia_ib++; \
    } \
  } \
  TA_setUpperBound(toarr,ia_j); \
} while(0)

// both arra and arrb must be sorted on field
#define TAArrayIsDisjoint(typ,field,arra,arrb,retBool) do { \
  long aid_na=TA_num(arra);long aid_nb=TA_num(arrb);long aid_ia=0;long aid_ib=0; \
  if (!(aid_na||aid_nb)) retBool=false; \
  else { \
    retBool=true; \
    while((aid_ia<aid_na)&&(aid_ib<aid_nb)) { \
      if ((TA_A(arra,aid_ia)field)<(TA_A2(arrb,aid_ib)field)) aid_ia++; \
      else if ((TA_A(arrb,aid_ib)field)<(TA_A2(arra,aid_ia)field)) aid_ib++; \
      else {retBool=false;break;} \
    } \
  } \
} while(0)

template<class I> void intersectArray(TinyArray<I> &a,TinyArray<I> &b,TinyArray<I> &to);
template <class I> bool arrayIsDisjoint(TinyArray<I> &a,TinyArray<I> &b);
template<class I,class V> I *findArrayElement(TinyArray<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I,class V> long findArrayElementIndex(TinyArray<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I,class V> long findArrayElementInsertionPoint(TinyArray<I> &a,I &sampleElement,V &sampleValue,bool findLeftMost=false);
template<class I> I *findArrayElement(TinyArray<I> &a,I findValue,bool findLeftMost=false);
template<class I> long findArrayElementIndex(TinyArray<I> &a,I findValue,bool findLeftMost=false);
template<class I> long findArrayElementInsertionPoint(TinyArray<I> &a,I findValue,bool findLeftMost=false);
template<class I> long findArrayElementIndexAndInsertionPoint(TinyArray<I> &a,I findValue,long &retInsertionPoint,bool findLeftMost=false);
#ifdef MAINCPPFILE
template <class I> void intersectArray(TinyArray<I> &a,TinyArray<I> &b,TinyArray<I> &to) {IntersectTAArray(I,,a,b,to);}
template <class I> bool arrayIsDisjoint(TinyArray<I> &a,TinyArray<I> &b) {bool ret;TAArrayIsDisjoint(I,,a,b,ret);return(ret);}
template<class I,class V> I *findArrayElement(TinyArray<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost) {
  long foundAtIndex=findArrayElementIndex(a,findValue,sampleElement,sampleValue,findLeftMost);
  return(foundAtIndex==-1?NULL:(a.a+foundAtIndex));
}
/*
template<class I,class V> long findArrayElementIndex(TinyArray<I> &a,V findValue,I &sampleElement,V &sampleValue,bool findLeftMost) {
  DWRD offs=(DWRD)(((void*)&sampleValue)-(void*)&sampleElement);
  long foundAtIndex,insertAtIndex;
  FindArrayElement(V,findValue,*(V*)(((void*)&TA_A(a,index))+offs),TA_lowerBound(a),TA_upperBound(a),foundAtIndex,insertAtIndex,findLeftMost);
  return(foundAtIndex);
}
template<class I,class V> long findArrayElementInsertionPoint(TinyArray<I> &a,I &sampleElement,V &sampleValue,bool findLeftMost) {
  DWRD offs=(DWRD)(((void*)&sampleValue)-(void*)&sampleElement);
  long foundAtIndex,insertAtIndex;
  FindArrayElement(V,findValue,*(V*)(((void*)&TA_A(a,index))+offs),TA_lowerBound(a),TA_upperBound(a),foundAtIndex,insertAtIndex,findLeftMost);
  return(insertAtIndex);
}
*/
template<class I> I *findArrayElement(TinyArray<I> &a,I findValue,bool findLeftMost) {
  long foundAtIndex=findArrayElementIndex(a,findValue,findLeftMost);
  return(foundAtIndex==-1?NULL:&TA_A(a,foundAtIndex));
}
template<class I> long findArrayElementIndex(TinyArray<I> &a,I findValue,bool findLeftMost) {
  long foundAtIndex,insertAtIndex;
  FindArrayElement(I,findValue,TA_A(a,index),TA_lowerBound(a),TA_upperBound(a),foundAtIndex,insertAtIndex,findLeftMost);
  return(foundAtIndex);
}
template<class I> long findArrayElementInsertionPoint(TinyArray<I> &a,I findValue,bool findLeftMost) {
  long foundAtIndex,insertAtIndex;
  FindArrayElement(I,findValue,TA_A(a,index),TA_lowerBound(a),TA_upperBound(a),foundAtIndex,insertAtIndex,findLeftMost);
  return(insertAtIndex);
}
template<class I> long findArrayElementIndexAndInsertionPoint(TinyArray<I> &a,I findValue,long &retInsertionPoint,bool findLeftMost) {
  long foundAtIndex;
  FindArrayElement(I,findValue,TA_A(a,index),TA_lowerBound(a),TA_upperBound(a),foundAtIndex,retInsertionPoint,findLeftMost);
  return(foundAtIndex);
}
#endif




#endif

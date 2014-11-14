//
//  BCArray.m
//  ShaderPlay
//
//  Created by Will Smart on 30/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "BCArray.h"
#include "../Array.h"

#ifdef MAINCPPFILE

#define implementation(IName,I,nsObjectToI,valueToNSObject) \
@implementation BCArray_##IName \
void dealloc { \
    if (myCppArray) DELETE(Array<I>,cppArray); \
    cppArray=NULL; \
} \
 \
id init { \
    if (!(self=[super init])) return(nil); \
    cppArray=NEW(Array<I>); \
    myCppArray=YES; \
    return(self); \
} \
id initWithArray:(void *)vp { \
    if (!(self=[super init])) return(nil); \
    cppArray=(Array<I>*)vp; \
    myCppArray=NO; \
    return(self); \
} \
I* a { \
    return(cppArray->a); \
} \
void removeAllValues { \
    cppArray->clear(); \
} \
void addValue:(I)value { \
    (*cppArray)+=value; \
} \
void insertValue:(I)value atIndex:(int)index { \
    cppArray->insert(index, value, 1); \
} \
void insertValues:(I*)values atIndex:(int)index count:(int)count { \
    cppArray->insert(index, *values, count); \
} \
I valueAtIndex:(int)index { \
    return((*cppArray)[index]); \
} \
void setLowerBound:(int)v { \
    cppArray->setLowerBound(v); \
} \
int lowerBound { \
    return((int)cppArray->lowerBound()); \
} \
void setUpperBound:(int)v { \
    cppArray->setUpperBound(v); \
} \
int upperBound { \
    return((int)cppArray->upperBound()); \
} \
int num { \
    return((int)cppArray->num()); \
} \
NSArray* asNSArray { \
    NSMutableArray *ret=[NSMutableArray arrayWithCapacity:self.num]; \
    IterateArray(I,value,*cppArray,[ret addObject:valueToNSObject];); \
    return(ret.copy); \
} \
void setAsNSArray:(NSArray*)v { \
    int lb=(int)cppArray->lowerBound(); \
    cppArray->setUpperBound(lb+v.count-1); \
    int i=0; \
    for (NSObject *nsObject in v) cppArray->a[i++]=(nsObjectToI); \
} \
CPPTYPE(Array<I> *)cppArray { \
    return(cppArray); \
} \
void setCppArray:(CPPTYPE(Array<I>)*)vp { \
    if (myCppArray) DELETE(Array<I>, cppArray); \
    cppArray=(Array<I>*)vp; \
} \
CPPTYPE(Array<I> *)asCPPArray { \
    Array<I> *ret=new Array<I>(); \
    return(&((*ret)=*cppArray)); \
} \
void setAsCPPArray:(CPPTYPE(Array<I>)*)vp { \
    (*cppArray)=*(Array<I>*)vp; \
} \
@end


implementation(bool,bool,((NSNumber*)nsObject).boolValue,[NSNumber numberWithBool:value])
implementation(char,char,((NSNumber*)nsObject).charValue,[NSNumber numberWithChar:value])
implementation(BYT,BYT,((NSNumber*)nsObject).charValue,[NSNumber numberWithUnsignedChar:value])
implementation(short,short,((NSNumber*)nsObject).shortValue,[NSNumber numberWithShort:value])
implementation(WRD,WRD,((NSNumber*)nsObject).shortValue,[NSNumber numberWithUnsignedShort:value])
implementation(int,int,((NSNumber*)nsObject).intValue,[NSNumber numberWithLong:value])
implementation(DWRD,DWRD,((NSNumber*)nsObject).intValue,[NSNumber numberWithUnsignedInt:value])
implementation(longlong,long long,((NSNumber*)nsObject).longLongValue,[NSNumber numberWithLongLong:value])
implementation(ULL,ULL,((NSNumber*)nsObject).longLongValue,[NSNumber numberWithChar:value])
implementation(BigUInt,BigUInt,*((BigUInt*)((NSMutableData*)nsObject).mutableBytes),[NSMutableData dataWithBytes:&value length:sizeof(BigUInt)])


#undef implementation

#endif



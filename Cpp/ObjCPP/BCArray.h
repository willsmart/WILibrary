//
//  BCArray.h
//  ShaderPlay
//
//  Created by Will Smart on 30/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifndef CPPTYPE
//#ifdef __cplusplus
//#define CPPTYPE(...) __VA_ARGS__
//#else
#define CPPTYPE(...) void
//#endif
#endif

template <class II> class Array;

#define interface(IName,I) @interface BCArray_##IName : NSObject \
{ \
bool myCppArray; \
Array<I> *cppArray; \
} \
-(id)init; \
-(id)initWithArray:(void*)vp; \
@property (assign,nonatomic) NSArray *asNSArray; \
@property CPPTYPE(Array<I>) *cppArray,*asCPPArray; \
@property int upperBound,lowerBound; \
@property (readonly) int num; \
@property (readonly) I *a; \
\
-(I)valueAtIndex:(int)index; \
-(void)insertValue:(I)value atIndex:(int)index; \
-(void)addValue:(I)value; \
-(void)removeAllValues; \
\
@end


interface(bool,bool)
interface(char,char)
interface(BYT,BYT)
interface(short,short)
interface(WRD,WRD)
interface(int,int)
interface(DWRD,DWRD)
interface(longlong,long long)
interface(ULL,ULL)
interface(BigUInt,BigUInt)

#undef interface


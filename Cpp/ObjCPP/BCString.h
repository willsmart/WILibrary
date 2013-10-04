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

template <class I> class StringBuf;

#define interface(I) \
@interface BCStringBuf_##I : NSObject \
{ \
    bool myCppString; \
    StringBuf<I> *cppString; \
} \
-(id)init; \
-(id)initWithCPPString:(void*)vp; \
-(id)initWithNSString:(NSString*)s; \
@property (assign,nonatomic) NSString *asNSString; \
@property CPPTYPE(StringBuf<I>) *cppString,*asCPPString; \
@property I *asCString; \
@property const I *asConstCString; \
-(void)copyCharString:(const char*)cs; \
@property int length; \
-(I)characterAtIndex:(int)index; \
-(void)setCharacter:(I)c atIndex:(int)index; \
-(void)clear; \
-(bool)isEqualToCPPString:(CPPTYPE(StringBuf<I>)*)vp; \
-(bool)isNotEqualToCString:(const I*)cs; \
-(bool)isNotEqualToCPPString:(CPPTYPE(StringBuf<I>)*)vp; \
-(bool)isEqualToCString:(const I*)cs; \
-(bool)isLessThanCPPString:(CPPTYPE(StringBuf<I>)*)vp; \
-(bool)isLessThanCString:(const I*)cs; \
-(bool)isGreaterThanCPPString:(CPPTYPE(StringBuf<I>)*)vp; \
-(bool)isGreaterThanCString:(const I*)cs; \
-(bool)startsWithCString:(const I *)cmp; \
-(bool)endsWithCString:(const I *)cmp; \
-(bool)containsCString:(const I *)cmp; \
-(int)indexOfCString:(const I *)cmp; \
-(int)relationToCPPString:(CPPTYPE(StringBuf<I>)*)vp; \
-(int)relationToCString:(const I *)cmp; \
-(void)copyFromCString:(const I *)copy withLength:(int)length; \
-(void)appendCString:(const I *)copy withLength:(int)length; \
-(void)appendCPPString:(CPPTYPE(StringBuf<I>)*)vp; \
-(void)appendCString:(const I *)cs; \
-(void)format:(const char *)format; \
-(void)appendFormat:(const char *)format; \
@end


interface(char)
interface(unichar)
interface(UTF32Char)

#undef interface

@interface BCString : BCStringBuf_char

@end
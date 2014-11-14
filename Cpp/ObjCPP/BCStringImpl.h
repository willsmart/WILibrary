//
//  BCArray.m
//  ShaderPlay
//
//  Created by Will Smart on 30/08/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "BCString.h"
#include "../String.h"

#ifdef MAINCPPFILE

#define implementation(I) \
@implementation BCStringBuf_##I \
void dealloc { \
    if (myCppString) DELETE(StringBuf<I>, cppString); \
    cppString=NULL; \
} \
 \
id init { \
    if (!(self=[super init])) return(nil); \
    cppString=NEW(StringBuf<I>); \
    myCppString=YES; \
    return(self); \
} \
id initWithCPPString:(void *)vp { \
    if (!(self=[super init])) return(nil); \
    cppString=(StringBuf<I>*)vp; \
    myCppString=NO; \
    return(self); \
} \
id initWithNSString:(NSString *)s { \
    if (!(self=[super init])) return(nil); \
    cppString=NEW(StringBuf<I>); \
    myCppString=YES; \
    self.asNSString=s; \
    return(self); \
} \
 \
NSString* asNSString { \
    if (sizeof(I)==1) return([NSString stringWithFormat:@"%s",(const char*)cppString->a]); \
    else if (sizeof(I)==2) return([NSString stringWithFormat:@"%S",(const unsigned short*)cppString->a]); \
    else return(nil); \
} \
void setAsNSString:(NSString*)v { \
    if (sizeof(I)==1) self.asConstCString=(I*)[v cStringUsingEncoding:NSUnicodeStringEncoding]; \
    else if (sizeof(I)==2) { \
        int l; \
        cppString->setLength(l=(int)v.length); \
        const UTF32Char *pc=(const UTF32Char*)[v cStringUsingEncoding:NSUTF32LittleEndianStringEncoding]; \
        for (int i=0;i<l;i++) cppString->a[i]=(pc[i]&0xffff0000?0:*(I*)(pc+i)); \
    } \
    else if (sizeof(I)==4) self.asConstCString=(I*)[v cStringUsingEncoding:NSUTF32LittleEndianStringEncoding]; \
} \
 \
CPPTYPE(StringBuf<I> *)cppString { \
    return(cppString); \
} \
void setCppString:(CPPTYPE(StringBuf<I>)*)vp { \
    if (myCppString) DELETE(StringBuf<I>, cppString); \
    cppString=(StringBuf<I>*)vp; \
} \
CPPTYPE(StringBuf<I> *)asCPPString { \
    StringBuf<I> *ret=new StringBuf<I>(); \
    return(&((*ret)=*cppString)); \
} \
void setAsCPPString:(CPPTYPE(StringBuf<I>)*)vp { \
    (*cppString)=*(StringBuf<I>*)vp; \
} \
I* asCString { \
    return(cppString->a); \
} \
const I* asConstCString { \
    return(cppString->a); \
} \
void setAsCString:(I*)cs { \
    (*cppString)=cs; \
} \
void setAsConstCString:(const I*)cs { \
    (*cppString)=cs; \
} \
void copyCharString:(const char*)cs { \
    cppString->copyFrom(cs); \
} \
int length { \
    return((int)cppString->length()); \
} \
void setLength:(int)v { \
    cppString->setLength(v); \
} \
I characterAtIndex:(int)index { \
    return((*cppString)[index]); \
} \
void setCharacter:(I)c atIndex:(int)index { \
    (*cppString)[index]=c; \
} \
void clear { \
    cppString->clear(); \
} \
bool isEqualToCPPString:(CPPTYPE(StringBuf<I>)*)vp { \
    return((*cppString)==*(StringBuf<I>*)vp); \
} \
bool isNotEqualToCString:(const I*)cs { \
    return((*cppString)==cs); \
} \
bool isNotEqualToCPPString:(CPPTYPE(StringBuf<I>)*)vp { \
    return((*cppString)!=*(StringBuf<I>*)vp); \
} \
bool isEqualToCString:(const I*)cs { \
    return((*cppString)!=cs); \
} \
bool isLessThanCPPString:(CPPTYPE(StringBuf<I>)*)vp { \
    return((*cppString)<*(StringBuf<I>*)vp); \
} \
bool isLessThanCString:(const I*)cs { \
    return((*cppString)<cs); \
} \
bool isGreaterThanCPPString:(CPPTYPE(StringBuf<I>)*)vp { \
    return((*cppString)>*(StringBuf<I>*)vp); \
} \
bool isGreaterThanCString:(const I*)cs { \
    return((*cppString)>cs); \
} \
bool startsWithCString:(const I *)cmp { \
    return(cppString->startsWith(cmp)); \
} \
bool endsWithCString:(const I *)cmp { \
    return(cppString->endsWith(cmp)); \
} \
bool containsCString:(const I *)cmp { \
    return(cppString->contains(cmp)); \
} \
int indexOfCString:(const I *)cmp { \
    return((int)cppString->indexOf(cmp)); \
} \
int relationToCPPString:(CPPTYPE(StringBuf<I>)*)vp { \
    return((int)cppString->relationTo(*(StringBuf<I>*)vp)); \
} \
int relationToCString:(const I *)cmp { \
    return((int)cppString->relationTo(cmp)); \
} \
void copyFromCString:(const I *)copy withLength:(int)length { \
    cppString->memCpy(copy,length); \
} \
void appendCString:(const I *)copy withLength:(int)length { \
    cppString->memCat(copy,length); \
} \
void appendCPPString:(CPPTYPE(StringBuf<I>)*)vp { \
    (*cppString)+=*(StringBuf<I>*)vp; \
} \
void appendCString:(const I *)cs { \
    (*cppString)+=cs; \
} \
void format:(const char *)format { \
    cppString->format(format); \
} \
void appendFormat:(const char *)format { \
    cppString->addFormat(format); \
} \
@end


implementation(char)
implementation(unichar)
implementation(UTF32Char)

@implementation BCString
@end

#undef implementation

#endif


#ifndef __AGE_TYPE_H__
#define __AGE_TYPE_H__

#ifndef bl
#	define bl bool
#endif
#ifndef TRUE
#	define TRUE 1
#endif
#ifndef FALSE
#	define FALSE 0
#endif

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

typedef s8* Str;
typedef void* Ptr;

typedef unsigned int Color;

typedef struct {
	s32 x;
	s32 y;
} Point;

typedef struct {
	s32 w;
	s32 h;
} Size;

#endif /* __AGE_TYPE_H__ */

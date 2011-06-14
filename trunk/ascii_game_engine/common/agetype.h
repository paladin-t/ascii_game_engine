#ifndef __AGE_TYPE_H__
#define __AGE_TYPE_H__

typedef enum {
	TRUE,
	FALSE,
} BooleanEnum;

#ifndef bool
#	define bool enum BooleanEnum
#endif

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#endif /* __AGE_TYPE_H__ */

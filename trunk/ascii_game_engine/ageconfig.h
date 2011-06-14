#ifndef __AGE_CONFIG_H__
#define __AGE_CONFIG_H__

#ifdef _MSC_VER
#	ifdef INTE_COMPILE
#		define AGE_API __declspec(dllexport)
#	else
#		define AGE_API __declspec(dllimport)
#	endif
#	define AGE_INTERNAL
#else
#	define AGE_API __attribute__ ((visibility("default")))
#	define AGE_INTERNAL __attribute__ ((visibility("hidden")))
#endif

#endif /* __AGE_CONFIG_H__ */

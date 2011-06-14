#include "age.h"

#define _VER_MAJOR 1
#define _VER_MINOR 0
#define _VER_REVISION 1
#define _AGE_VERSION ((_VER_MAJOR << 24) | (_VER_MINOR << 16) | (_VER_REVISION))
#define _AGE_VERSION_STRING "1.0.0001"

u32 getVer(void) {
	return _AGE_VERSION;
}

const s8* getVerString(void) {
	return _AGE_VERSION_STRING;
}

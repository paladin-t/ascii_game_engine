#ifdef _MSC_VER
#       include <crtdbg.h>
#endif // _MSC_VER
#include <stdio.h>
#include <stdlib.h>

#include "age.h"

static void __exit(void) {
#ifdef _MSC_VER
	int c = _CrtDumpMemoryLeaks();

	if(0 != c) {
		_asm int 3
	}
#endif /* _MSC_VER */
}

int main(int argc, char* argv[]) {
#ifdef _MSC_VER
	_CrtSetBreakAlloc(0);
#endif /* _MSC_VER */

	atexit(__exit);

	return 0;
}

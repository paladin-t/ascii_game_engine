#ifdef _MSC_VER
#	include <windows.h>
#	include <crtdbg.h>
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

	{
		HANDLE hConsole;
		int k;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		// you can loop k higher to see more color choices
		for(k = 1; k < 255; k++) {
			// pick the colorattribute k you want
			SetConsoleTextAttribute(hConsole, k);
			printf("%d  I want to be nice today!\n", k);
		}
	}

	return 0;
}

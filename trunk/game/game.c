/*
** This source file is part of MY-BASIC
**
** For the latest info, see http://code.google.com/p/my-basic/
**
** Copyright (c) 2011 Tony & Tony's Toy Game Development Team
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

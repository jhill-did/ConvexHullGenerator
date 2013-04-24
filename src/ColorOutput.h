#ifndef COLOROUPUT_H_INCLUDED
#define COLOROUPUT_H_INCLUDED
#include <windows.h>

namespace CO
{
    void printC(const char* str, WORD color);

    const static WORD COLOR_RED   = 0x0C;
    const static WORD COLOR_GREEN = 0xA0;
    const static WORD COLOR_ERROR = 0xC0;
    const static WORD COLOR_INFO  = 0xE0;
}



#endif // COLOROUPUT_H_INCLUDED

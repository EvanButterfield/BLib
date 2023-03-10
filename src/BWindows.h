/* date = March 3rd 2023 9:55 pm */

#ifndef _B_WINDOWS_H
#define _B_WINDOWS_H

#define AssertHR(X) Assert(SUCCEEDED(X))

typedef struct platform_data
{
    HINSTANCE Instance;
    HWND Window;
    
    s64 PerfCountFrequency;
    b32 SleepIsGranular;
    
    s32 Width;
    s32 Height;
} platform_data;

#endif //_B_WINDOWS_H

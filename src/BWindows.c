#include "BWindows.h"

global platform_data PlatformData;

global LARGE_INTEGER LastCounter;

internal LRESULT CALLBACK
Win32WindowProc(HWND Window, UINT Message,
                WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;
    
    switch(Message)
    {
        case WM_CLOSE:
        {
            ShouldClose = true;
        } break;
        
        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }
    
    return(Result);
}

internal inline LARGE_INTEGER
Win32GetWallClock(void)
{
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return(Result);
}

internal inline f32
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
    f32 Result = ((f32)(End.QuadPart - Start.QuadPart) /
                  (f32)PlatformData.PerfCountFrequency);
    return(Result);
}

internal void
InitializePlatform(int Width, int Height, wchar_t *WindowName)
{
    PlatformData.Instance = GetModuleHandle(0);
    
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    PlatformData.PerfCountFrequency = PerfCountFrequencyResult.QuadPart;
    
    // NOTE(evan): Set the Windows schedular granularity to 1 ms
    //             so that Sleep() can be more granular
    UINT DesiredSchedularMS = 1;
    b32 SleepIsGranular = (timeBeginPeriod(DesiredSchedularMS) == TIMERR_NOERROR);
    
    WNDCLASSW WindowClass = {0};
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32WindowProc;
    WindowClass.hInstance = PlatformData.Instance;
    WindowClass.hCursor = LoadCursorW(0, IDC_ARROW);
    WindowClass.lpszClassName = L"BLibWindowClass";
    
    LastCounter = Win32GetWallClock();
    
    if(RegisterClassW(&WindowClass))
    {
        PlatformData.Window = CreateWindowW(WindowClass.lpszClassName, WindowName,
                                            WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE,
                                            CW_USEDEFAULT, CW_USEDEFAULT, Width, Height,
                                            0, 0, PlatformData.Instance, 0);
    }
}

// NOTE(evan): Returns delta time
internal f32
UpdatePlatform(void)
{
    LARGE_INTEGER EndCounter = Win32GetWallClock();
    f32 DeltaTime = Win32GetSecondsElapsed(LastCounter, EndCounter);
    LastCounter = EndCounter;
    
    MSG Message;
    while(PeekMessageW(&Message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&Message);
        DispatchMessageW(&Message);
    }
    
    {
        RECT Rect;
        GetClientRect(PlatformData.Window, &Rect);
        PlatformData.Width = Rect.right - Rect.left;
        PlatformData.Height = Rect.bottom - Rect.top;
    }
    
    return(DeltaTime);
}
#include <windows.h>
#include <string>
#include <iostream>

// Link against the User32 library
#pragma comment(lib, "user32.lib")

// Exported function to be called by rundll32 (PowerShell Command)
extern "C" __declspec(dllexport) void CALLBACK ps(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    if (lpszCmdLine == nullptr || strlen(lpszCmdLine) == 0) {
        MessageBoxA(nullptr,
            "No command provided!\n\nUsage:\n"
            "  rundll32.exe <dll-path>,ps <PowerShell-Command>\n\n"
            "Example:\n"
            "  rundll32.exe MyLib.dll,ps \"Get-Process\"",
            "Error",
            MB_OK | MB_ICONERROR);
        return;
    }

    std::string command(lpszCmdLine);
    std::string powershellCommand = "powershell.exe -NoExit -ExecutionPolicy Bypass -Command \"" + command + "\"";

    // Execute the PowerShell command
    int result = system(powershellCommand.c_str());
    if (result != 0) {
        MessageBoxA(nullptr, "Failed to execute PowerShell command.", "Error", MB_OK | MB_ICONERROR);
    }
}

// Exported function to be called by rundll32 (Executable Launcher)
extern "C" __declspec(dllexport) void CALLBACK exe(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    if (lpszCmdLine == nullptr || strlen(lpszCmdLine) == 0) {
        MessageBoxA(nullptr,
            "No executable path provided!\n\nUsage:\n"
            "  rundll32.exe <dll-path>,exe <executable-path>\n\n"
            "Example:\n"
            "  rundll32.exe MyLib.dll,exe \"notepad.exe\"",
            "Error",
            MB_OK | MB_ICONERROR);
        return;
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcessA(nullptr, lpszCmdLine, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        MessageBoxA(nullptr, "Failed to launch executable.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Wait for the process to finish
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

#include <windows.h>
#include <string>
#include <iostream>

// Link against the User32 library
#pragma comment(lib, "user32.lib")

// Exported function to be called by rundll32
extern "C" __declspec(dllexport) void CALLBACK RunPowerShell(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    if (lpszCmdLine == nullptr || strlen(lpszCmdLine) == 0) {
        MessageBoxA(nullptr, "No command provided!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Parse the PowerShell command from the arguments
    std::string command(lpszCmdLine);

    // Construct the PowerShell execution command
    std::string powershellCommand = "powershell.exe -Command \"" + command + "\"";

    // Execute the PowerShell command
    int result = system(powershellCommand.c_str());
    if (result != 0) {
        MessageBoxA(nullptr, "Failed to execute PowerShell command.", "Error", MB_OK | MB_ICONERROR);
    }
}

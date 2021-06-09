#include <iostream>
#include <windows.h>

using namespace std;

// Idle time in milliseconds
unsigned int idle_time;

POINT m_start;
POINT m;

void moveMouse() {
    if (GetCursorPos(&m)) {
        double fScreenWidth = GetSystemMetrics( SM_CXSCREEN )-1; 
        double fScreenHeight = GetSystemMetrics( SM_CYSCREEN )-1;
        double fx = (m.x != m_start.x) ? (m.x + 1)*(65535.0f/fScreenWidth) : (m.x - 1)*(65535.0f/fScreenWidth);
        double fy = m.y*(65535.0f/fScreenHeight);
        INPUT Input={0};
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
        Input.mi.dx = fx;
        Input.mi.dy = fy;
        SendInput(1,&Input,sizeof(INPUT));
    }
}

int main() {

    idle_time = GetPrivateProfileInt(L"idle", L"idle_time", 0, L"config.ini");

    if (idle_time != 0) {
        ShowWindow(GetConsoleWindow(), SW_HIDE);

        if (GetCursorPos(&m_start)) {
            
            LASTINPUTINFO last_input;
            DWORD current_idle_time;

            last_input.cbSize = sizeof(last_input);

            for(;;) {
                if (!GetLastInputInfo( &last_input )) {
                    cerr << "Failed to get input info" << endl;
                }
                current_idle_time = GetTickCount() - last_input.dwTime;
                if (current_idle_time > idle_time) {
                    moveMouse();
                }
            }
        }
    } else {
        cerr << "Idle time must be more than 0" << endl;
    }
}
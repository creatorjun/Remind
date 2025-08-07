#pragma once

#include <windows.h>

class View {
public:
    View(HINSTANCE hInstance);
    bool Create(HWND hParent, const wchar_t* title, int x, int y, int width, int height);
    void RunMessageLoop();

private:
    // C++ 클래스 멤버 함수를 Win32 콜백으로 직접 사용할 수 없으므로,
    // 정적(static) 함수를 통해 연결하는 브릿지 역할을 만듭니다.
    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT MemberWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HWND m_hListBox1;
    HWND m_hListBox2;
};
#include "View.h"
#include "Worker.h" // 샘플 데이터를 가져오기 위해 포함

#define WINDOW_ALPHA 200
#define IDC_LISTBOX1 101
#define IDC_LISTBOX2 102

View::View(HINSTANCE hInstance)
    : m_hInstance(hInstance), m_hWnd(NULL), m_hListBox1(NULL), m_hListBox2(NULL) {}

bool View::Create(HWND hParent, const wchar_t* title, int x, int y, int width, int height) {
    const wchar_t CLASS_NAME[] = L"Desktop Widget Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = View::StaticWindowProc; // 정적 멤버 함수를 콜백으로 지정
    wc.hInstance = m_hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    RegisterClass(&wc);

    // CreateWindowEx의 마지막 인자로 this 포인터를 전달하여
    // StaticWindowProc에서 이 클래스 인스턴스에 접근할 수 있도록 합니다.
    m_hWnd = CreateWindowEx(
        WS_EX_LAYERED, CLASS_NAME, title, WS_POPUP,
        x, y, width, height,
        hParent, NULL, m_hInstance, this // 마지막 인자로 this 전달
    );

    if (!m_hWnd) return false;

    SetLayeredWindowAttributes(m_hWnd, 0, WINDOW_ALPHA, LWA_ALPHA);
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    return true;
}

void View::RunMessageLoop() {
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// 정적(static) 멤버 함수: C 스타일 콜백 함수의 역할을 합니다.
LRESULT CALLBACK View::StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    View* pView = nullptr;
    if (uMsg == WM_NCCREATE) {
        // 창이 생성될 때 CreateWindowEx에서 전달한 this 포인터를 받아서 저장
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pView = (View*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pView);
    } else {
        // 생성 이후에는 저장된 this 포인터를 가져와서 사용
        pView = (View*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pView) {
        // this 포인터를 통해 실제 메시지를 처리할 멤버 함수 호출
        return pView->MemberWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// 실제 메시지를 처리하는 멤버 함수
LRESULT View::MemberWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            m_hListBox1 = CreateWindowEx(0, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY, 0, 0, 0, 0, hwnd, (HMENU)IDC_LISTBOX1, m_hInstance, NULL);
            m_hListBox2 = CreateWindowEx(0, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY, 0, 0, 0, 0, hwnd, (HMENU)IDC_LISTBOX2, m_hInstance, NULL);
            
            Worker tempWorker; // 임시 워커 객체로 데이터에 접근
            for (const auto& item : tempWorker.GetSampleData1()) SendMessage(m_hListBox1, LB_ADDSTRING, 0, (LPARAM)item.c_str());
            for (const auto& item : tempWorker.GetSampleData2()) SendMessage(m_hListBox2, LB_ADDSTRING, 0, (LPARAM)item.c_str());
            return 0;
        }
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            int padding = 10;
            int spacing = 10;
            int listHeight = (height - (padding * 2) - spacing) / 2;
            MoveWindow(m_hListBox1, padding, padding, width - (padding * 2), listHeight, TRUE);
            MoveWindow(m_hListBox2, padding, padding + listHeight + spacing, width - (padding * 2), listHeight, TRUE);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
            EndPaint(hwnd, &ps);
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
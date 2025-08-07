#include <windows.h>
#include "Worker.h"
#include "View.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    // 1. Worker를 통해 필요한 정보를 가져옵니다.
    Worker worker;
    RECT monitorRect = worker.GetRightmostMonitorRect();
    HWND hDesktopParent = worker.FindDesktopWorkerW();

    // 2. 계산된 정보를 바탕으로 창의 크기와 위치를 정합니다.
    int width = (monitorRect.right - monitorRect.left) / 2;
    int height = monitorRect.bottom - monitorRect.top;
    int x = monitorRect.left + width;
    int y = monitorRect.top;

    // 3. View 객체를 생성하고 실행합니다.
    View view(hInstance);
    if (view.Create(hDesktopParent, L"리스트 위젯 (리팩토링됨)", x, y, width, height)) {
        view.RunMessageLoop();
    }

    return 0;
}
#include "Worker.h"

namespace {
    // 이 파일 안에서만 사용할 전역 변수와 구조체
    HWND g_hWorkerW = NULL;
    struct MonitorInfo { RECT rc; bool is_set; };

    BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData) {
        MonitorInfo* pmi = (MonitorInfo*)dwData;
        MONITORINFO mi = { sizeof(mi) };
        if (GetMonitorInfo(hMonitor, &mi)) {
            if (!pmi->is_set || mi.rcMonitor.right > pmi->rc.right) {
                pmi->rc = mi.rcMonitor;
                pmi->is_set = true;
            }
        }
        return TRUE;
    }

    BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM) {
        HWND p = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
        if (p != NULL) {
            g_hWorkerW = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
        }
        return TRUE;
    }
}

Worker::Worker() {
    // 생성자에서 샘플 데이터를 초기화합니다.
    m_sampleData1 = { L"C++ 프로젝트 시작하기", L"Win32 API 기초", L"메시지 루프의 이해", L"윈도우 클래스 등록", L"CreateWindowEx 함수", L"자식 컨트롤 생성", L"리스트 박스 다루기", L"GDI 그래픽 출력" };
    m_sampleData2 = { L"TASK-001: UI 디자인", L"TASK-002: 데이터 구조 설계", L"TASK-003: 핵심 로직 구현", L"TASK-004: 모듈별 테스트", L"TASK-005: 디버깅 및 최적화", L"TASK-006: 사용자 문서 작성", L"TASK-007: 최종 빌드", L"TASK-008: 배포 준비" };
}

RECT Worker::GetRightmostMonitorRect() {
    MonitorInfo rightmost_monitor = {0};
    rightmost_monitor.is_set = false;
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&rightmost_monitor);
    return rightmost_monitor.rc;
}

HWND Worker::FindDesktopWorkerW() {
    HWND progman = FindWindow(L"Progman", NULL);
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);
    g_hWorkerW = NULL;
    EnumWindows(EnumWindowsProc, 0);
    return g_hWorkerW;
}

const std::vector<std::wstring>& Worker::GetSampleData1() const {
    return m_sampleData1;
}

const std::vector<std::wstring>& Worker::GetSampleData2() const {
    return m_sampleData2;
}
#pragma once // 헤더 파일이 중복으로 포함되는 것을 방지합니다.

#include <windows.h>
#include <vector>
#include <string>

class Worker {
public:
    Worker(); // 생성자
    RECT GetRightmostMonitorRect();
    HWND FindDesktopWorkerW();
    const std::vector<std::wstring>& GetSampleData1() const;
    const std::vector<std::wstring>& GetSampleData2() const;

private:
    std::vector<std::wstring> m_sampleData1;
    std::vector<std::wstring> m_sampleData2;
};
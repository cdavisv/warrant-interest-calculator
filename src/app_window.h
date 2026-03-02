#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include <windows.h>
#include <commctrl.h>
#include <string>

class AppWindow {
public:
    explicit AppWindow(HINSTANCE hInstance);
    ~AppWindow();

    bool create();
    int run();

private:
    static LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    // Date entry subclass for auto-formatting
    static LRESULT CALLBACK dateEditSubclassProc(
        HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam,
        UINT_PTR subclassId, DWORD_PTR refData);

    // Screen management
    void clearScreen();
    void showMainMenu();
    void showCalculator();

    // Calculation
    void calculate();
    void showError(const std::wstring& message);

    // Helpers
    std::wstring getControlText(HWND hCtrl);
    void setFont(HWND hCtrl, HFONT hFont);

    HINSTANCE hInstance_;
    HWND hWnd_ = nullptr;

    // Fonts
    HFONT hTitleFont_ = nullptr;
    HFONT hSubtitleFont_ = nullptr;
    HFONT hNormalFont_ = nullptr;
    HFONT hResultFont_ = nullptr;

    // Current control handles (valid only for current screen)
    HWND hWarrantEntry_ = nullptr;
    HWND hInterestEntry_ = nullptr;
    HWND hStartDateEntry_ = nullptr;
    HWND hEndDateEntry_ = nullptr;
    HWND hResultsLabel_ = nullptr;

    static constexpr int WINDOW_WIDTH = 520;
    static constexpr int WINDOW_HEIGHT = 420;
    static constexpr const wchar_t* WINDOW_CLASS = L"WarrantCalculatorClass";
    static constexpr const wchar_t* WINDOW_TITLE = L"Libby Drainage District Warrant Calculator";
    static constexpr const wchar_t* DEFAULT_RATE = L"6.00";
};

#endif
